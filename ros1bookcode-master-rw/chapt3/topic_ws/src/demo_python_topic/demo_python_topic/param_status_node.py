#!/usr/bin/env python3
import rospy
from std_msgs.msg import String


def main():
    rospy.init_node('param_status_node')

    robot_name = rospy.get_param('~robot_name', 'fishbot')
    message = rospy.get_param('~message', 'ready')
    topic = rospy.get_param('~topic', '/demo/param_status')
    rate_value = rospy.get_param('~rate', 1.0)
    if rate_value <= 0:
        rospy.logwarn('~rate 必须大于 0，已回退到 1.0 Hz')
        rate_value = 1.0

    pub = rospy.Publisher(topic, String, queue_size=10)
    rate = rospy.Rate(rate_value)

    rospy.loginfo('参数状态示例启动: robot_name=%s, topic=%s, rate=%.2f',
                  robot_name, topic, rate_value)
    while not rospy.is_shutdown():
        stamp = rospy.Time.now().to_sec()
        pub.publish(String(data='{}: {} @ {:.3f}'.format(robot_name, message, stamp)))
        rate.sleep()


if __name__ == '__main__':
    main()
