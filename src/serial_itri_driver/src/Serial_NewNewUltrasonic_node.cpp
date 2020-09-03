#include "ros/ros.h"
#include "std_msgs/String.h"
#include <serial/serial.h>
#include <sstream>
#include <iostream>
#include <string>
#include <std_msgs/UInt8MultiArray.h>
#include <std_msgs/UInt32MultiArray.h>
using namespace std;
serial::Serial ser;
std_msgs::UInt32MultiArray UltrasonicData;
int main(int argc, char **argv)
{
    ros::init(argc, argv,"Serial_NewNewUltrasonic_node");
	ros::NodeHandle nh;
	ros::Publisher chatter_pub = nh.advertise<std_msgs::UInt32MultiArray>("Serial_NewNewUltrasonic_node", 100);
	try{
		//initial port
		ser.setPort("/dev/ttyS5");
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
	
    ros::Rate loop_rate(50);
	//ROS_INFO_STREAM("Start");
	//
    while (ros::ok())
    {
        unsigned char RXbuff;
		//Wait Head and Tail
		bool Packet_E = false;
		UltrasonicData.data.clear();
		while(!Packet_E)
		{	
			ser.read(&RXbuff,1);
			//ROS_INFO("%x",RXbuff);
			// Have Read
			if(RXbuff == 0x54)
			{
				//Skip 
				for(int RXbuff_index = 0; RXbuff_index < 1; ++RXbuff_index)
				{
					ser.read(&RXbuff,1);
				}
				for(int RXbuff_index = 0; RXbuff_index < 4; ++RXbuff_index)
				{
					ser.read(&RXbuff,1);
					UltrasonicData.data.push_back(RXbuff);
				}
				Packet_E = true;
				break;
			}
			/*
			// Read Ultrasonic to Buffer
			ser.read(RXbuff,25);
			//
			for(int i=0;i<25;++i)
			{
				ROS_INFO("%x",RXbuff[i]);
			}
			ROS_INFO("\n");
			// Find Header
			for(int Ultra_index = 0; Ultra_index < 25 - 5; ++Ultra_index)
			{
				// If Have Read
				if(RXbuff[Ultra_index] == 0x55 && RXbuff[Ultra_index+1]==0x54 && RXbuff[Ultra_index+11]==0xFF)
				{
					//Left
					UltrasonicData.data.push_back(RXbuff[Ultra_index+3]);
					UltrasonicData.data.push_back(RXbuff[Ultra_index+4]);
					UltrasonicData.data.push_back(RXbuff[Ultra_index+5]);
					UltrasonicData.data.push_back(RXbuff[Ultra_index+6]);
					//Right
					Packet_E = true;
					break;
				}
			}
			*/
			/*
			//
			for(int i=0;i<50;++i)
			{
				ROS_INFO("%x",RXbuff[i]);
			}
			ROS_INFO("\n");
			
			//ROS_INFO("%x",RXbuff[0]);
			//Check
			if(RXbuff[0]==0x55 && RXbuff[1]==0x54 && RXbuff[11]==0xFF)
			{
				//Calc
				//Left
				UltrasonicData.data.push_back(RXbuff[3]);
				UltrasonicData.data.push_back(RXbuff[4]);
				UltrasonicData.data.push_back(RXbuff[5]);
				UltrasonicData.data.push_back(RXbuff[6]);
				//Right
				Packet_E = true;
			}
			*/
		}
		chatter_pub.publish(UltrasonicData);
		loop_rate.sleep();
		//chatter_pub.publish(UltrasonicData);
        ros::spinOnce();
    }
    return 0;
}