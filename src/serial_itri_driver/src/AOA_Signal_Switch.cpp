#include "ros/ros.h"
#include "std_msgs/String.h"
#include <serial/serial.h>
#include <sstream>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/UInt8MultiArray.h>
#include <std_msgs/UInt32MultiArray.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float64MultiArray.h>
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <numeric>
//Receive AOA Information
std_msgs::UInt32MultiArray AOA_Info;
//Current Mode
int Current_Mode = 0;
void AOA_callback(const std_msgs::UInt32MultiArray::ConstPtr& AOA_Information)
{
	AOA_Info = *AOA_Information;
	if(AOA_Info.data[2]!=3 && AOA_Info.data[2]!=Current_Mode)
	{
		Current_Mode = AOA_Info.data[2];
	}
}
int main(int argc, char **argv)
{
  	ros::init(argc, argv, "AOA_Signal_Switch");
	ros::NodeHandle nh;
	ros::Subscriber AOA_node = nh.subscribe("Serial_AOA_node", 10,AOA_callback);
	//Car move control
	ros::Publisher pub = nh.advertise<std_msgs::Int8>("AOA_signal_switch", 100);
	ros::Rate loop_rate(100);
    
	while (ros::ok())
	{
        std_msgs::Int8 Current_Mode_Package;
        Current_Mode_Package.data = Current_Mode;
        pub.publish(Current_Mode_Package);
		ros::spinOnce();
		loop_rate.sleep();
    }
  return 0;
}
