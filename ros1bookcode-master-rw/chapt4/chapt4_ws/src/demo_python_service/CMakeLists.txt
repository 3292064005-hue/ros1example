cmake_minimum_required(VERSION 3.0.2)
project(demo_python_service)
find_package(catkin REQUIRED COMPONENTS rospy sensor_msgs cv_bridge chapt4_interfaces)
catkin_python_setup()
catkin_package(CATKIN_DEPENDS rospy sensor_msgs cv_bridge chapt4_interfaces)
catkin_install_python(PROGRAMS
  demo_python_service/face_detect_node.py
  demo_python_service/face_detect_client_node.py
  demo_python_service/learn_face_detect.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(DIRECTORY launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
