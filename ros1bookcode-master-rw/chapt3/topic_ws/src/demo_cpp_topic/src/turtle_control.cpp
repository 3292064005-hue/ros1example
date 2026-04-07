#include <cmath>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>

class TurtleController
{
public:
    TurtleController()
    {
        velocity_publisher_ = nh_.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 10);
        pose_subscription_ = nh_.subscribe("/turtle1/pose", 10, &TurtleController::onPoseReceived, this);
    }

private:
    void onPoseReceived(const turtlesim::Pose::ConstPtr &pose) {
      geometry_msgs::Twist message;
      double current_x = pose->x;
      double current_y = pose->y;
      ROS_INFO("当前位置:(x=%f,y=%f)", current_x, current_y);
      double distance = std::sqrt((target_x_ - current_x) * (target_x_ - current_x) +
                                  (target_y_ - current_y) * (target_y_ - current_y));
      double angle = std::atan2(target_y_ - current_y, target_x_ - current_x) - pose->theta;
      if (distance > 0.1) {
        if (std::fabs(angle) > 0.2) {
          message.angular.z = std::fabs(angle);
        } else {
          message.linear.x = k_ * distance;
        }
      }
      if (message.linear.x > max_speed_) {
        message.linear.x = max_speed_;
      }
      velocity_publisher_.publish(message);
    }

    ros::NodeHandle nh_;
    ros::Subscriber pose_subscription_;
    ros::Publisher velocity_publisher_;
    double target_x_{1.0};
    double target_y_{1.0};
    double k_{1.0};
    double max_speed_{3.0};
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "turtle_controller");
    TurtleController node;
    ros::spin();
    return 0;
}
