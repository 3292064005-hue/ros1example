cmake_minimum_required(VERSION 3.0.2)
project(demo_python_topic)
find_package(catkin REQUIRED COMPONENTS rospy std_msgs)
catkin_python_setup()
catkin_package(CATKIN_DEPENDS rospy std_msgs)
catkin_install_python(PROGRAMS
  demo_python_topic/novel_pub_node.py
  demo_python_topic/novel_sub_node.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
