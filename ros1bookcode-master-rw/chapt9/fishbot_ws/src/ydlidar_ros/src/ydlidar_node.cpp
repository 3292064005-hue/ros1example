#include "CYdLidar.h"
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/PointCloud.h>
#include <geometry_msgs/Point32.h>
#include <std_srvs/Empty.h>
#include <cmath>
#include <limits>
#include <string>

#define ROS1_VERSION_STR "ros1-migrated"

int main(int argc, char *argv[]) {
  ros::init(argc, argv, "ydlidar_node");
  ros::NodeHandle nh;
  ros::NodeHandle pnh("~");
  ROS_INFO("[YDLIDAR INFO] Current ROS Driver Version: %s", ROS1_VERSION_STR);

  CYdLidar laser;
  std::string str_optvalue;
  pnh.param<std::string>("port", str_optvalue, "/dev/ydlidar");
  laser.setlidaropt(LidarPropSerialPort, str_optvalue.c_str(), str_optvalue.size());
  pnh.param<std::string>("ignore_array", str_optvalue, std::string(""));
  laser.setlidaropt(LidarPropIgnoreArray, str_optvalue.c_str(), str_optvalue.size());
  std::string frame_id;
  pnh.param<std::string>("frame_id", frame_id, std::string("laser_frame"));

  int optval = 230400;
  pnh.param("baudrate", optval, optval); laser.setlidaropt(LidarPropSerialBaudrate, &optval, sizeof(int));
  optval = TYPE_TRIANGLE; pnh.param("lidar_type", optval, optval); laser.setlidaropt(LidarPropLidarType, &optval, sizeof(int));
  optval = YDLIDAR_TYPE_SERIAL; pnh.param("device_type", optval, optval); laser.setlidaropt(LidarPropDeviceType, &optval, sizeof(int));
  optval = 9; pnh.param("sample_rate", optval, optval); laser.setlidaropt(LidarPropSampleRate, &optval, sizeof(int));
  optval = 4; pnh.param("abnormal_check_count", optval, optval); laser.setlidaropt(LidarPropAbnormalCheckCount, &optval, sizeof(int));
  optval = 8; pnh.param("intensity_bit", optval, optval); laser.setlidaropt(LidarPropIntenstiyBit, &optval, sizeof(int));

  bool b_optvalue = false;
  pnh.param("fixed_resolution", b_optvalue, b_optvalue); laser.setlidaropt(LidarPropFixedResolution, &b_optvalue, sizeof(bool));
  b_optvalue = true; pnh.param("reversion", b_optvalue, b_optvalue); laser.setlidaropt(LidarPropReversion, &b_optvalue, sizeof(bool));
  b_optvalue = true; pnh.param("inverted", b_optvalue, b_optvalue); laser.setlidaropt(LidarPropInverted, &b_optvalue, sizeof(bool));
  b_optvalue = true; pnh.param("auto_reconnect", b_optvalue, b_optvalue); laser.setlidaropt(LidarPropAutoReconnect, &b_optvalue, sizeof(bool));
  b_optvalue = false; pnh.param("isSingleChannel", b_optvalue, b_optvalue); laser.setlidaropt(LidarPropSingleChannel, &b_optvalue, sizeof(bool));
  laser.setAutoIntensity(true);
  b_optvalue = false; pnh.param("support_motor_dtr", b_optvalue, b_optvalue); laser.setlidaropt(LidarPropSupportMotorDtrCtrl, &b_optvalue, sizeof(bool));

  float f_optvalue = 180.0f;
  pnh.param("angle_max", f_optvalue, f_optvalue); laser.setlidaropt(LidarPropMaxAngle, &f_optvalue, sizeof(float));
  f_optvalue = -180.0f; pnh.param("angle_min", f_optvalue, f_optvalue); laser.setlidaropt(LidarPropMinAngle, &f_optvalue, sizeof(float));
  f_optvalue = 64.0f; pnh.param("range_max", f_optvalue, f_optvalue); laser.setlidaropt(LidarPropMaxRange, &f_optvalue, sizeof(float));
  f_optvalue = 0.1f; pnh.param("range_min", f_optvalue, f_optvalue); laser.setlidaropt(LidarPropMinRange, &f_optvalue, sizeof(float));
  f_optvalue = 10.0f; pnh.param("frequency", f_optvalue, f_optvalue); laser.setlidaropt(LidarPropScanFrequency, &f_optvalue, sizeof(float));

  bool invalid_range_is_inf = false;
  pnh.param("invalid_range_is_inf", invalid_range_is_inf, invalid_range_is_inf);

  bool ret = laser.initialize();
  if (ret) ret = laser.turnOn();
  else ROS_ERROR("%s", laser.DescribeError());

  ros::Publisher laser_pub = nh.advertise<sensor_msgs::LaserScan>("scan", 20);
  ros::Publisher pc_pub = nh.advertise<sensor_msgs::PointCloud>("point_cloud", 20);

  ros::ServiceServer stop_service = nh.advertiseService<std_srvs::Empty::Request, std_srvs::Empty::Response>("stop_scan",
    [&](std_srvs::Empty::Request&, std_srvs::Empty::Response&) { return laser.turnOff(); });
  ros::ServiceServer start_service = nh.advertiseService<std_srvs::Empty::Request, std_srvs::Empty::Response>("start_scan",
    [&](std_srvs::Empty::Request&, std_srvs::Empty::Response&) { return laser.turnOn(); });

  ros::Rate loop_rate(20);
  while (ret && ros::ok()) {
    LaserScan scan;
    if (laser.doProcessSimple(scan)) {
      sensor_msgs::LaserScan scan_msg;
      sensor_msgs::PointCloud pc_msg;
      scan_msg.header.stamp.fromNSec(scan.stamp);
      scan_msg.header.frame_id = frame_id;
      pc_msg.header = scan_msg.header;
      scan_msg.angle_min = scan.config.min_angle;
      scan_msg.angle_max = scan.config.max_angle;
      scan_msg.angle_increment = scan.config.angle_increment;
      scan_msg.scan_time = scan.config.scan_time;
      scan_msg.time_increment = scan.config.time_increment;
      scan_msg.range_min = scan.config.min_range;
      scan_msg.range_max = scan.config.max_range;
      int size = static_cast<int>((scan.config.max_angle - scan.config.min_angle) / scan.config.angle_increment) + 1;
      scan_msg.ranges.assign(size, invalid_range_is_inf ? std::numeric_limits<float>::infinity() : 0.0f);
      scan_msg.intensities.assign(size, 0.0f);
      pc_msg.channels.resize(2);
      pc_msg.channels[0].name = "intensities";
      pc_msg.channels[1].name = "stamps";
      for (size_t i = 0; i < scan.points.size(); ++i) {
        int index = static_cast<int>(std::ceil((scan.points[i].angle - scan.config.min_angle) / scan.config.angle_increment));
        if (index >= 0 && index < size && scan.points[i].range >= scan.config.min_range) {
          scan_msg.ranges[index] = scan.points[i].range;
          scan_msg.intensities[index] = scan.points[i].intensity;
        }
        if (scan.points[i].range >= scan.config.min_range && scan.points[i].range <= scan.config.max_range) {
          geometry_msgs::Point32 p;
          p.x = scan.points[i].range * std::cos(scan.points[i].angle);
          p.y = scan.points[i].range * std::sin(scan.points[i].angle);
          p.z = 0.0;
          pc_msg.points.push_back(p);
          pc_msg.channels[0].values.push_back(scan.points[i].intensity);
          pc_msg.channels[1].values.push_back(i * scan.config.time_increment);
        }
      }
      laser_pub.publish(scan_msg);
      pc_pub.publish(pc_msg);
    } else {
      ROS_ERROR_THROTTLE(1.0, "Failed to get scan");
    }
    ros::spinOnce();
    loop_rate.sleep();
  }
  ROS_INFO("[YDLIDAR INFO] Now YDLIDAR is stopping .......");
  laser.turnOff();
  laser.disconnecting();
  return 0;
}
