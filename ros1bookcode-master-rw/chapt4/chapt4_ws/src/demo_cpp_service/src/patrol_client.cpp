#include <cstdlib>
#include <ctime>
#include <ros/ros.h>
#include <ros/param.h>
#include <chapt4_interfaces/Patrol.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "patrol_client");
  ros::NodeHandle nh;
  ros::ServiceClient patrol_client = nh.serviceClient<chapt4_interfaces::Patrol>("patrol");
  ros::Rate loop(0.1);
  std::srand(std::time(nullptr));
  ros::param::set("/turtle_controller/k", 1.5);
  ROS_INFO("参数k 已修改为：1.5");
  while (ros::ok()) {
    patrol_client.waitForExistence();
    chapt4_interfaces::Patrol srv;
    srv.request.target_x = std::rand() % 15;
    srv.request.target_y = std::rand() % 15;
    ROS_INFO("请求巡逻：(%.2f, %.2f)", srv.request.target_x, srv.request.target_y);
    if (patrol_client.call(srv)) {
      if (srv.response.result == chapt4_interfaces::PatrolResponse::SUCCESS) {
        ROS_INFO("目标点处理成功");
      } else {
        ROS_WARN("目标点处理失败");
      }
    } else {
      ROS_ERROR("调用 patrol 服务失败");
    }
    loop.sleep();
  }
  return 0;
}
