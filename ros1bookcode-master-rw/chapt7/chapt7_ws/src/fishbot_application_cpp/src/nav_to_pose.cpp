#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <tf/tf.h>

int main(int argc, char** argv) {
  ros::init(argc, argv, "nav_to_pose_cpp");
  ros::NodeHandle nh("~");
  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> client("move_base", true);
  ROS_INFO("waiting for move_base action server...");
  client.waitForServer();

  double x = 1.0, y = 0.0, yaw = 0.0;
  nh.param("x", x, x);
  nh.param("y", y, y);
  nh.param("yaw", yaw, yaw);

  move_base_msgs::MoveBaseGoal goal;
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  goal.target_pose.pose.position.x = x;
  goal.target_pose.pose.position.y = y;
  geometry_msgs::Quaternion q = tf::createQuaternionMsgFromYaw(yaw);
  goal.target_pose.pose.orientation = q;
  client.sendGoal(goal);
  client.waitForResult();

  if (client.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    ROS_INFO("navigation succeeded");
    return 0;
  }
  ROS_WARN_STREAM("navigation failed: " << client.getState().toString());
  return 1;
}
