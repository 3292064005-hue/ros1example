#!/usr/bin/env python3
import rospy
import actionlib
import tf
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from actionlib_msgs.msg import GoalStatus


def make_goal(x, y, yaw):
    q = tf.transformations.quaternion_from_euler(0.0, 0.0, yaw)
    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = 'map'
    goal.target_pose.header.stamp = rospy.Time.now()
    goal.target_pose.pose.position.x = x
    goal.target_pose.pose.position.y = y
    goal.target_pose.pose.orientation.x = q[0]
    goal.target_pose.pose.orientation.y = q[1]
    goal.target_pose.pose.orientation.z = q[2]
    goal.target_pose.pose.orientation.w = q[3]
    return goal


def main():
    rospy.init_node('waypoint_follower')
    points = rospy.get_param('~waypoints', [0.0, 0.0, 0.0, 1.0, 0.0, 0.0])
    client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
    client.wait_for_server()
    for i in range(int(len(points) / 3)):
        x, y, yaw = points[i * 3], points[i * 3 + 1], points[i * 3 + 2]
        rospy.loginfo('前往航点 %d -> %.2f %.2f %.2f', i, x, y, yaw)
        client.send_goal(make_goal(x, y, yaw))
        client.wait_for_result()
        if client.get_state() != GoalStatus.SUCCEEDED:
            rospy.logwarn('航点 %d 执行失败，提前退出', i)
            return
    rospy.loginfo('所有航点执行完成')


if __name__ == '__main__':
    main()
