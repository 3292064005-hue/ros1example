#include "learn_compose/listener.hpp"
namespace learn_compose {
Listener::Listener(ros::NodeHandle& nh) {
  sub_ = nh.subscribe<std_msgs::Int32>("count", 10, &Listener::callback, this);
}
void Listener::callback(const std_msgs::Int32::ConstPtr& msg) {
  ROS_INFO("收到数据:%d(0x%lX)", msg->data, reinterpret_cast<unsigned long>(msg.get()));
}
}
