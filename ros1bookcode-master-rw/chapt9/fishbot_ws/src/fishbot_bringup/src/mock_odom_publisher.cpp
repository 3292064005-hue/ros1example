#include <cmath>

#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <ros/ros.h>

class MockOdomPublisher {
public:
  MockOdomPublisher() : private_nh_("~") {
    private_nh_.param("rate", rate_, 30.0);
    private_nh_.param<std::string>("odom_frame", odom_frame_, "odom");
    private_nh_.param<std::string>("base_frame", base_frame_, "base_footprint");

    if (rate_ <= 0.0) {
      ROS_WARN("~rate must be positive, falling back to 30.0 Hz");
      rate_ = 30.0;
    }

    cmd_sub_ = nh_.subscribe("/cmd_vel", 10, &MockOdomPublisher::cmdCallback, this);
    odom_pub_ = nh_.advertise<nav_msgs::Odometry>("/odom", 10);
    last_time_ = ros::Time::now();
    timer_ = nh_.createTimer(ros::Duration(1.0 / rate_),
                             &MockOdomPublisher::timerCallback, this);
  }

private:
  void cmdCallback(const geometry_msgs::Twist::ConstPtr& msg) {
    linear_x_ = msg->linear.x;
    angular_z_ = msg->angular.z;
  }

  void timerCallback(const ros::TimerEvent&) {
    const ros::Time now = ros::Time::now();
    const double dt = (now - last_time_).toSec();
    last_time_ = now;

    if (dt > 0.0) {
      x_ += linear_x_ * std::cos(yaw_) * dt;
      y_ += linear_x_ * std::sin(yaw_) * dt;
      yaw_ += angular_z_ * dt;
    }

    nav_msgs::Odometry odom;
    odom.header.stamp = now;
    odom.header.frame_id = odom_frame_;
    odom.child_frame_id = base_frame_;
    odom.pose.pose.position.x = x_;
    odom.pose.pose.position.y = y_;
    odom.pose.pose.orientation.z = std::sin(yaw_ * 0.5);
    odom.pose.pose.orientation.w = std::cos(yaw_ * 0.5);
    odom.twist.twist.linear.x = linear_x_;
    odom.twist.twist.angular.z = angular_z_;
    odom_pub_.publish(odom);
  }

  ros::NodeHandle nh_;
  ros::NodeHandle private_nh_;
  ros::Subscriber cmd_sub_;
  ros::Publisher odom_pub_;
  ros::Timer timer_;
  ros::Time last_time_;

  std::string odom_frame_;
  std::string base_frame_;
  double rate_{30.0};
  double linear_x_{0.0};
  double angular_z_{0.0};
  double x_{0.0};
  double y_{0.0};
  double yaw_{0.0};
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "mock_odom_publisher");
  MockOdomPublisher node;
  ros::spin();
  return 0;
}
