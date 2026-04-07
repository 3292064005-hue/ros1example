cmake_minimum_required(VERSION 3.0.2)
project(ros_serial2wifi)
find_package(catkin REQUIRED COMPONENTS rospy)
catkin_python_setup()
catkin_package(CATKIN_DEPENDS rospy)
catkin_install_python(PROGRAMS
  ros_serail2wifi/tcpserver.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
