#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if ! command -v catkin_init_workspace >/dev/null 2>&1; then
  echo "catkin_init_workspace not found. Source your ROS1 environment first, e.g.:" >&2
  echo "  source /opt/ros/noetic/setup.bash" >&2
  exit 1
fi

while IFS= read -r ws; do
  if [ -d "$ws/src" ] && [ ! -e "$ws/src/CMakeLists.txt" ]; then
    echo "[bootstrap] initializing catkin workspace: $ws"
    (cd "$ws" && catkin_init_workspace src)
  fi
done < <(find "$repo_root" -type d \( -name '*_ws' -o -name 'fishbot_ws' -o -name 'topic_ws' -o -name 'topic_practice_ws' \) | sort)

echo "[bootstrap] done"
