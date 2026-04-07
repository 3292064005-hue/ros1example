# CATKIN Compile Audit

This repository is now cleaned for ROS1 static build review, with the remaining constraints stated explicitly.

## Fixed in this pass

### P0 build / launch chain fixes
- Added missing `tf` dependency to `chapt5/chapt5_ws/src/demo_python_tf`.
- Added missing runtime dependencies to all `fishbot_description` packages:
  - `xacro`
  - `robot_state_publisher`
  - `joint_state_publisher`
  - `rviz`
  - `gazebo_ros` (chapters 6/7/8 only)
- Added missing runtime dependencies to all `fishbot_navigation` packages:
  - `map_server`
  - `amcl`
  - `move_base`
- Added chapter 8 runtime dependencies from navigation config to:
  - `custom_global_planner`
  - `custom_local_planner`
- Added missing runtime dependencies to `chapt9/fishbot_ws/src/fishbot_bringup`:
  - `geometry_msgs`
  - `fishbot_description`
  - `ros_serial2wifi`
  - `ydlidar_ros`
  - `robot_state_publisher`
  - `joint_state_publisher`
- Fixed `fishbot_bringup/launch/bringup.launch` to include `ydlidar_ros` instead of the stale package name.

### P0 package / launch consistency fix
- Unified the YDLidar package identity:
  - package name: `ydlidar_ros`
  - launch package refs: `ydlidar_ros`
  - docs updated accordingly

### P0 CMake source-selection fix
- Reworked `chapt9/fishbot_ws/src/ydlidar_ros/CMakeLists.txt` so Linux builds do **not** glob in Windows serial sources.
- Added the Unix serial sources explicitly, including `lock.c`, which is safer than recursive SDK globbing for ROS1/Linux builds.

### P1 generated-message compile ordering
- Added `add_dependencies(sys_status_display ${catkin_EXPORTED_TARGETS})` to `status_display` so builds depending on generated messages do not race.

## Important repository-level constraint

This repository contains **multiple independent catkin workspaces by chapter**.
It is **not** a single flat catkin workspace.

There are intentionally duplicated package names across chapters, for example:
- `fishbot_description`
- `fishbot_navigation`
- `autopatrol_interfaces`
- `autopatrol_robot`
- `fishbot_application`
- `fishbot_application_cpp`

You should compile each chapter workspace separately, not merge every package into one `src/`.

## Workspace bootstrap note

The chapter workspaces are source-only snapshots and may not yet contain the generated `src/CMakeLists.txt` file created by `catkin_init_workspace`.

Use the helper script at the repository root before the first `catkin_make`:
- `bootstrap_catkin_workspaces.sh`

That script will run `catkin_init_workspace src` in each detected workspace if needed.

## What has **not** been verified here

This audit environment does **not** contain a ROS1/Noetic installation, so the following could not be executed here:
- `catkin_make`
- `rospack plugins --attrib=plugin nav_core`
- runtime plugin loading in `move_base`
- Gazebo launch integration tests
- YDLidar device runtime tests

So this pass is a **static compile-chain review and cleanup**, not a real Noetic build log.
