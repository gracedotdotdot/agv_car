#!/usr/bin/env python
import rospy
import numpy as np

import sensor_msgs.point_cloud2 as pc2  
from sensor_msgs.msg import PointCloud2

class PassThrough(object):
    def __init__(self):
        self.node_name = rospy.get_name()
        rospy.loginfo("[%s] Initializing " %(self.node_name))

        rospy.Subscriber("~input", PointCloud2, self.pc_callback)
        self.filter_limit_min = rospy.get_param("~filter_limit_min", 0.1)
        self.filter_limit_max = rospy.get_param("~filter_limit_max", 10.0)
        self.filter_limit_negative = rospy.get_param("~filter_limit_negative", False)
    
    def pc_callback(self, msg):
        for p in pc2.read_points(point_cloud, field_names = ("x", "y", "z"), skip_nans=True):
            if self.

if __name__ == '__main__':
    rospy.init_node('PassThrough')

    passthrough = PassThrough()
    rospy.on_shutdown(passthrough.on_shutdown)

    rospy.spin()
