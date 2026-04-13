cmake_minimum_required(VERSION 3.0.2)
project(ydlidar_ros C CXX)
set(CMAKE_CXX_STANDARD 14)

find_package(catkin REQUIRED COMPONENTS roscpp sensor_msgs visualization_msgs geometry_msgs std_srvs)

catkin_package(CATKIN_DEPENDS roscpp sensor_msgs visualization_msgs geometry_msgs std_srvs)

set(YDLIDAR_SDK_SOURCES
  sdk/src/CYdLidar.cpp
  sdk/src/DTSLidarDriver.cpp
  sdk/src/ETLidarDriver.cpp
  sdk/src/GSLidarDriver.cpp
  sdk/src/SDMLidarDriver.cpp
  sdk/src/TiaLidarDriver.cpp
  sdk/src/YDlidarDriver.cpp
  sdk/src/ydlidar_sdk.cpp
  sdk/src/filters/NoiseFilter.cpp
  sdk/src/filters/StrongLightFilter.cpp
  sdk/core/base/timer.cpp
  sdk/core/common/ydlidar_def.cpp
  sdk/core/json/cJSON.c
  sdk/core/network/ActiveSocket.cpp
  sdk/core/network/PassiveSocket.cpp
  sdk/core/network/SimpleSocket.cpp
  sdk/core/serial/serial.cpp
)

if(UNIX AND NOT APPLE)
  list(APPEND YDLIDAR_SDK_SOURCES
    sdk/core/serial/impl/unix/list_ports_linux.cpp
    sdk/core/serial/impl/unix/unix_serial.cpp
    sdk/core/serial/impl/unix/lock.c
  )
elseif(WIN32)
  list(APPEND YDLIDAR_SDK_SOURCES
    sdk/core/serial/impl/windows/list_ports_win.cpp
    sdk/core/serial/impl/windows/win_serial.cpp
  )
endif()

include_directories(
  ${catkin_INCLUDE_DIRS}
  .
  sdk
  sdk/core
  sdk/src
  sdk/core/base
  sdk/core/common
  sdk/core/math
  sdk/core/network
  sdk/core/serial
  sdk/core/serial/impl/unix
  sdk/core/serial/impl/windows
)

add_executable(ydlidar_node src/ydlidar_node.cpp ${YDLIDAR_SDK_SOURCES})
add_executable(ydlidar_client src/ydlidar_client.cpp)

foreach(t ydlidar_node ydlidar_client)
  add_dependencies(${t} ${catkin_EXPORTED_TARGETS})
  target_link_libraries(${t} ${catkin_LIBRARIES})
endforeach()

install(TARGETS ydlidar_node ydlidar_client
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

install(DIRECTORY launch params startup config DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
