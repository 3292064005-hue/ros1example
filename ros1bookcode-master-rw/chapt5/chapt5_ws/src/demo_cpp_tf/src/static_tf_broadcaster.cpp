#include <ros/ros.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <tf2_ros/static_transform_broadcaster.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "static_tf_broadcaster");
  static tf2_ros::StaticTransformBroadcaster broadcaster;
  geometry_msgs::TransformStamped transform;
  transform.header.stamp = ros::Time::now();
  transform.header.frame_id = "base_link";
  transform.child_frame_id = "target_point";
  transform.transform.translation.x = 1.0;
  transform.transform.translation.y = 2.0;
  transform.transform.translation.z = 0.0;
  tf2::Quaternion quat; quat.setRPY(0, 0, 0);
  transform.transform.rotation = tf2::toMsg(quat);
  broadcaster.sendTransform(transform);
  ros::spin();
  return 0;
}
