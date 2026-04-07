#include <ros/ros.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/transform_broadcaster.h>

class DynamicTFBroadcaster
{
public:
  DynamicTFBroadcaster() {
    timer_ = nh_.createTimer(ros::Duration(0.01), &DynamicTFBroadcaster::publishTransform, this);
  }

  void publishTransform(const ros::TimerEvent&) {
    geometry_msgs::TransformStamped transform;
    transform.header.stamp = ros::Time::now();
    transform.header.frame_id = "map";
    transform.child_frame_id = "base_link";
    transform.transform.translation.x = 2.0;
    transform.transform.translation.y = 3.0;
    transform.transform.translation.z = 0.0;
    tf2::Quaternion quat;
    quat.setRPY(0, 0, 30 * M_PI / 180.0);
    transform.transform.rotation = tf2::toMsg(quat);
    tf_broadcaster_.sendTransform(transform);
  }

private:
  ros::NodeHandle nh_;
  tf2_ros::TransformBroadcaster tf_broadcaster_;
  ros::Timer timer_;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "dynamic_tf_broadcaster");
  DynamicTFBroadcaster node;
  ros::spin();
  return 0;
}
