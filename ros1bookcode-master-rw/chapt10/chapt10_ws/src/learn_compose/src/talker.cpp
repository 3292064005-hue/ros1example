#include "learn_compose/talker.hpp"
namespace learn_compose {
Talker::Talker(ros::NodeHandle& nh) : count_(0) {
  pub_ = nh.advertise<std_msgs::Int32>("count", 10);
  timer_ = nh.createTimer(ros::Duration(1.0), &Talker::onTimer, this);
}
void Talker::onTimer(const ros::TimerEvent&) {
  std_msgs::Int32 msg;
  msg.data = count_++;
  ROS_INFO("发布数据:%d(0x%lX)", msg.data, reinterpret_cast<unsigned long>(&msg));
  pub_.publish(msg);
}
}
