cmake_minimum_required(VERSION 3.0.2)
project(demo_python_pkg)
find_package(catkin REQUIRED COMPONENTS rospy)
catkin_python_setup()
catkin_package()
catkin_install_python(PROGRAMS
  demo_python_pkg/python_node.py
  demo_python_pkg/person_node.py
  demo_python_pkg/writer_node.py
  demo_python_pkg/learn_thread.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
