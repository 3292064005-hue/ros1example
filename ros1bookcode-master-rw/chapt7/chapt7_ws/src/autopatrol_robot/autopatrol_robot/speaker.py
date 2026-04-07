#!/usr/bin/env python3
import rospy
from autopatrol_interfaces.srv import SpeachText, SpeachTextResponse
import espeakng


class Speaker:
    def __init__(self):
        self.speaker = espeakng.Speaker()
        self.speaker.voice = rospy.get_param('~voice', 'zh')
        self.service = rospy.Service('speech_text', SpeachText, self.handle)

    def handle(self, request):
        rospy.loginfo('正在朗读 %s', request.text)
        self.speaker.say(request.text)
        self.speaker.wait()
        return SpeachTextResponse(result=True)


def main():
    rospy.init_node('speaker')
    Speaker()
    rospy.spin()


if __name__ == '__main__':
    main()
