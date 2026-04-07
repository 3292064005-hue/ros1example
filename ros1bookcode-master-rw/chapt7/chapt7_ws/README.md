# 基于 ROS1 move_base 的自动巡检机器人

## 1. 项目介绍

本项目已经整理为 **ROS1(catkin)** 版本。机器人在 Gazebo 中运行，使用 **move_base 导航栈** 完成巡检导航。到达目标点后，可播放语音并抓取一张图像保存到本地。

各功能包如下：
- `fishbot_description`：机器人描述与仿真配置
- `fishbot_navigation`：地图、AMCL、move_base 导航配置
- `fishbot_application`：Python 导航应用
- `fishbot_application_cpp`：C++ 导航应用
- `autopatrol_interfaces`：自动巡检接口
- `autopatrol_robot`：自动巡检主逻辑

## 2. 环境

- Ubuntu 20.04 / 22.04
- ROS1 Noetic

## 3. 依赖安装

```bash
sudo apt update
sudo apt install -y \
  ros-noetic-robot-state-publisher \
  ros-noetic-joint-state-publisher \
  ros-noetic-gazebo-ros-pkgs \
  ros-noetic-xacro \
  ros-noetic-map-server \
  ros-noetic-amcl \
  ros-noetic-move-base

sudo apt install -y python3-pip espeak-ng
pip3 install espeakng transforms3d
```

## 4. 构建

```bash
catkin_make
source devel/setup.bash
```

## 5. 运行

运行仿真：
```bash
roslaunch fishbot_description gazebo_sim.launch
```

运行导航：
```bash
roslaunch fishbot_navigation navigation.launch
```

运行自动巡检：
```bash
roslaunch autopatrol_robot autopatrol.launch
```
