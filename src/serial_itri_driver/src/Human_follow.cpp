#include "ros/ros.h"
#include "Human_follow.h"
#include "std_msgs/String.h"
#include <serial/serial.h>
#include <sstream>
#include <std_msgs/Int32MultiArray.h>
#include <std_msgs/UInt8MultiArray.h>
#include <std_msgs/UInt32MultiArray.h>
#include <std_msgs/Float64MultiArray.h>
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <numeric>
#include <geometry_msgs/Twist.h>
#include "OBS.h"
AOA_Box _AOA_Box;
//Argument Setting
//int Long_Dist = 60;
//int Short_Dist = 40;
/*************************************/ 
double v = First_v;		//cm
double w = First_w;		//rad
//Sensor Value
int AOA_Val[5]={0};
int UltraSonics[8];
UltraSonic_Process UltraSonic_Pro;
OBS OBS_obj;
int Mode_Effect_Flag = 0;
int Current_Mode = 0;
int Get_Mode()
{
	if(AOA_Val[2]!=3 && AOA_Val[2]!=Current_Mode)
	{
		Current_Mode = AOA_Val[2];
	}
	return Current_Mode;
}
void UltraSonic(const std_msgs::UInt32MultiArray::ConstPtr& array)
{
	int index = 0;
	// print all the remaining numbers
	for(std::vector<unsigned int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
	{
		UltraSonics[index] = (int)*it;
		index++;
	}
	UltraSonic_Pro.Update_Value(UltraSonics);
	//ROS_INFO("%d %d %d %d",UltraSonics[0],UltraSonics[1],UltraSonics[2],UltraSonics[3]);
	UltraSonic_Pro.Mean();
	/*
	for(int i=0;i<4;i++)
	{
		cout << UltraSonics[i] << ",";
	}
	*/
	cout << endl;
}
void RealSense_obs(const std_msgs::Float64MultiArray::ConstPtr& array)
{
	double obs_temp[6] = {0};
	double *ptr_obs = obs_temp;
	// print all the remaining numbers
	for(std::vector<double>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
	{
		*ptr_obs = *it;
		ptr_obs++;
	}
	//ROS_INFO("OBS: %e, %e, %e, %e",obs_temp[0],obs_temp[1],obs_temp[2],obs_temp[3]);
	memcpy(OBS_obj.OBS_Raw, obs_temp,sizeof(double)*6);
	OBS_obj.OBS_Phase();
	OBS_obj.Display();
	//cout << endl;
}
void AOA_callback(const std_msgs::UInt32MultiArray::ConstPtr& array)
{
	int index = 0;
	// print all the remaining numbers
	for(std::vector<unsigned int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
	{
		AOA_Val[index] = (int)*it;
		index++;
	}
	AOA_Val[0] +=10;
	if(AOA_Val[0]>180)
	{
		AOA_Val[0] -= 360;
	}
	AOA_Val[0] = -AOA_Val[0];
	//cout << (int)AOA_Val[0] << endl;
	//AOA_Val[0] = -AOA_Val[0];
	//AOA_Val[0] -= 135;
	//cout << AOA_Val[1] << ",";
	//cout << endl;
	//Calibration
	_AOA_Box.Update_Value(AOA_Val[0],AOA_Val[1]);
	//cout << AOA_Val[0] << endl;
	/*
	if(AOA_Val[2]!=4)
	{
		//cout << (int)AOA_Val[2] << "," <<(int)AOA_Val[3] << "," << (int)AOA_Val[4] << endl;
		Mode_Effect_Flag = 1;
	}
	else
	{
		Mode_Effect_Flag = 0;
	}
	*/
	//cout << _AOA_Box.AOABOX_Mean << endl;
	return;
}
int main(int argc, char **argv)
{
  	ros::init(argc, argv, "Human_follow");
	ros::NodeHandle nh;
	ros::Subscriber AOA_node = nh.subscribe("Serial_AOA_node", 10,AOA_callback);
	ros::Subscriber Ultrasonick = nh.subscribe("Serial_NewNewUltrasonic_node", 10,UltraSonic);
	ros::Subscriber obs = nh.subscribe("obs_dir", 10, RealSense_obs);
	//Car move control
	ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("remote_cmd_vel", 1000);
	ros::Publisher pub_AOA = nh.advertise<std_msgs::Float64MultiArray>("pub_AOA", 100);
	ros::Rate loop_rate(100);
	//
	int Decrease_flag = 0;
	Move_Control MCL(&OBS_obj);
	double v_pre,w_pre;
	geometry_msgs::Twist cmd;
	unsigned int AOA_Pre_Angle = 0;
	//Remote time
	double Remote_time = 0;
    int Switch_flag = 0;
	std_msgs::Float64MultiArray AOA_pub_Output;
	while (ros::ok())
	{
		AOA_pub_Output.data.clear();
		int Mode = Get_Mode();
		if(Mode==2)
		{
			Remote_time = 0;
			cmd.linear.x = MCL.v;
			cmd.angular.z = MCL.w;
			ROS_INFO("%d %d",AOA_Val[0],AOA_Val[1]);
			MCL.Updata(AOA_Val,UltraSonic_Pro.UltraSonic_Mean);
			MCL.Control();
		}
		//Slam Mode
		else if(Mode==1)
		{
			Remote_time = 0;
			cmd.linear.x = 0;
			cmd.angular.z = 0;
		}
		//Remote Mode
		else if(Mode==0 && (AOA_Val[3]!=0 || AOA_Val[4]!=0))
		{
			//Increase
			Remote_time = Remote_time + 1;
			//cout << "Mode: 0" << endl;
			float AOA_X = AOA_Val[3];
			float AOA_Y = AOA_Val[4];
			//L2 norm
			double Distance = sqrt(pow(AOA_X,2)+pow(AOA_Y,2));
			int Dist_Direction = 1;
			int Ang_Direction = 1;
			if(AOA_Y < 0)
			{
				Dist_Direction = -1;
			}
			if(AOA_X < 0)
			{
				Ang_Direction = -1;
			}
			Distance = Dist_Direction*Distance;
			double Angle = asin(AOA_Y/Distance);
			if(Distance > 50)
			{
                if(Switch_flag==1)
                {
                    Remote_time = 1;
                    Switch_flag=0;
                }
				//Speed Tweak (Sigmoid)
				double a = (1.0/(1.0+exp(-(Remote_time-50.0))))*0.8;
				//Lock Max
				double v = a * ((Distance -20)*0.01);
				cmd.linear.x = v;
				//1 rad = 1.57
				//Angle Tweak
				double a_w = (1.0 - exp(-2*Remote_time))/(1.0 + exp(-2*Remote_time));
				//cmd.angular.z = (Ang_Direction*(Angle-1.57))/1.2;
				cmd.angular.z = a * (Ang_Direction*(Angle-1.57));
				ROS_INFO("Linear Speed: %e, Angle Speed: %e\n", cmd.linear.x, cmd.angular.z);
				//cout << cmd.linear.x <<"," << cmd.angular.z << endl;
			}
			else if(Distance < -50)
			{
            	if(Switch_flag==0)
				{
					Remote_time = 0;
					Switch_flag=1;
				}
				//Speed Tweak (Sigmoid)
				double a = (1.0/(1.0+exp(-(Remote_time-50.0))))*0.8;
				//Lock Max
				double v = a * ((Distance + 20)*0.01);
				//Speed Tweak
				cmd.linear.x = v;
				//1 rad = 1.57
				//cmd.angular.z = (Ang_Direction*(Angle-1.57))/1.2;
                double a_w = (1.0 - exp(-2*Remote_time))/(1.0 + exp(-2*Remote_time));
                //cmd.angular.z = (Ang_Direction*(Angle-1.57))/1.2;
            	cmd.angular.z = a * (Ang_Direction*(Angle-1.57));
				ROS_INFO("Linear Speed: %e, Angle Speed: %e\n", cmd.linear.x, cmd.angular.z);
			}
			else
			{
				Remote_time = 0;
				cmd.linear.x = 0;
				cmd.angular.z = 0;
			}
		}
		pub.publish(cmd);
		AOA_pub_Output.data.push_back((double)Mode);
		AOA_pub_Output.data.push_back((double)cmd.linear.x);
		AOA_pub_Output.data.push_back((double)cmd.angular.z);
		pub_AOA.publish(AOA_pub_Output);
		ros::spinOnce();
		loop_rate.sleep();
  }
  return 0;
}
