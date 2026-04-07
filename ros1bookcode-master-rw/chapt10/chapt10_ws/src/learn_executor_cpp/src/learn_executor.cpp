#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_srvs/Trigger.h>
#include <sstream>
#include <thread>

class LearnExecutorNode {
public:
  LearnExecutorNode() {
    pub_ = nh_.advertise<std_msgs::String>("string_topic", 10);
    timer_ = nh_.createTimer(ros::Duration(1.0), &LearnExecutorNode::timerCallback, this);
    srv_ = nh_.advertiseService("slow_trigger", &LearnExecutorNode::serviceCallback, this);
  }
private:
  std::string threadInfo() const {
    std::ostringstream ss;
    ss << "线程ID:" << std::this_thread::get_id();
    return ss.str();
  }
  void timerCallback(const ros::TimerEvent&) {
    std_msgs::String msg;
    msg.data = "话题发布:" + threadInfo();
    ROS_INFO("%s", msg.data.c_str());
    pub_.publish(msg);
  }
  bool serviceCallback(std_srvs::Trigger::Request&, std_srvs::Trigger::Response& res) {
    ROS_INFO("服务开始处理:%s", threadInfo().c_str());
    std::this_thread::sleep_for(std::chrono::seconds(10));
    res.success = true;
    res.message = "slow service done";
    ROS_INFO("服务处理完成:%s", threadInfo().c_str());
    return true;
  }
  ros::NodeHandle nh_;
  ros::Publisher pub_;
  ros::Timer timer_;
  ros::ServiceServer srv_;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "learn_executor");
  LearnExecutorNode node;
  ros::AsyncSpinner spinner(2);
  spinner.start();
  ros::waitForShutdown();
  return 0;
}
