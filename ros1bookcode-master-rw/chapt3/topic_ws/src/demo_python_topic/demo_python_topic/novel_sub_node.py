#!/usr/bin/env python3
import threading
import time
from queue import Queue
import espeakng
import rospy
from std_msgs.msg import String

class NovelSubNode:
    def __init__(self):
        self.novels_queue_ = Queue()
        self.novel_subscriber_ = rospy.Subscriber('novel', String, self.novel_callback, queue_size=10)
        self.speech_thread_ = threading.Thread(target=self.speak_thread)
        self.speech_thread_.daemon = True
        self.speech_thread_.start()

    def novel_callback(self, msg):
        self.novels_queue_.put(msg.data)

    def speak_thread(self):
        speaker = espeakng.Speaker()
        speaker.voice = 'zh'
        while not rospy.is_shutdown():
            if self.novels_queue_.qsize() > 0:
                text = self.novels_queue_.get()
                rospy.loginfo(f'正在朗读 {text}')
                speaker.say(text)
                speaker.wait()
            else:
                time.sleep(1)

if __name__ == '__main__':
    rospy.init_node('novel_read')
    NovelSubNode()
    rospy.spin()
