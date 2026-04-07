#!/usr/bin/env python3
import rospy
import tf2_ros
from tf.transformations import euler_from_quaternion

class TFListener:
    def __init__(self):
        self.buffer_ = tf2_ros.Buffer()
        self.listener_ = tf2_ros.TransformListener(self.buffer_)
        self.timer_ = rospy.Timer(rospy.Duration(1.0), self.get_transform)

    def get_transform(self, _event):
        try:
            result = self.buffer_.lookup_transform('base_link', 'bottle_link', rospy.Time(0), rospy.Duration(1.0))
            transform = result.transform
            rotation_euler = euler_from_quaternion([
                transform.rotation.x,
                transform.rotation.y,
                transform.rotation.z,
                transform.rotation.w,
            ])
            rospy.loginfo(f'平移:{transform.translation},旋转四元数:{transform.rotation}:旋转欧拉角:{rotation_euler}')
        except Exception as e:
            rospy.logwarn(f'不能够获取坐标变换，原因: {str(e)}')

if __name__ == '__main__':
    rospy.init_node('tf2_listener')
    TFListener()
    rospy.spin()
