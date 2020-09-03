#include "ros/ros.h"
#include "Human_follow_New.h"
#include "std_msgs/String.h"
#include <serial/serial.h>
#include <sstream>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/UInt8MultiArray.h>
#include <std_msgs/UInt32MultiArray.h>
#include <std_msgs/Float64MultiArray.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int8.h>
#include "laser_obs_msg/laser_obs_array.h"
#include "laser_obs_msg/laser_obs.h"
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <numeric>
//Avoidance const
const double Avoidance_Distance = 0.4;
//
std_msgs::UInt32MultiArray AOA_Info;
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
laser_obs_msg::laser_obs Laser_Interval_Array;
//
laser_interval_pair laser_interval_pair_object;
//min distance and angle
laser_obs_msg::laser_angle_diatance min_distance_angle;
//Time Different
double diff_time()
{
  return (Current_Frame - Pre_Frame);
}
void Laser_Interval_callback(const laser_obs_msg::laser_obs::ConstPtr& Laser_Interval)
{
	Laser_Interval_Array = *Laser_Interval;
	//ROS_INFO_STREAM(Laser_Interval_Array.laser_pair.size());
	laser_interval_pair_object.Update(Laser_Interval_Array);
	min_distance_angle = laser_interval_pair_object.Find_min_Distance_Condition();
	//ROS_INFO("distance %e, angle %e", min_distance_angle.distance, min_distance_angle.angle);
}
//
void AOA_Signal_callback(const std_msgs::Int8::ConstPtr& AOA_Switch_Signal)
{
	Current_Mode = *AOA_Switch_Signal;
}
//
void AOA_callback(const std_msgs::UInt32MultiArray::ConstPtr& AOA_Information)
{
	AOA_Info = *AOA_Information;
	//Convert to rad
	Mode = AOA_Info.data[2];
	Current_Frame = AOA_Info.data[5];
}
int main(int argc, char **argv)
{
  	ros::init(argc, argv, "Human_follow1");
	ros::NodeHandle nh;
	ros::Subscriber Laser_Interval = nh.subscribe("laser_interval_pair", 10, Laser_Interval_callback);
	ros::Subscriber AOA_node = nh.subscribe("Serial_AOA_node", 10, AOA_callback);
	ros::Subscriber AOA_Switch_Signal_node = nh.subscribe("AOA_signal_switch", 10, AOA_Signal_callback);
	//Car move control
	ros::Publisher pub_cmd_vel = nh.advertise<geometry_msgs::Twist>("fake_cmd_vel", 100);
	ros::Publisher pub_AOA = nh.advertise<std_msgs::Float64MultiArray>("pub_AOA", 100);
	ros::Rate loop_rate(100);
	//
	geometry_msgs::Twist Global_Control_Signal;
	std_msgs::Float64MultiArray AOA_pub_Output;
	// AGV Control
	AGV_Follow_Control AGV_Follow_Control_Object;
	//
	int Decrease_flag = 0;
	double v_pre, w_pre;
	unsigned int AOA_Pre_Angle = 0;
	//Remote time
	double Remote_time = 0;
    int Switch_flag = 0;
	int Timer_Count = 0;
	geometry_msgs::Twist Control_Signal;
	//Initial
	Current_Mode.data = 0;
	//
	while (ros::ok())
	{
		//Mode Remote
		if(Current_Mode.data == 2)
		{
			//Person Exist
			if(diff_time() < 1)
			{
				if(First_Timer == false)
				{
					Timer_Count = ros::Time::now().toSec();
					First_Timer = true;
				}
				else
				{
					if(ros::Time::now().toSec() - Timer_Count > 2)
					{
						AOA_Effective = false;
					}
				}
			}
			else
			{
				First_Timer = false;
				AOA_Effective = true;
			}
			//Control
			if(AOA_Effective)
			{
				//Avoidance
				if(min_distance_angle.distance > 1e-9 && min_distance_angle.distance < Avoidance_Distance)
				{
					Control_Signal = AGV_Follow_Control_Object.Dangerous_Forced_Deceleration();
					ROS_INFO_STREAM("Dangerous....");
				}
				else
				{
					Control_Signal = AGV_Follow_Control_Object.Control_Calc(AOA_Info);
					Pre_Frame = Current_Frame;
				}
				//ROS_INFO_STREAM("Speed Increase");
			}
			else
			{
				Control_Signal = AGV_Follow_Control_Object.Forced_Deceleration();
				//ROS_INFO_STREAM("Speed Descent");
			}
			Global_Control_Signal = Control_Signal;
			pub_cmd_vel.publish(Global_Control_Signal);
		}
		ros::spinOnce();
		loop_rate.sleep();
  	}
  	return 0;
}
