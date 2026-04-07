#ifndef LEARN_COMPOSE__TALKER_COMPONENT_HPP_
#define LEARN_COMPOSE__TALKER_COMPONENT_HPP_
#include <ros/ros.h>
#include <std_msgs/Int32.h>
namespace learn_compose {
class Talker {
public:
  explicit Talker(ros::NodeHandle& nh);
private:
  void onTimer(const ros::TimerEvent&);
  int32_t count_;
  ros::Publisher pub_;
  ros::Timer timer_;
};
}
#endif
