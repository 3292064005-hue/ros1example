#include <ros/ros.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "cpp_node");
    ros::NodeHandle nh;
    ROS_INFO("你好 C++ 节点！");
    ros::spin();
    return 0;
}
