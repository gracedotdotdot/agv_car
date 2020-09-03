#include <ros/ros.h>
#include <serial/serial.h>
#include "std_msgs/String.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <std_msgs/UInt32MultiArray.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/UInt8MultiArray.h>

using namespace std;
void Ultra_callback(const std_msgs::UInt32MultiArray::ConstPtr& array)
{
	// print all the remaining numbers
	for(std::vector<unsigned int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
	{
		cout << *it << " " << endl;
	}
    cout << endl;
	return;
}
int main (int argc, char** argv)
{
	ros::init(argc, argv, "serial_Ultrasonic_sub");
	ros::NodeHandle nh;
	ros::Subscriber sub_pub = nh.subscribe("serial_Ultrasonic_node", 100,Ultra_callback);
	while(ros::ok())
	{
		ros::spinOnce();
    }
}
