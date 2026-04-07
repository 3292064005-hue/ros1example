#!/usr/bin/env python3
import os
import cv2
import rospkg
import rospy
from cv_bridge import CvBridge
from chapt4_interfaces.srv import FaceDetector, FaceDetectorRequest

class FaceDetectorClient:
    def __init__(self):
        self.bridge = CvBridge()
        self.client = rospy.ServiceProxy('/face_detect', FaceDetector)
        pkg_path = rospkg.RosPack().get_path('demo_python_service')
        self.test1_image_path = os.path.join(pkg_path, 'resource', 'test1.jpg')
        self.image = cv2.imread(self.test1_image_path)

    def send_request(self):
        rospy.wait_for_service('/face_detect')
        request = FaceDetectorRequest()
        request.image = self.bridge.cv2_to_imgmsg(self.image, encoding='bgr8')
        response = self.client(request)
        rospy.loginfo(f'接收到响应: 图像中共有：{response.number}张脸，耗时{response.use_time}')

    def update_detect_model(self, model):
        rospy.set_param('/face_detection_node/face_locations_model', model)
        rospy.loginfo(f'参数 face_locations_model 设置为 {model}')

if __name__ == '__main__':
    rospy.init_node('face_detect_client')
    client = FaceDetectorClient()
    client.update_detect_model('hog')
    client.send_request()
    client.update_detect_model('cnn')
    client.send_request()
