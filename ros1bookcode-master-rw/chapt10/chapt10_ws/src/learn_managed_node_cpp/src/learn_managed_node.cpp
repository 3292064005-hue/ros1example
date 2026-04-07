#include <ros/ros.h>
#include <std_srvs/Trigger.h>

class LearnManagedNode {
public:
  LearnManagedNode() : timer_period_(1.0), timer_active_(false) {
    configure_srv_ = nh_.advertiseService("configure", &LearnManagedNode::onConfigure, this);
    activate_srv_ = nh_.advertiseService("activate", &LearnManagedNode::onActivate, this);
    deactivate_srv_ = nh_.advertiseService("deactivate", &LearnManagedNode::onDeactivate, this);
    shutdown_srv_ = nh_.advertiseService("shutdown_managed_node", &LearnManagedNode::onShutdown, this);
    ROS_INFO("managed_node: 已创建");
  }
private:
  void timerCallback(const ros::TimerEvent&) { ROS_INFO("定时器打印进行中..."); }
  bool onConfigure(std_srvs::Trigger::Request&, std_srvs::Trigger::Response& res) {
    timer_period_ = 1.0;
    ROS_INFO("on_configure():配置周期 timer_period");
    res.success = true; res.message = "configured"; return true;
  }
  bool onActivate(std_srvs::Trigger::Request&, std_srvs::Trigger::Response& res) {
    if (!timer_active_) { timer_ = nh_.createTimer(ros::Duration(timer_period_), &LearnManagedNode::timerCallback, this); timer_active_ = true; }
    ROS_INFO("on_activate():处理激活指令，创建定时器");
    res.success = true; res.message = "activated"; return true;
  }
  bool onDeactivate(std_srvs::Trigger::Request&, std_srvs::Trigger::Response& res) {
    if (timer_active_) { timer_.stop(); timer_active_ = false; }
    ROS_INFO("on_deactivate():处理失活指令，停止定时器");
    res.success = true; res.message = "deactivated"; return true;
  }
  bool onShutdown(std_srvs::Trigger::Request&, std_srvs::Trigger::Response& res) {
    if (timer_active_) { timer_.stop(); timer_active_ = false; }
    ROS_INFO("on_shutdown():处理关闭指令");
    res.success = true; res.message = "shutdown";
    ros::shutdown();
    return true;
  }
  ros::NodeHandle nh_;
  ros::ServiceServer configure_srv_, activate_srv_, deactivate_srv_, shutdown_srv_;
  ros::Timer timer_;
  double timer_period_;
  bool timer_active_;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "managed_node");
  LearnManagedNode node;
  ros::spin();
  return 0;
}
