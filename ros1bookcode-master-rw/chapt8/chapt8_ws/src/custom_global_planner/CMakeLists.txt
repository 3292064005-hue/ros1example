cmake_minimum_required(VERSION 3.0.2)
project(custom_global_planner)
find_package(catkin REQUIRED COMPONENTS roscpp nav_core costmap_2d pluginlib geometry_msgs nav_msgs)
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES custom_global_planner
  CATKIN_DEPENDS roscpp nav_core costmap_2d pluginlib geometry_msgs nav_msgs
)
include_directories(include ${catkin_INCLUDE_DIRS})
add_library(custom_global_planner src/custom_global_planner.cpp)
add_dependencies(custom_global_planner ${catkin_EXPORTED_TARGETS})
target_link_libraries(custom_global_planner ${catkin_LIBRARIES})
install(TARGETS custom_global_planner LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION})
install(DIRECTORY include/${PROJECT_NAME}/ DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION})
install(FILES custom_global_planner.xml DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
