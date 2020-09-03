#include "ros/ros.h"
#include "std_msgs/String.h"
#include <serial/serial.h>
#include <sstream>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int8.h>
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <numeric>
//Sensor Value
int Mode = 0;
double AOA_Time = 0;
// Frame
double Current_Frame = 0;
double Pre_Frame = 0;
// Flag
bool First_Timer = false;
bool AOA_Effective = false;
//
std_msgs::Int8 Current_Mode;
//
void AOA_Signal_callback(const std_msgs::Int8::ConstPtr& AOA_Switch_Signal)
{
	Current_Mode = *AOA_Switch_Signal;
}
int main(int argc, char **argv)
{
  	ros::init(argc, argv, "stop_mode");
	ros::NodeHandle nh;
	ros::Subscriber AOA_Switch_Signal_node = nh.subscribe("AOA_signal_switch",10,AOA_Signal_callback);
	//Car move control
	ros::Publisher pub_cmd_vel = nh.advertise<geometry_msgs::Twist>("fake_cmd_vel", 100);
	ros::Rate loop_rate(100);

	//Initial
	Current_Mode.data = 0;
	//
	while (ros::ok())
	{
		//Mode Remote
		if(Current_Mode.data == 0)
		{
            geometry_msgs::Twist Control_Signal;
            Control_Signal.linear.x = 0;
            Control_Signal.linear.y = 0;
            Control_Signal.linear.z = 0;
            Control_Signal.angular.x = 0;
            Control_Signal.angular.y = 0;
            Control_Signal.angular.z = 0;
			pub_cmd_vel.publish(Control_Signal);
		}
		ros::spinOnce();
		loop_rate.sleep();
  	}
  	return 0;
}
