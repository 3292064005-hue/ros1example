#!/usr/bin/env python3
import os
import time
import cv2
import face_recognition
import rospkg
import rospy
from cv_bridge import CvBridge
from chapt4_interfaces.srv import FaceDetector, FaceDetectorResponse

class FaceDetectorionNode:
    def __init__(self):
        self.bridge = CvBridge()
        self.service = rospy.Service('/face_detect', FaceDetector, self.detect_face_callback)
        pkg_path = rospkg.RosPack().get_path('demo_python_service')
        self.default_image_path = os.path.join(pkg_path, 'resource', 'default.jpg')
        self.upsample_times = rospy.get_param('~face_locations_upsample_times', 1)
        self.model = rospy.get_param('~face_locations_model', 'hog')

    def refresh_params(self):
        self.upsample_times = rospy.get_param('~face_locations_upsample_times', self.upsample_times)
        self.model = rospy.get_param('~face_locations_model', self.model)

    def detect_face_callback(self, request):
        self.refresh_params()
        if request.image.data:
            cv_image = self.bridge.imgmsg_to_cv2(request.image, desired_encoding='bgr8')
        else:
            cv_image = cv2.imread(self.default_image_path)
        start_time = time.time()
        rospy.loginfo('加载完图像，开始检测')
        face_locations = face_recognition.face_locations(cv_image, number_of_times_to_upsample=self.upsample_times, model=self.model)
        end_time = time.time()
        rospy.loginfo(f'检测完成，耗时{end_time-start_time}')
        response = FaceDetectorResponse()
        response.number = len(face_locations)
        response.use_time = end_time - start_time
        for top, right, bottom, left in face_locations:
            response.top.append(top)
            response.right.append(right)
            response.bottom.append(bottom)
            response.left.append(left)
        return response

if __name__ == '__main__':
    rospy.init_node('face_detection_node')
    FaceDetectorionNode()
    rospy.spin()
