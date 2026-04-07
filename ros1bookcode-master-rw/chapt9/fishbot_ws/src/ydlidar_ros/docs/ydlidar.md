# YDLIDAR ROS1 使用说明

## 1. 创建 catkin 工作空间
```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
```

## 2. 放入功能包并编译
```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

## 3. 初始化串口权限
```bash
chmod 0777 src/ydlidar_ros/startup/*
sudo sh src/ydlidar_ros/startup/initenv.sh
```

## 4. 运行激光雷达节点
```bash
roslaunch ydlidar_ros ydlidar.launch
```

## 5. 验证数据
```bash
rostopic echo /scan
```
