#include <ros/ros.h>
#include <nav_msgs/Odometry.h>

class OdomPublisherSubscriber {
public:
  OdomPublisherSubscriber() {
    odom_publisher_ = nh_.advertise<nav_msgs::Odometry>("odom", 10, true);
    odom_subscription_ = nh_.subscribe("odom", 10, &OdomPublisherSubscriber::callback, this);
    timer_ = nh_.createTimer(ros::Duration(1.0), &OdomPublisherSubscriber::timerCallback, this);
  }
private:
  void callback(const nav_msgs::Odometry::ConstPtr&) { ROS_INFO("收到里程计消息"); }
  void timerCallback(const ros::TimerEvent&) { odom_publisher_.publish(nav_msgs::Odometry()); }
  ros::NodeHandle nh_;
  ros::Publisher odom_publisher_;
  ros::Subscriber odom_subscription_;
  ros::Timer timer_;
};

int main(int argc, char* argv[]) {
  ros::init(argc, argv, "queue_latch_demo");
  OdomPublisherSubscriber node;
  ros::spin();
  return 0;
}
