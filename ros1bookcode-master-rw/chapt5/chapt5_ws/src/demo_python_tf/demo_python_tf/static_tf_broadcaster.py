#!/usr/bin/env python3
import rospy
import tf2_ros
from geometry_msgs.msg import TransformStamped
from tf.transformations import quaternion_from_euler

if __name__ == '__main__':
    rospy.init_node('static_tf_broadcaster')
    br = tf2_ros.StaticTransformBroadcaster()
    transform = TransformStamped()
    transform.header.stamp = rospy.Time.now()
    transform.header.frame_id = 'base_link'
    transform.child_frame_id = 'bottle_link'
    transform.transform.translation.x = 0.3
    transform.transform.translation.y = 0.0
    transform.transform.translation.z = 0.5
    q = quaternion_from_euler(0, 0, 0)
    transform.transform.rotation.x = q[0]
    transform.transform.rotation.y = q[1]
    transform.transform.rotation.z = q[2]
    transform.transform.rotation.w = q[3]
    br.sendTransform(transform)
    rospy.spin()
