#include "ros/ros.h"
#include "OBS.h"
#include "std_msgs/String.h"
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/UInt8MultiArray.h>
#include <std_msgs/UInt32MultiArray.h>
#include <std_msgs/Float64MultiArray.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <geometry_msgs/Twist.h>
OBS OBS_obj;
double AOA_temp[3] = {0};
geometry_msgs::Twist Output_cmd_vel_raw;
void RealSense_obs(const std_msgs::Float64MultiArray::ConstPtr& array)
{
	double obs_temp[6] = {0};
	double *ptr_obs = obs_temp;
	for(std::vector<double>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
	{
		*ptr_obs = *it;
		ptr_obs++;
	}
	ROS_INFO("OBS: %e, %e, %e, %e",obs_temp[0],obs_temp[1],obs_temp[2],obs_temp[3]);
	memcpy(OBS_obj.OBS_Raw, obs_temp,sizeof(double)*6);
	OBS_obj.OBS_Phase();
}
void Sub_AOA_func(const std_msgs::Float64MultiArray::ConstPtr& array)
{
	double *ptr_AOA = AOA_temp;
	for(std::vector<double>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
	{
		*ptr_AOA = *it;
		ptr_AOA++;
	}
}
void Sub_cmd_vel_func(const geometry_msgs::Twist::ConstPtr& array)
{
	Output_cmd_vel_raw.linear.x = array->linear.x;
	Output_cmd_vel_raw.linear.y = array->linear.y;
	Output_cmd_vel_raw.linear.z = array->linear.z;
	Output_cmd_vel_raw.angular.x = array->angular.x;
	Output_cmd_vel_raw.angular.y = array->angular.y;
	Output_cmd_vel_raw.angular.z = array->angular.z;
} 
int main(int argc, char **argv)
{
  	ros::init(argc, argv, "OBS");
	ros::NodeHandle nh;
	ros::Subscriber Sub_OBS = nh.subscribe("obs_dir", 10, RealSense_obs);
	ros::Subscriber Sub_AOA = nh.subscribe("pub_AOA", 10, Sub_AOA_func);
    ros::Subscriber Sub_cmd_vel = nh.subscribe("cmd_vel_raw", 10, Sub_cmd_vel_func);
	//Car move control
	ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
	ros::Rate loop_rate(100);
	while (ros::ok())
	{
		/*
		OBS_obj.Calc_OBS();
		if(int(OBS_obj.OBS_v_w_Status["Status"]!=3) && int(AOA_temp[0]) == 2)
		{
			ROS_INFO("Status: %e",OBS_obj.OBS_v_w_Status["Status"]);
			Output_cmd_vel_raw.linear.x = OBS_obj.OBS_v_w_Status["v"];
			Output_cmd_vel_raw.angular.z = OBS_obj.OBS_v_w_Status["w"];
		}
		*/
		pub.publish(Output_cmd_vel_raw);
		ros::spinOnce();
		loop_rate.sleep();
  }
  return 0;
}
