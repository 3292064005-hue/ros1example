cmake_minimum_required(VERSION 3.0.2)
project(fishbot_application)
find_package(catkin REQUIRED COMPONENTS rospy actionlib move_base_msgs geometry_msgs tf)
catkin_python_setup()
catkin_package(CATKIN_DEPENDS rospy actionlib move_base_msgs geometry_msgs tf)
catkin_install_python(PROGRAMS
  fishbot_application/get_robot_pose.py
  fishbot_application/init_robot_pose.py
  fishbot_application/nav_to_pose.py
  fishbot_application/waypoint_follower.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(DIRECTORY launch DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
