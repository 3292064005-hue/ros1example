cmake_minimum_required(VERSION 3.0.2)
project(motion_control_system)
find_package(catkin REQUIRED COMPONENTS pluginlib roscpp)
catkin_package(INCLUDE_DIRS include LIBRARIES spin_motion_controller CATKIN_DEPENDS pluginlib roscpp)
include_directories(include ${catkin_INCLUDE_DIRS})
add_library(spin_motion_controller SHARED src/spin_motion_controller.cpp)
target_link_libraries(spin_motion_controller ${catkin_LIBRARIES})
add_executable(test_plugin src/test_plugin.cpp)
target_link_libraries(test_plugin ${catkin_LIBRARIES})
install(TARGETS spin_motion_controller test_plugin
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(DIRECTORY include/ DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
install(FILES spin_motion_plugins.xml DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
