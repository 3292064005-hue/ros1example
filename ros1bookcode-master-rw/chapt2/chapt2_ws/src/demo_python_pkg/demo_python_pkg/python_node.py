#!/usr/bin/env python3
import rospy

def main():
    rospy.init_node('python_node')
    rospy.loginfo('你好 Python 节点！')
    rospy.spin()

if __name__ == '__main__':
    main()
