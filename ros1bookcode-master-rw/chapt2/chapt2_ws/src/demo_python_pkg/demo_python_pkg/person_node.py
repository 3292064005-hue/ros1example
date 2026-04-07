#!/usr/bin/env python3
import rospy

class PersonNode:
    def __init__(self, node_name: str, name: str, age: int) -> None:
        rospy.init_node(node_name, anonymous=False)
        self.age = int(age)
        self.name = name

    def eat(self, food_name: str):
        rospy.loginfo(f'我叫{self.name},今年{self.age}岁，我现在正在吃{food_name}')

def main():
    node = PersonNode('person_node', '法外狂徒张三', 18)
    node.eat('鱼香肉丝')
    rospy.spin()

if __name__ == '__main__':
    main()
