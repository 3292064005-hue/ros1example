#!/usr/bin/env python3
import math
import os
import rospy
import actionlib
import tf
from sensor_msgs.msg import Image
from geometry_msgs.msg import PoseStamped, PoseWithCovarianceStamped, Quaternion
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from actionlib_msgs.msg import GoalStatus
from autopatrol_interfaces.srv import SpeachText
from cv_bridge import CvBridge
import cv2


def quaternion_from_yaw(yaw):
    q = tf.transformations.quaternion_from_euler(0.0, 0.0, yaw)
    return Quaternion(x=q[0], y=q[1], z=q[2], w=q[3])


class PatrolNode:
    def __init__(self):
        self.initial_point = rospy.get_param('~initial_point', [0.0, 0.0, 0.0])
        self.target_points = rospy.get_param('~target_points', [0.0, 0.0, 0.0])
        self.image_save_path = rospy.get_param('~image_save_path', '')
        if self.image_save_path and not os.path.exists(self.image_save_path):
            os.makedirs(self.image_save_path, exist_ok=True)
        self.bridge = CvBridge()
        self.latest_image = None
        self.tf_listener = tf.TransformListener()
        self.client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
        rospy.loginfo('等待 move_base action server...')
        self.client.wait_for_server()
        rospy.wait_for_service('speech_text')
        self.speech = rospy.ServiceProxy('speech_text', SpeachText)
        self.sub = rospy.Subscriber('/camera_sensor/image_raw', Image, self.image_callback, queue_size=1)

    def image_callback(self, msg):
        self.latest_image = msg

    def speak_text(self, text):
        try:
            resp = self.speech(text)
            if resp.result:
                rospy.loginfo('语音播报成功: %s', text)
            else:
                rospy.logwarn('语音播报失败: %s', text)
        except rospy.ServiceException as exc:
            rospy.logwarn('语音播报请求失败: %s', exc)

    def get_pose_by_xyyaw(self, x, y, yaw):
        goal = MoveBaseGoal()
        goal.target_pose.header.frame_id = 'map'
        goal.target_pose.header.stamp = rospy.Time.now()
        goal.target_pose.pose.position.x = x
        goal.target_pose.pose.position.y = y
        goal.target_pose.pose.orientation = quaternion_from_yaw(yaw)
        return goal

    def init_robot_pose(self):
        pub = rospy.Publisher('/initialpose', PoseWithCovarianceStamped, queue_size=1, latch=True)
        rospy.sleep(0.5)
        msg = PoseWithCovarianceStamped()
        msg.header.frame_id = 'map'
        msg.header.stamp = rospy.Time.now()
        msg.pose.pose.position.x = self.initial_point[0]
        msg.pose.pose.position.y = self.initial_point[1]
        msg.pose.pose.orientation = quaternion_from_yaw(self.initial_point[2])
        pub.publish(msg)
        rospy.loginfo('已发布初始化位姿到 /initialpose')
        rospy.sleep(1.0)

    def get_target_points(self):
        pts = rospy.get_param('~target_points', self.target_points)
        result = []
        for i in range(int(len(pts) / 3)):
            x, y, yaw = pts[i * 3], pts[i * 3 + 1], pts[i * 3 + 2]
            rospy.loginfo('目标点 %d -> (%.2f, %.2f, %.2f)', i, x, y, yaw)
            result.append((x, y, yaw))
        return result

    def nav_to_pose(self, goal):
        self.client.send_goal(goal)
        while not rospy.is_shutdown():
            state = self.client.get_state()
            if state in (GoalStatus.PENDING, GoalStatus.ACTIVE):
                rospy.sleep(0.5)
                continue
            if state == GoalStatus.SUCCEEDED:
                rospy.loginfo('导航成功')
            else:
                rospy.logwarn('导航失败，状态码=%s', state)
            return state == GoalStatus.SUCCEEDED
        return False

    def get_current_pose(self):
        self.tf_listener.waitForTransform('map', 'base_footprint', rospy.Time(0), rospy.Duration(1.0))
        trans, rot = self.tf_listener.lookupTransform('map', 'base_footprint', rospy.Time(0))
        yaw = tf.transformations.euler_from_quaternion(rot)[2]
        return trans, yaw

    def record_image(self):
        if self.latest_image is None:
            rospy.logwarn('当前还没有收到图像，跳过保存')
            return
        try:
            (trans, yaw) = self.get_current_pose()
            cv_image = self.bridge.imgmsg_to_cv2(self.latest_image, desired_encoding='bgr8')
            if not self.image_save_path:
                rospy.logwarn('未配置 image_save_path，跳过保存')
                return
            filename = os.path.join(self.image_save_path, f'image_{trans[0]:.2f}_{trans[1]:.2f}_{yaw:.2f}.png')
            cv2.imwrite(filename, cv_image)
            rospy.loginfo('图像已保存: %s', filename)
        except Exception as exc:
            rospy.logwarn('图像保存失败: %s', exc)


def main():
    rospy.init_node('patrol_node')
    patrol = PatrolNode()
    patrol.speak_text('正在初始化位置')
    patrol.init_robot_pose()
    patrol.speak_text('位置初始化完成')
    rate = rospy.Rate(0.02)
    while not rospy.is_shutdown():
        for x, y, yaw in patrol.get_target_points():
            patrol.speak_text(f'准备前往目标点{x:.1f},{y:.1f}')
            ok = patrol.nav_to_pose(patrol.get_pose_by_xyyaw(x, y, yaw))
            if ok:
                patrol.speak_text(f'已到达目标点{x:.1f},{y:.1f},准备记录图像')
                patrol.record_image()
                patrol.speak_text('图像记录完成')
            else:
                patrol.speak_text(f'目标点{x:.1f},{y:.1f}导航失败')
        rate.sleep()


if __name__ == '__main__':
    main()
