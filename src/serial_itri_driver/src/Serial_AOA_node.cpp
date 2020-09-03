#include "ros/ros.h"
#include "std_msgs/String.h"
#include <serial/serial.h>
#include <sstream>
#include <iostream>
#include <string>
#include <std_msgs/UInt32MultiArray.h>
using namespace std;
serial::Serial ser;
//UWB
std_msgs::UInt32MultiArray AOA_Value;
unsigned int AOA_Angle = 0;
unsigned int AOA_CM = 0;
//
int main(int argc, char **argv)
{
    ros::init(argc, argv, "Serial_AOA_node");
	ros::NodeHandle nh;
	ros::Publisher chatter_pub = nh.advertise<std_msgs::UInt32MultiArray>("Serial_AOA_node", 100);
	try{
		//initial port
		ser.setPort("/dev/ttyS2");
		ser.setBaudrate(115200);
		serial::Timeout Lto = serial::Timeout::simpleTimeout(0);
		ser.setTimeout(Lto);
		ser.open();
	}
	catch (serial::IOException& e){
		ROS_ERROR_STREAM("Unable to open port ");
		return -1;
	}
	//open or not
	if(ser.isOpen())
	{
		//serL.setBreak(true);
		ROS_INFO_STREAM("Serial Port initialized");
	}
	else
	{ 
		return -1;
	}
	//
    ros::Rate loop_rate(50);
	while (ros::ok())
	{
		double secs = ros::Time::now().toSec();
		double nsec = ros::Time::now().toNSec();
		double Now_Time = secs + nsec * 10e-9;
		//Clear Flag
		bool Read_Follow_Success = false;
		bool Read_Mode_Success = false;
		//Clear AOA
		AOA_Value.data.clear();
		//UART Buffer
		unsigned char RXbuff[40]={0};
		//Read UART Data
		ser.read(RXbuff,40);
		uint16_t Angle = 0;
		uint16_t CM = 0;
		//Get Follow
		for(int i=0;i<40-9;i++)
		{
			ROS_INFO("%x,",(int)RXbuff[i]);
			if(RXbuff[i]==0xAA && RXbuff[i+1]==0x55 && RXbuff[i+3]==0x3C)
			{
				Angle = (uint16_t)(RXbuff[i+9] << 8 | RXbuff[i+8]);
				CM = (uint16_t)(RXbuff[i+11] << 8 | RXbuff[i+10]);
				//Set Flag
				Read_Follow_Success = true;
				//Normal
				if(Angle>0 && Angle<=360)AOA_Angle=Angle;
				if(CM>0)AOA_CM=CM;
				break;
			}
		}
		//Mode,X,Y
		uint8_t Mode = 0;
		int8_t X = 0;
		int8_t Y = 0;
		//Get Remote and Mode
		for(int i=0;i<40-9;i++)
		{
			if(RXbuff[i]==0xAA && RXbuff[i+1]==0x55 && RXbuff[i+3]==0x46)
			{
				Mode = (uint16_t)(RXbuff[i+8]);
				X = (int8_t)(RXbuff[i+9]);
				Y = (int8_t)(RXbuff[i+10]);
				//Set Flag
				Read_Mode_Success = true;
				break;
			}
		}
		//Put AOA Information into Publish Package
		AOA_Value.data.push_back(AOA_Angle);
		AOA_Value.data.push_back(AOA_CM);
		if(Mode == 0 && X ==0 && Y == 0)Mode = 3;
		AOA_Value.data.push_back(Mode);
		AOA_Value.data.push_back(X);
		AOA_Value.data.push_back(Y);
		AOA_Value.data.push_back(Now_Time);
		//Check 
		if(Read_Follow_Success * Read_Mode_Success == true)
		{
			chatter_pub.publish(AOA_Value);
		}
		//
		loop_rate.sleep();
        ros::spinOnce();
	}
	//ROS_INFO_STREAM("Start");
	//
	/*
    while (ros::ok())
    {
        unsigned char RXbuff[40]={0};
		uint16_t Angle = 0;
		uint16_t CM = 0;
	    ser.read(RXbuff,40);
		//for(int i = 0;i<40;i++)ROS_INFO("%d",RXbuff[i]);
		//Get Follow
		for(int i=0;i<40-9;i++)
		{
			if(RXbuff[i]==0xAA && RXbuff[i+1]==0x55 && RXbuff[i+3]==0x3C)
			{
				Angle = (uint16_t)(RXbuff[i+9] << 8 | RXbuff[i+8]);
				CM = (uint16_t)(RXbuff[i+11] << 8 | RXbuff[i+10]);
				break;
			}
		}
		uint8_t Mode = 0;
		int8_t X = 0;
		int8_t Y = 0;
		//Get Remote and Mode
		for(int i=0;i<40-9;i++)
		{
			if(RXbuff[i]==0xAA && RXbuff[i+1]==0x55 && RXbuff[i+3]==0x46)
			{
				Mode = (uint16_t)(RXbuff[i+8]);
				X = (int8_t)(RXbuff[i+9]);
				Y = (int8_t)(RXbuff[i+10]);
				break;
			}
		}
		//cout << Angle << endl;
		if(Angle>0 && Angle<=360)AOA_Angle=Angle;
		if(CM>0)AOA_CM=CM;
		//Analysis
		AOA_Value.data.clear();
		AOA_Value.data.push_back(AOA_Angle);
		AOA_Value.data.push_back(AOA_CM);
		if(Mode == 0 && X ==0 && Y == 0)
		{
			Mode = 3;
		}
		AOA_Value.data.push_back(Mode);
		AOA_Value.data.push_back(X);
		AOA_Value.data.push_back(Y);
		chatter_pub.publish(AOA_Value);
		loop_rate.sleep();
        ros::spinOnce();
    }
	*/
    return 0;
}