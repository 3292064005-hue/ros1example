#!/usr/bin/env python3
import os
import pty
import socket
import select
import rospy
import time


class TcpSocketServerNode:
    def __init__(self):
        self.tcp_port = rospy.get_param('~tcp_port', 8889)
        self.serial_port = rospy.get_param('~serial_port', '/tmp/laserport')

    def run(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        sock.bind(('0.0.0.0', self.tcp_port))
        sock.listen(1)

        master, slave = pty.openpty()
        if os.path.exists(self.serial_port):
            os.remove(self.serial_port)
        os.symlink(os.ttyname(slave), self.serial_port)

        rospy.loginfo('TCP端口:%s 已映射到串口:%s', self.tcp_port, self.serial_port)
        mypoll = select.poll()
        mypoll.register(master, select.POLLIN)
        try:
            while not rospy.is_shutdown():
                rospy.loginfo('等待接受连接...')
                client, addr = sock.accept()
                mypoll.register(client.fileno(), select.POLLIN)
                rospy.loginfo('来自 %s 的连接已建立', addr)
                alive = True
                last_exchange = time.time()
                try:
                    while alive and not rospy.is_shutdown():
                        fdlist = mypoll.poll(256)
                        for fd, _ in fdlist:
                            last_exchange = time.time()
                            data = os.read(fd, 256)
                            write_fd = client.fileno() if fd == master else master
                            if len(data) == 0:
                                alive = False
                                break
                            os.write(write_fd, data)
                        if time.time() - last_exchange > 5.0:
                            rospy.logwarn('5s no data, disconnecting client')
                            alive = False
                except Exception as exc:
                    rospy.logwarn('连接异常中断: %s', exc)
                finally:
                    try:
                        mypoll.unregister(client.fileno())
                    except Exception:
                        pass
                    client.close()
        finally:
            sock.close()


def main():
    rospy.init_node('tcp_socket_server_node')
    TcpSocketServerNode().run()


if __name__ == '__main__':
    main()
