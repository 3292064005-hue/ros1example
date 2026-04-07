#!/usr/bin/env python3
import platform
import psutil
import rospy
from status_interfaces.msg import SystemStatus

class SysStatusPub:
    def __init__(self):
        self.pub = rospy.Publisher('sys_status', SystemStatus, queue_size=10)
        self.timer = rospy.Timer(rospy.Duration(1.0), self.timer_callback)

    def timer_callback(self, _event):
        cpu_percent = psutil.cpu_percent()
        memory_info = psutil.virtual_memory()
        net_io_counters = psutil.net_io_counters()
        msg = SystemStatus()
        msg.stamp = rospy.Time.now()
        msg.host_name = platform.node()
        msg.cpu_percent = cpu_percent
        msg.memory_percent = memory_info.percent
        msg.memory_total = memory_info.total / 1024 / 1024
        msg.memory_available = memory_info.available / 1024 / 1024
        msg.net_sent = net_io_counters.bytes_sent / 1024 / 1024
        msg.net_recv = net_io_counters.bytes_recv / 1024 / 1024
        rospy.loginfo('发布系统状态')
        self.pub.publish(msg)

if __name__ == '__main__':
    rospy.init_node('sys_status_pub')
    SysStatusPub()
    rospy.spin()
