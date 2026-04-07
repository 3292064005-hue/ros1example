#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>

using Imu = sensor_msgs::Imu;
using Odometry = nav_msgs::Odometry;
using MySyncPolicy = message_filters::sync_policies::ApproximateTime<Imu, Odometry>;

class TimeSyncTestNode {
public:
  TimeSyncTestNode() : imu_sub_(nh_, "imu", 10), odom_sub_(nh_, "odom", 10) {
    raw_imu_sub_ = nh_.subscribe("imu", 10, &TimeSyncTestNode::imuCallback, this);
    raw_odom_sub_ = nh_.subscribe("odom", 10, &TimeSyncTestNode::odomCallback, this);
    synchronizer_.reset(new message_filters::Synchronizer<MySyncPolicy>(MySyncPolicy(10), imu_sub_, odom_sub_));
    synchronizer_->registerCallback(boost::bind(&TimeSyncTestNode::resultCallback, this, _1, _2));
  }
private:
  void imuCallback(const Imu::ConstPtr& imu_msg) { ROS_INFO("imu(%u,%u)", imu_msg->header.stamp.sec, imu_msg->header.stamp.nsec); }
  void odomCallback(const Odometry::ConstPtr& odom_msg) { ROS_INFO("odom(%u,%u)", odom_msg->header.stamp.sec, odom_msg->header.stamp.nsec); }
  void resultCallback(const Imu::ConstPtr& imu_msg, const Odometry::ConstPtr& odom_msg) {
    ROS_INFO("imu(%u,%u),odom(%u,%u)", imu_msg->header.stamp.sec, imu_msg->header.stamp.nsec, odom_msg->header.stamp.sec, odom_msg->header.stamp.nsec);
  }
  ros::NodeHandle nh_;
  ros::Subscriber raw_imu_sub_, raw_odom_sub_;
  message_filters::Subscriber<Imu> imu_sub_;
  message_filters::Subscriber<Odometry> odom_sub_;
  std::shared_ptr<message_filters::Synchronizer<MySyncPolicy>> synchronizer_;
};

int main(int argc, char** argv) {
  ros::init(argc, argv, "sync_node");
  TimeSyncTestNode node;
  ros::spin();
  return 0;
}
