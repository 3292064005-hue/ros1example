#ifndef CUSTOM_LOCAL_PLANNER__CUSTOM_LOCAL_PLANNER_HPP_
#define CUSTOM_LOCAL_PLANNER__CUSTOM_LOCAL_PLANNER_HPP_

#include <nav_core/base_local_planner.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <tf2_ros/buffer.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Path.h>
#include <ros/ros.h>
#include <limits>

namespace custom_local_planner {
class CustomLocalPlanner : public nav_core::BaseLocalPlanner {
public:
  CustomLocalPlanner();
  void initialize(std::string name, tf2_ros::Buffer* tf, costmap_2d::Costmap2DROS* costmap_ros) override;
  bool setPlan(const std::vector<geometry_msgs::PoseStamped>& orig_global_plan) override;
  bool computeVelocityCommands(geometry_msgs::Twist& cmd_vel) override;
  bool isGoalReached() override;
private:
  geometry_msgs::PoseStamped nearestTargetPose(const geometry_msgs::PoseStamped& current_pose) const;
  static double normalizeAngle(double angle);
  bool initialized_;
  bool goal_reached_;
  tf2_ros::Buffer* tf_;
  costmap_2d::Costmap2DROS* costmap_ros_;
  ros::NodeHandle nh_;
  std::vector<geometry_msgs::PoseStamped> global_plan_;
  double max_linear_speed_;
  double max_angular_speed_;
  double lookahead_dist_;
};
}
#endif
