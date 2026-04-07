cmake_minimum_required(VERSION 3.0.2)
project(demo_python_tf)
find_package(catkin REQUIRED COMPONENTS rospy tf2_ros geometry_msgs)
catkin_python_setup()
catkin_package(CATKIN_DEPENDS rospy tf2_ros geometry_msgs)
catkin_install_python(PROGRAMS
  demo_python_tf/dynamic_tf_broadcaster.py
  demo_python_tf/static_tf_broadcaster.py
  demo_python_tf/tf_listener.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
