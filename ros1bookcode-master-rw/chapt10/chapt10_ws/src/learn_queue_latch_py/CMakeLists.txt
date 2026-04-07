cmake_minimum_required(VERSION 3.0.2)
project(learn_queue_latch_py)
find_package(catkin REQUIRED COMPONENTS rospy nav_msgs)
catkin_python_setup()
catkin_package(CATKIN_DEPENDS rospy nav_msgs)
catkin_install_python(PROGRAMS
  learn_queue_latch_py/queue_latch_demo.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
