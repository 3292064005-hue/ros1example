#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <cmath>

#define RAD2DEG(x) ((x) * 180.0 / M_PI)

void scanCb(const sensor_msgs::LaserScan::ConstPtr& scan) {
  int count = scan->time_increment > 0.0 ? static_cast<int>(scan->scan_time / scan->time_increment) : static_cast<int>(scan->ranges.size());
  ROS_INFO("[YDLIDAR INFO]: I heard a laser scan %s[%d]", scan->header.frame_id.c_str(), count);
  ROS_INFO("[YDLIDAR INFO]: angle_range : [%f, %f]", RAD2DEG(scan->angle_min), RAD2DEG(scan->angle_max));
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "ydlidar_driver_client");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("scan", 10, scanCb);
  ros::spin();
  return 0;
}
