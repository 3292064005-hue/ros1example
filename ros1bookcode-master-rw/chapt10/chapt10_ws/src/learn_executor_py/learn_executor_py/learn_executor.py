#!/usr/bin/env python3
import rospy, threading, time
from std_msgs.msg import String
from std_srvs.srv import Trigger, TriggerResponse

class LearnExecutorNode:
    def __init__(self):
        self.publisher = rospy.Publisher('string_topic', String, queue_size=10)
        self.timer = rospy.Timer(rospy.Duration(1.0), self.timer_callback)
        self.service = rospy.Service('slow_trigger', Trigger, self.service_callback)
    def timer_callback(self, _):
        msg = String(data=f'话题发布，线程ID:{threading.get_ident()} 线程总数:{threading.active_count()}')
        rospy.loginfo(msg.data)
        self.publisher.publish(msg)
    def service_callback(self, _):
        rospy.loginfo(f'处理服务，线程ID:{threading.get_ident()}')
        time.sleep(10)
        rospy.loginfo(f'处理完成，线程ID:{threading.get_ident()}')
        return TriggerResponse(success=True, message='slow service done')

def main():
    rospy.init_node('learn_executor')
    LearnExecutorNode()
    rospy.spin()
if __name__ == '__main__':
    main()
