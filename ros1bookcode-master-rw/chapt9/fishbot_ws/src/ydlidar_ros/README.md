# YDLIDAR ROS1 PACKAGE

该目录已经整理为 **ROS1(catkin)** 用法。

## 构建
```bash
cd ~/catkin_ws
catkin_make
source devel/setup.bash
```

## 运行
```bash
roslaunch ydlidar_ros ydlidar.launch
```

## 话题检查
```bash
rostopic echo /scan
```

## 说明
- 包名已统一为 `ydlidar_ros`。
- 本仓库中已删除旧版说明文字与多余驱动包装源文件。
