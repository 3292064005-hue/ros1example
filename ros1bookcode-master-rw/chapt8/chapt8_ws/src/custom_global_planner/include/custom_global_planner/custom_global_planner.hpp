#ifndef CUSTOM_GLOBAL_PLANNER__CUSTOM_GLOBAL_PLANNER_HPP_
#define CUSTOM_GLOBAL_PLANNER__CUSTOM_GLOBAL_PLANNER_HPP_

#include <nav_core/base_global_planner.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Path.h>
#include <ros/ros.h>

namespace custom_global_planner {
class CustomGlobalPlanner : public nav_core::BaseGlobalPlanner {
public:
  CustomGlobalPlanner();
  CustomGlobalPlanner(std::string name, costmap_2d::Costmap2DROS* costmap_ros);
  void initialize(std::string name, costmap_2d::Costmap2DROS* costmap_ros) override;
  bool makePlan(const geometry_msgs::PoseStamped& start,
                const geometry_msgs::PoseStamped& goal,
                std::vector<geometry_msgs::PoseStamped>& plan) override;
private:
  bool initialized_;
  ros::NodeHandle nh_;
  costmap_2d::Costmap2DROS* costmap_ros_;
  double interpolation_resolution_;
};
}
#endif
