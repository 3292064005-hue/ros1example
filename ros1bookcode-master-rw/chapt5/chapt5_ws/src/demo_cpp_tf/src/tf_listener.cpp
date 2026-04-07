#include <ros/ros.h>
#include <tf2/utils.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>

class TFListener {
 public:
  TFListener() : buffer_(), listener_(buffer_) {
    timer_ = nh_.createTimer(ros::Duration(5.0), &TFListener::getTransform, this);
  }

  void getTransform(const ros::TimerEvent&) {
    try {
      const auto transform = buffer_.lookupTransform("base_link", "target_point", ros::Time(0), ros::Duration(1.0));
      const auto &translation = transform.transform.translation;
      const auto &rotation = transform.transform.rotation;
      double yaw, pitch, roll;
      tf2::Quaternion q(rotation.x, rotation.y, rotation.z, rotation.w);
      tf2::Matrix3x3(q).getEulerYPR(yaw, pitch, roll);
      ROS_INFO("平移分量: (%f, %f, %f)", translation.x, translation.y, translation.z);
      ROS_INFO("旋转分量: (%f, %f, %f)", roll, pitch, yaw);
    } catch (tf2::TransformException &ex) {
      ROS_WARN("异常: %s", ex.what());
    }
  }

 private:
  ros::NodeHandle nh_;
  tf2_ros::Buffer buffer_;
  tf2_ros::TransformListener listener_;
  ros::Timer timer_;
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "tf_listener");
  TFListener node;
  ros::spin();
  return 0;
}
