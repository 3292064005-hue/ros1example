#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2_ros/transform_broadcaster.h>

class OdomToTF {
public:
  OdomToTF() {
    sub_ = nh_.subscribe("odom", 10, &OdomToTF::callback, this);
  }
  void callback(const nav_msgs::Odometry::ConstPtr& msg) {
    geometry_msgs::TransformStamped tf_msg;
    tf_msg.header = msg->header;
    tf_msg.child_frame_id = msg->child_frame_id.empty() ? "base_footprint" : msg->child_frame_id;
    tf_msg.transform.translation.x = msg->pose.pose.position.x;
    tf_msg.transform.translation.y = msg->pose.pose.position.y;
    tf_msg.transform.translation.z = msg->pose.pose.position.z;
    tf_msg.transform.rotation = msg->pose.pose.orientation;
    broadcaster_.sendTransform(tf_msg);
  }
private:
  ros::NodeHandle nh_;
  ros::Subscriber sub_;
  tf2_ros::TransformBroadcaster broadcaster_;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "odom2tf");
  OdomToTF node;
  ros::spin();
  return 0;
}
