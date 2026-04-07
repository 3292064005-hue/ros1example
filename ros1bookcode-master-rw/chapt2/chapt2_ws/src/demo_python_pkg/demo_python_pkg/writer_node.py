#!/usr/bin/env python3
import rospy
from demo_python_pkg.person_node import PersonNode

class WriterNode(PersonNode):
    def __init__(self, node_name: str, name: str, age: int, book: str) -> None:
        super().__init__(node_name, name, age)
        rospy.loginfo('WriterNode 的 __init__ 函数被调用了')
        self.book = book

def main():
    node = WriterNode('writer_node', '法外狂徒张三', 18, '论快速入狱')
    node.eat('鱼香肉丝')
    rospy.spin()

if __name__ == '__main__':
    main()
