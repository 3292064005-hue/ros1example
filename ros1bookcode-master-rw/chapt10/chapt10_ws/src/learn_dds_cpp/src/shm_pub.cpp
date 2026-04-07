#include <ros/ros.h>
#include <std_msgs/Int32.h>

int main(int argc, char** argv) {
  ros::init(argc, argv, "loaned_message_publisher");
  ros::NodeHandle nh;
  ros::Publisher pub = nh.advertise<std_msgs::Int32>("loaned_int_topic", 10);
  ros::Rate rate(1.0);
  int32_t count = 0;
  while (ros::ok()) {
    std_msgs::Int32 msg;
    msg.data = count++;
    ROS_INFO("发布数据:%d", msg.data);
    pub.publish(msg);
    rate.sleep();
  }
  return 0;
}
