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
    rospy.init_node('nav_to_pose')
    client = actionlib.SimpleActionClient('move_base', MoveBaseAction)
    rospy.loginfo('等待 move_base...')
    client.wait_for_server()
    x = rospy.get_param('~x', 1.0)
    y = rospy.get_param('~y', 0.0)
    yaw = rospy.get_param('~yaw', 0.0)
    client.send_goal(make_goal(x, y, yaw))
    client.wait_for_result()
    state = client.get_state()
    if state == GoalStatus.SUCCEEDED:
        rospy.loginfo('导航成功')
    else:
        rospy.logwarn('导航失败, state=%s', state)


if __name__ == '__main__':
    main()
