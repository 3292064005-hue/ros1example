cmake_minimum_required(VERSION 3.0.2)
project(autopatrol_robot)
find_package(catkin REQUIRED COMPONENTS rospy actionlib move_base_msgs sensor_msgs geometry_msgs tf cv_bridge autopatrol_interfaces)
catkin_python_setup()
catkin_package(CATKIN_DEPENDS rospy actionlib move_base_msgs sensor_msgs geometry_msgs tf cv_bridge autopatrol_interfaces)
catkin_install_python(PROGRAMS
  autopatrol_robot/patrol_node.py
  autopatrol_robot/speaker.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(DIRECTORY launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
install(DIRECTORY config DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
