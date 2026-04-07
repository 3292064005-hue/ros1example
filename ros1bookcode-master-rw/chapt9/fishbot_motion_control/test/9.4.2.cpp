#include <Arduino.h>
#include <WiFi.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <Esp32PcntEncoder.h>
#include <Esp32McpwmMotor.h>
#include <PidController.h>
#include <Kinematics.h>

namespace {
constexpr const char* kWifiSsid = "fishros";
constexpr const char* kWifiPassword = "88888888";
constexpr uint16_t kRosserialPort = 8889;
IPAddress kRosHost(192, 168, 1, 103);
}

class WiFiHardware {
 public:
  WiFiHardware() : server_ip_(kRosHost), server_port_(kRosserialPort) {}
  void setConnection(const IPAddress& ip, uint16_t port) { server_ip_ = ip; server_port_ = port; }
  void init() { ensureWifi(); ensureClient(); }
  int read() { ensureClient(); return client_.read(); }
  void write(uint8_t* data, int length) { ensureClient(); client_.write(data, length); }
  unsigned long time() { return millis(); }
 private:
  void ensureWifi() {
    if (WiFi.status() == WL_CONNECTED) return;
    WiFi.mode(WIFI_STA);
    WiFi.begin(kWifiSsid, kWifiPassword);
    while (WiFi.status() != WL_CONNECTED) delay(500);
  }
  void ensureClient() {
    ensureWifi();
    while (!client_.connected()) {
      client_.stop();
      client_.connect(server_ip_, server_port_);
      delay(500);
    }
  }
  WiFiClient client_;
  IPAddress server_ip_;
  uint16_t server_port_;
};

WiFiHardware wifi_hw;
ros::NodeHandle_<WiFiHardware> nh(wifi_hw);

Esp32PcntEncoder encoders[2];
Esp32McpwmMotor motor;
PidController pid_controller[2];
Kinematics kinematics;

float target_linear_speed = 0.0F;
float target_angular_speed = 0.0F;
float out_left_speed = 0.0F;
float out_right_speed = 0.0F;

void applyTargetSpeeds() {
  kinematics.kinematics_inverse(target_linear_speed, target_angular_speed,
                                &out_left_speed, &out_right_speed);
  pid_controller[0].update_target(out_left_speed);
  pid_controller[1].update_target(out_right_speed);
}

void twistCallback(const geometry_msgs::Twist& msg) {
  target_linear_speed = msg.linear.x * 1000.0F;
  target_angular_speed = msg.angular.z;
  applyTargetSpeeds();
}

ros::Subscriber<geometry_msgs::Twist> cmd_vel_sub("cmd_vel", &twistCallback);

void setup() {
  Serial.begin(115200);
  motor.attachMotor(0, 22, 23);
  motor.attachMotor(1, 12, 13);
  encoders[0].init(0, 32, 33);
  encoders[1].init(1, 26, 25);
  pid_controller[0].update_pid(0.625, 0.125, 0.0);
  pid_controller[1].update_pid(0.625, 0.125, 0.0);
  pid_controller[0].out_limit(-100, 100);
  pid_controller[1].out_limit(-100, 100);
  kinematics.set_wheel_distance(175);
  kinematics.set_motor_param(0, 0.105805);
  kinematics.set_motor_param(1, 0.105805);
  applyTargetSpeeds();

  wifi_hw.setConnection(kRosHost, kRosserialPort);
  nh.initNode();
  nh.subscribe(cmd_vel_sub);
}

void loop() {
  delay(10);
  kinematics.update_motor_speed(millis(), encoders[0].getTicks(), encoders[1].getTicks());
  motor.updateMotorSpeed(0, pid_controller[0].update(kinematics.get_motor_speed(0)));
  motor.updateMotorSpeed(1, pid_controller[1].update(kinematics.get_motor_speed(1)));
  nh.spinOnce();
}
