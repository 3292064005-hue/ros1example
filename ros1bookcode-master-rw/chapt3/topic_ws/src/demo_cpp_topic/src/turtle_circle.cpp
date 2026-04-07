#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

class TurtleCircle
{
private:
  ros::NodeHandle nh_;
  ros::Publisher pub_;
  ros::Timer timer_;

public:
  TurtleCircle() {
    pub_ = nh_.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
    timer_ = nh_.createTimer(ros::Duration(1.0), &TurtleCircle::timerCallback, this);
  }
  void timerCallback(const ros::TimerEvent&) {
    geometry_msgs::Twist msg;
    msg.linear.x = 1.0;
    msg.angular.z = 0.5;
    pub_.publish(msg);
  }
};

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "turtle_square");
  TurtleCircle node;
  ros::spin();
  return 0;
}
