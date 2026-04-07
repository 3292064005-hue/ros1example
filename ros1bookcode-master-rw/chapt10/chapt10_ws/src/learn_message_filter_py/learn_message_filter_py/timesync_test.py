#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import Imu
from nav_msgs.msg import Odometry
from message_filters import Subscriber, ApproximateTimeSynchronizer

class TimeSyncTestNode:
    def __init__(self):
        self.imu_sub = Subscriber('imu', Imu)
        self.odom_sub = Subscriber('odom', Odometry)
        self.synchronizer = ApproximateTimeSynchronizer([self.imu_sub, self.odom_sub], 10, 0.01)
        self.synchronizer.registerCallback(self.result_callback)
        rospy.Subscriber('imu', Imu, self.imu_callback, queue_size=10)
        rospy.Subscriber('odom', Odometry, self.odom_callback, queue_size=10)
    def imu_callback(self, msg):
        rospy.loginfo(f'imu({msg.header.stamp.secs},{msg.header.stamp.nsecs})')
    def odom_callback(self, msg):
        rospy.loginfo(f'odom({msg.header.stamp.secs},{msg.header.stamp.nsecs})')
    def result_callback(self, imu_msg, odom_msg):
        rospy.loginfo(f'imu({imu_msg.header.stamp.secs},{imu_msg.header.stamp.nsecs}),odom({odom_msg.header.stamp.secs},{odom_msg.header.stamp.nsecs})')

def main():
    rospy.init_node('sync_node')
    TimeSyncTestNode()
    rospy.spin()
if __name__ == '__main__':
    main()
