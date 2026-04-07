cmake_minimum_required(VERSION 3.0.2)
project(learn_message_filter_py)
find_package(catkin REQUIRED COMPONENTS rospy sensor_msgs nav_msgs message_filters)
catkin_python_setup()
catkin_package(CATKIN_DEPENDS rospy sensor_msgs nav_msgs message_filters)
catkin_install_python(PROGRAMS
  learn_message_filter_py/timesync_test.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
