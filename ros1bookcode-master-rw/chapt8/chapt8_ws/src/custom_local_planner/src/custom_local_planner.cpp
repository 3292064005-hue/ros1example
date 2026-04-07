#include "custom_local_planner/custom_local_planner.hpp"
#include <pluginlib/class_list_macros.h>
#include <tf2/utils.h>
#include <cmath>

namespace custom_local_planner {
CustomLocalPlanner::CustomLocalPlanner()
  : initialized_(false), goal_reached_(false), tf_(nullptr), costmap_ros_(nullptr),
    max_linear_speed_(0.26), max_angular_speed_(1.0), lookahead_dist_(0.35) {}

void CustomLocalPlanner::initialize(std::string name, tf2_ros::Buffer* tf, costmap_2d::Costmap2DROS* costmap_ros) {
  if (initialized_) return;
  tf_ = tf;
  costmap_ros_ = costmap_ros;
  nh_ = ros::NodeHandle("~/" + name);
  nh_.param("max_linear_speed", max_linear_speed_, 0.26);
  nh_.param("max_angular_speed", max_angular_speed_, 1.0);
  nh_.param("lookahead_dist", lookahead_dist_, 0.35);
  initialized_ = true;
}

bool CustomLocalPlanner::setPlan(const std::vector<geometry_msgs::PoseStamped>& orig_global_plan) {
  global_plan_ = orig_global_plan;
  goal_reached_ = global_plan_.empty();
  return true;
}

double CustomLocalPlanner::normalizeAngle(double a) {
  while (a > M_PI) a -= 2.0 * M_PI;
  while (a < -M_PI) a += 2.0 * M_PI;
  return a;
}

geometry_msgs::PoseStamped CustomLocalPlanner::nearestTargetPose(const geometry_msgs::PoseStamped& current_pose) const {
  if (global_plan_.empty()) return current_pose;
  std::size_t best_idx = global_plan_.size() - 1;
  double best_dist = std::numeric_limits<double>::max();
  for (std::size_t i = 0; i < global_plan_.size(); ++i) {
    const auto& p = global_plan_[i];
    const double dx = p.pose.position.x - current_pose.pose.position.x;
    const double dy = p.pose.position.y - current_pose.pose.position.y;
    const double d = std::hypot(dx, dy);
    if (d < best_dist) {
      best_dist = d;
      best_idx = i;
    }
    if (d >= lookahead_dist_ && i > 0) {
      break;
    }
  }
  return global_plan_[best_idx];
}

bool CustomLocalPlanner::computeVelocityCommands(geometry_msgs::Twist& cmd_vel) {
  cmd_vel = geometry_msgs::Twist();
  if (!initialized_ || global_plan_.empty()) {
    goal_reached_ = true;
    return true;
  }

  geometry_msgs::PoseStamped current_pose;
  if (!costmap_ros_->getRobotPose(current_pose)) {
    ROS_WARN_THROTTLE(1.0, "CustomLocalPlanner failed to get robot pose");
    return false;
  }

  const auto goal = global_plan_.back();
  const double gx = goal.pose.position.x - current_pose.pose.position.x;
  const double gy = goal.pose.position.y - current_pose.pose.position.y;
  const double goal_dist = std::hypot(gx, gy);
  const double current_yaw = tf2::getYaw(current_pose.pose.orientation);

  if (goal_dist < 0.15) {
    const double yaw_err = normalizeAngle(tf2::getYaw(goal.pose.orientation) - current_yaw);
    if (std::fabs(yaw_err) < 0.15) {
      goal_reached_ = true;
      return true;
    }
    cmd_vel.angular.z = std::max(std::min(yaw_err, max_angular_speed_), -max_angular_speed_);
    return true;
  }

  const auto target = nearestTargetPose(current_pose);
  const double target_heading = std::atan2(target.pose.position.y - current_pose.pose.position.y,
                                           target.pose.position.x - current_pose.pose.position.x);
  const double heading_error = normalizeAngle(target_heading - current_yaw);

  cmd_vel.linear.x = std::min(max_linear_speed_, std::max(0.05, 0.5 * goal_dist));
  if (std::fabs(heading_error) > 0.8) {
    cmd_vel.linear.x = 0.0;
  }
  cmd_vel.angular.z = std::max(std::min(2.0 * heading_error, max_angular_speed_), -max_angular_speed_);
  goal_reached_ = false;
  return true;
}

bool CustomLocalPlanner::isGoalReached() {
  return goal_reached_;
}
}  // namespace custom_local_planner
PLUGINLIB_EXPORT_CLASS(custom_local_planner::CustomLocalPlanner, nav_core::BaseLocalPlanner)
