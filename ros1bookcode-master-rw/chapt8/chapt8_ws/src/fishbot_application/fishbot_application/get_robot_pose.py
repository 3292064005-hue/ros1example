#!/usr/bin/env python3
import rospy
import tf


def main():
    rospy.init_node('get_robot_pose')
    listener = tf.TransformListener()
    rate = rospy.Rate(1.0)
    while not rospy.is_shutdown():
        try:
            listener.waitForTransform('map', 'base_footprint', rospy.Time(0), rospy.Duration(1.0))
            trans, rot = listener.lookupTransform('map', 'base_footprint', rospy.Time(0))
            yaw = tf.transformations.euler_from_quaternion(rot)[2]
            rospy.loginfo('当前位姿 x=%.3f y=%.3f yaw=%.3f', trans[0], trans[1], yaw)
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException) as exc:
            rospy.logwarn('TF 查询失败: %s', exc)
        rate.sleep()


if __name__ == '__main__':
    main()
