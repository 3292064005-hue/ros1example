# ROS1 仓库状态说明

本仓库当前状态：
- 代码主线已经统一为 **ROS1(catkin)**。
- 导航链统一为 **map_server + amcl + move_base + actionlib**。
- 历史 历史中间工件与迁移遗留项已做清理或降级。
- 目录、包名、README 与运行指令已按 ROS1 口径回写。

已完成的洁净化处理：
- 删除失效的 `*.db3` 数据目录、`shm.xml`、未使用的控制/配置占位文件。
- 导航相关目录统一为 `fishbot_navigation`、`custom_global_planner`、`custom_local_planner`。
- 教学示例目录统一为 `learn_queue_latch_*`、`learn_managed_node_*`。
- 雷达目录统一为 `ydlidar_ros`，包名保持 `ydlidar`。

建议运行基线：
```bash
catkin_make
source devel/setup.bash
```
