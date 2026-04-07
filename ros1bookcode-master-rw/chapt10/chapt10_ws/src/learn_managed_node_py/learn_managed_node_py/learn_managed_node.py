#!/usr/bin/env python3
import rospy
from std_srvs.srv import Trigger, TriggerResponse

class LearnManagedNode:
    def __init__(self):
        self.timer = None
        self.timer_period = 1.0
        rospy.Service('configure', Trigger, self.on_configure)
        rospy.Service('activate', Trigger, self.on_activate)
        rospy.Service('deactivate', Trigger, self.on_deactivate)
        rospy.Service('shutdown_managed_node', Trigger, self.on_shutdown)
        rospy.loginfo('managed_node:已创建')
    def timer_callback(self, _):
        rospy.loginfo('定时器打印进行中...')
    def on_configure(self, _):
        self.timer_period = 1.0
        rospy.loginfo('on_configure():配置周期 timer_period')
        return TriggerResponse(success=True, message='configured')
    def on_activate(self, _):
        if self.timer is None:
            self.timer = rospy.Timer(rospy.Duration(self.timer_period), self.timer_callback)
        rospy.loginfo('on_activate():处理激活指令，创建定时器')
        return TriggerResponse(success=True, message='activated')
    def on_deactivate(self, _):
        if self.timer is not None:
            self.timer.shutdown(); self.timer = None
        rospy.loginfo('on_deactivate():处理失活指令停止定时器')
        return TriggerResponse(success=True, message='deactivated')
    def on_shutdown(self, _):
        if self.timer is not None:
            self.timer.shutdown(); self.timer = None
        rospy.loginfo('on_shutdown():处理关闭指令')
        rospy.signal_shutdown('managed node shutdown requested')
        return TriggerResponse(success=True, message='shutdown')

def main():
    rospy.init_node('managed_node')
    LearnManagedNode()
    rospy.spin()
if __name__ == '__main__':
    main()
