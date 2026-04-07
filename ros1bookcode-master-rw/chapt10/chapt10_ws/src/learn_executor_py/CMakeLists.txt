cmake_minimum_required(VERSION 3.0.2)
project(learn_executor_py)
find_package(catkin REQUIRED COMPONENTS rospy std_msgs std_srvs)
catkin_python_setup()
catkin_package(CATKIN_DEPENDS rospy std_msgs std_srvs)
catkin_install_python(PROGRAMS
  learn_executor_py/learn_executor.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
