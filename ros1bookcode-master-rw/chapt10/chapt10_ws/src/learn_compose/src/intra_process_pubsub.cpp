#include "learn_compose/talker.hpp"
#include "learn_compose/listener.hpp"
#include <ros/ros.h>
int main(int argc, char** argv) {
  ros::init(argc, argv, "intra_process_pubsub");
  ros::NodeHandle nh;
  learn_compose::Talker talker(nh);
  learn_compose::Listener listener(nh);
  ros::spin();
  return 0;
}
