cmake_minimum_required(VERSION 3.0.2)
project(learn_managed_node_py)
find_package(catkin REQUIRED COMPONENTS rospy std_srvs)
catkin_python_setup()
catkin_package(CATKIN_DEPENDS rospy std_srvs)
catkin_install_python(PROGRAMS
  learn_managed_node_py/learn_managed_node.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
