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
unsigned int X = 0;
unsigned int Y = 0;
unsigned int Mode = 0;
int main(int argc, char **argv)
{
    ros::init(argc, argv, "Serial_AOA_node_New");
	ros::NodeHandle nh;
	ros::Publisher chatter_pub = nh.advertise<std_msgs::UInt32MultiArray>("Serial_AOA_node_New", 100);
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
	if(ser.isOpen()){
		//serL.setBreak(true);
		ROS_INFO_STREAM("Serial Port initialized");
	}
	else{ 
		return -1;
	}
	
    ros::Rate loop_rate(50);
	//ROS_INFO_STREAM("Start");
	//
    while (ros::ok())
    {
        unsigned char RXbuff[30]={0};
		uint16_t Angle = 0;
		uint16_t CM = 0;
	    ser.read(RXbuff,30);
        for(int i=0;i<30-9;i++)
		{
			//cout << std::hex << (int)RXbuff[i] << " ";
			if(RXbuff[i]==0xAA && RXbuff[i+1]==0x55 && RXbuff[i+3]==0x46)
			{
				/*
				for(int j=i;j<i+12;j++)
				{
					cout << std::hex << (int)RXbuff[j] << " ";
				}
				*/
				Mode = (uint16_t)(RXbuff[i+8]);
				X = (uint16_t)(RXbuff[i+9]);
				Y = (uint16_t)(RXbuff[i+10]);
				/*
				Angle = (uint16_t)(RXbuff[i+9] << 8 | RXbuff[i+8]);
				CM = (uint16_t)(RXbuff[i+11] << 8 | RXbuff[i+10]);
				*/
				break;
				///Angle = ((char)RXbuff[i+8] << 8 | (char)RXbuff[i+9]);
			}
		}
		//cout << Angle << endl;
		/*
		if(Angle>0 && Angle<=360)AOA_Angle=Angle;
		if(CM>0)AOA_CM=CM;
		*/
		//Analysis
		AOA_Value.data.clear();
		AOA_Value.data.push_back(X);
		AOA_Value.data.push_back(Y);
		cout << Mode << "," << X << "," << Y << endl;
		chatter_pub.publish(AOA_Value);
		loop_rate.sleep();
        ros::spinOnce();
    }
    return 0;
}