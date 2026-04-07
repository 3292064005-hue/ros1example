#!/usr/bin/env python3
import math
import rospy
import tf2_ros
from geometry_msgs.msg import TransformStamped
from tf.transformations import quaternion_from_euler

class DynamicTFBroadcaster:
    def __init__(self):
        self.br = tf2_ros.TransformBroadcaster()
        self.timer = rospy.Timer(rospy.Duration(0.01), self.publish_transform)

    def publish_transform(self, _event):
        transform = TransformStamped()
        transform.header.stamp = rospy.Time.now()
        transform.header.frame_id = 'map'
        transform.child_frame_id = 'base_link'
        transform.transform.translation.x = 2.0
        transform.transform.translation.y = 3.0
        transform.transform.translation.z = 0.0
        q = quaternion_from_euler(0, 0, math.radians(30))
        transform.transform.rotation.x = q[0]
        transform.transform.rotation.y = q[1]
        transform.transform.rotation.z = q[2]
        transform.transform.rotation.w = q[3]
        self.br.sendTransform(transform)

if __name__ == '__main__':
    rospy.init_node('dynamic_tf_broadcaster')
    DynamicTFBroadcaster()
    rospy.spin()
