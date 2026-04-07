#!/usr/bin/env python3
import rospy
from nav_msgs.msg import Odometry

class OdomPublisherSubscriber:
    def __init__(self):
        self.odom_publisher = rospy.Publisher('odom', Odometry, queue_size=10, latch=True)
        self.odom_subscriber = rospy.Subscriber('odom', Odometry, self.odom_callback, queue_size=10)
        self.timer = rospy.Timer(rospy.Duration(1.0), self.timer_callback)
    def odom_callback(self, _):
        rospy.loginfo('收到里程计消息')
    def timer_callback(self, _):
        self.odom_publisher.publish(Odometry())

def main():
    rospy.init_node('queue_latch_demo')
    OdomPublisherSubscriber()
    rospy.spin()
if __name__ == '__main__':
    main()
