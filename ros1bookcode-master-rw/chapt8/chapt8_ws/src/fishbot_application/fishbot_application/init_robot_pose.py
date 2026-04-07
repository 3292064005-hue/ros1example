#!/usr/bin/env python3
import rospy
import tf
from geometry_msgs.msg import PoseWithCovarianceStamped


def main():
    rospy.init_node('init_robot_pose')
    pub = rospy.Publisher('/initialpose', PoseWithCovarianceStamped, queue_size=1, latch=True)
    x = rospy.get_param('~x', 0.0)
    y = rospy.get_param('~y', 0.0)
    yaw = rospy.get_param('~yaw', 0.0)
    q = tf.transformations.quaternion_from_euler(0.0, 0.0, yaw)
    msg = PoseWithCovarianceStamped()
    msg.header.frame_id = 'map'
    msg.header.stamp = rospy.Time.now()
    msg.pose.pose.position.x = x
    msg.pose.pose.position.y = y
    msg.pose.pose.orientation.x = q[0]
    msg.pose.pose.orientation.y = q[1]
    msg.pose.pose.orientation.z = q[2]
    msg.pose.pose.orientation.w = q[3]
    pub.publish(msg)
    rospy.loginfo('已发布初始位姿 x=%.3f y=%.3f yaw=%.3f', x, y, yaw)
    rospy.sleep(1.0)
