#!/usr/bin/env python
import rospy
import numpy as np

from geometry_msgs.msg import Twist, Vector3

class Controller(object):
    def __init__(self):
        self.node_name = rospy.get_name()
        rospy.loginfo("[%s] Initializing " %(self.node_name))

        self.linear_v = 0.5
        self.omega = 1.0
        self.omega_v = 2.0

        self.cmd_pub = rospy.Publisher('/cmd_vel', Twist, queue_size=1)

    def listen_keyboard(self):
        key = raw_input()

        v = 0
        omega = 0

        if key == 'w':
            rospy.loginfo("Robot Motion: Going Forward")

            v = self.linear_v
            omega = 0

        elif key == 'a':
            rospy.loginfo("Robot Motion: Turning Left")

            v = self.omega_v
            omega = self.omega

        elif key == 'd':
            rospy.loginfo("Robot Motion: Turning Right")

            v = self.omega_v
            omega = -self.omega

        elif key == 's':
            rospy.loginfo("Robot Motion: Going Backward")

            v = -self.linear_v
            omega = 0

        else:
            rospy.loginfo("Robot Motion: Wrong Input")

        cmd = Twist()
        cmd.linear.x = v
        cmd.angular.z = omega

        self.cmd_pub.publish(cmd)

    def shutdown(self):
        rospy.loginfo('Controller: Shut Down')


if __name__ == '__main__':
    rospy.init_node('Controller')

    controller = Controller()
    rospy.on_shutdown(controller.shutdown)

    while not rospy.is_shutdown():
        controller.listen_keyboard()
        rospy.sleep(0.1)

    # rospy.spin()


