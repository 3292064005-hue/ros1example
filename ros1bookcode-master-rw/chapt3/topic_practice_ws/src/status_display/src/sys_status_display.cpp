#include <QApplication>
#include <QLabel>
#include <QString>
#include <sstream>
#include <thread>
#include <ros/ros.h>
#include <status_interfaces/SystemStatus.h>

class SysStatusDisplay {
 public:
  SysStatusDisplay(ros::NodeHandle &nh) {
    sub_ = nh.subscribe("sys_status", 10, &SysStatusDisplay::callback, this);
    label_ = new QLabel(toQString(status_interfaces::SystemStatus()));
    label_->show();
  }

  QString toQString(const status_interfaces::SystemStatus &msg) {
    std::stringstream show_str;
    show_str
        << "===========系统状态可视化显示工具============\n"
        << "数 据 时 间:	" << msg.stamp.sec << "	s\n"
        << "用  户  名:	" << msg.host_name << "	\n"
        << "CPU使用率:	" << msg.cpu_percent << "	%\n"
        << "内存使用率:	" << msg.memory_percent << "	%\n"
        << "内存总大小:	" << msg.memory_total << "	MB\n"
        << "剩余有效内存:	" << msg.memory_available << "	MB\n"
        << "网络发送量:	" << msg.net_sent << "	MB\n"
        << "网络接收量:	" << msg.net_recv << "	MB\n"
        << "==========================================";
    return QString::fromStdString(show_str.str());
  }

  void callback(const status_interfaces::SystemStatus::ConstPtr &msg) {
    label_->setText(toQString(*msg));
  }

 private:
  ros::Subscriber sub_;
  QLabel* label_;
};

int main(int argc, char* argv[]) {
  ros::init(argc, argv, "sys_status_display");
  QApplication app(argc, argv);
  ros::NodeHandle nh;
  SysStatusDisplay node(nh);
  std::thread spin_thread([]() { ros::spin(); });
  spin_thread.detach();
  return app.exec();
}
