#!/usr/bin/env python3
from queue import Queue
import requests
import rospy
from std_msgs.msg import String

class NovelPubNode:
    def __init__(self):
        self.novels_queue_ = Queue()
        self.novel_publisher_ = rospy.Publisher('novel', String, queue_size=10)
        self.timer_ = rospy.Timer(rospy.Duration(5.0), self.timer_callback)

    def download_novel(self, url):
        response = requests.get(url)
        response.encoding = 'utf-8'
        rospy.loginfo(f'下载完成：{url}')
        for line in response.text.splitlines():
            self.novels_queue_.put(line)

    def timer_callback(self, _event):
        if self.novels_queue_.qsize() > 0:
            msg = String(data=self.novels_queue_.get())
            self.novel_publisher_.publish(msg)
            rospy.loginfo(f'发布了一行小说：{msg.data}')

if __name__ == '__main__':
    rospy.init_node('novel_pub')
    node = NovelPubNode()
    node.download_novel('http://localhost:8000/novel1.txt')
    rospy.spin()
