#ifndef LEARN_COMPOSE__LISTENER_COMPONENT_HPP_
#define LEARN_COMPOSE__LISTENER_COMPONENT_HPP_
#include <ros/ros.h>
#include <std_msgs/Int32.h>
namespace learn_compose {
class Listener {
public:
  explicit Listener(ros::NodeHandle& nh);
private:
  void callback(const std_msgs::Int32::ConstPtr& msg);
  ros::Subscriber sub_;
};
}
#endif
