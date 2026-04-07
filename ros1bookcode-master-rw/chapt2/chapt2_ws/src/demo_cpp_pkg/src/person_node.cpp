#include <string>
#include <ros/ros.h>

class PersonNode
{
private:
  ros::NodeHandle nh_;
  std::string name_;
  int age_;

public:
  PersonNode(const std::string &name, int age) : name_(name), age_(age) {}

  void eat(const std::string &food_name)
  {
    ROS_INFO("我是%s，今年%d岁，我现在正在吃%s", name_.c_str(), age_, food_name.c_str());
  }
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "person_node");
  PersonNode node("法外狂徒张三", 18);
  node.eat("鱼香ROS");
  ros::spin();
  return 0;
}
