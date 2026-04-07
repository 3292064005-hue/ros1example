#include "custom_global_planner/custom_global_planner.hpp"
#include <pluginlib/class_list_macros.h>
#include <cmath>

namespace custom_global_planner {
CustomGlobalPlanner::CustomGlobalPlanner() : initialized_(false), costmap_ros_(nullptr), interpolation_resolution_(0.1) {}
CustomGlobalPlanner::CustomGlobalPlanner(std::string name, costmap_2d::Costmap2DROS* costmap_ros) : CustomGlobalPlanner() { initialize(name, costmap_ros); }
void CustomGlobalPlanner::initialize(std::string name, costmap_2d::Costmap2DROS* costmap_ros) {
  if (initialized_) return;
  nh_ = ros::NodeHandle("~/" + name);
  costmap_ros_ = costmap_ros;
  nh_.param("interpolation_resolution", interpolation_resolution_, 0.1);
  initialized_ = true;
}

bool CustomGlobalPlanner::makePlan(const geometry_msgs::PoseStamped& start,
                                   const geometry_msgs::PoseStamped& goal,
                                   std::vector<geometry_msgs::PoseStamped>& plan) {
  plan.clear();
  if (!initialized_) return false;
  if (start.header.frame_id != goal.header.frame_id) {
    ROS_WARN("CustomGlobalPlanner requires start and goal in same frame.");
    return false;
  }
  const double dx = goal.pose.position.x - start.pose.position.x;
  const double dy = goal.pose.position.y - start.pose.position.y;
  const double dist = std::hypot(dx, dy);
  int steps = std::max(1, static_cast<int>(std::ceil(dist / interpolation_resolution_)));
  plan.reserve(steps + 1);
  for (int i = 0; i <= steps; ++i) {
    double t = static_cast<double>(i) / static_cast<double>(steps);
    geometry_msgs::PoseStamped p = start;
    p.header.stamp = ros::Time::now();
    p.pose.position.x = start.pose.position.x + dx * t;
    p.pose.position.y = start.pose.position.y + dy * t;
    p.pose.orientation = goal.pose.orientation;
    plan.push_back(p);
  }
  plan.back() = goal;
  return true;
}
}  // namespace custom_global_planner
PLUGINLIB_EXPORT_CLASS(custom_global_planner::CustomGlobalPlanner, nav_core::BaseGlobalPlanner)
