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
std_msgs::UInt32MultiArray UWB_Value;

unsigned int hex2deci(unsigned char *strHex)
{
	unsigned int dwValue = 0;
	unsigned char *pCh = strHex;
	while(*pCh!=' ')
	{
		//cout << dwValue << endl;
		dwValue <<= 4;
		if(*pCh >= '0' && *pCh <= '9')dwValue += *pCh - '0';
		else if (*pCh >= 'A' && *pCh <= 'F')dwValue += *pCh - 'A' + 10;
		else if (*pCh >= 'a' && *pCh <= 'f')dwValue += *pCh - 'a' + 10;
		else return 0;
		pCh++;
	}
	return dwValue;
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "Serial_UWB_node");
	ros::NodeHandle nh;
	ros::Publisher chatter_pub = nh.advertise<std_msgs::UInt32MultiArray>("Serial_UWB_node", 100);
	try{
		//initial port
		ser.setPort("/dev/ttyACM1");
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
		ROS_INFO_STREAM("SerialTX Port initialized");
	}
	else{
		return -1;
	}
    ros::Rate loop_rate(10);
	ROS_INFO_STREAM("Start");
  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
    int count = 0;
    while (ros::ok())
    {
        unsigned char RXbuff[65]={0};
	    ser.read(RXbuff,65);
		//ROS_INFO_STREAM("Read");
		//Check MID is 'mr'
		if(RXbuff[0]!='m' || RXbuff[1]!='r')continue;
		//base
		int Base = 4;
		//Get T-A0,T-A1,T-A2
		unsigned char T_A0[9] = {0};
		unsigned char T_A1[9] = {0};
		unsigned char T_A2[9] = {0};
		//ROS_INFO_STREAM("T_A0");
		count = 0;
		//T-A0
		for(int T_A0_index = 6 + Base; T_A0_index < 14+Base; T_A0_index++)
		{
			T_A0[count] = RXbuff[T_A0_index];
			//cout << T_A0[count];
			count = count + 1; 
		}
		//cout << endl;
		count = 0;
		//T-A1
		//ROS_INFO_STREAM("T_A1");
		for(int T_A1_index = 15+Base; T_A1_index < 23+Base; T_A1_index++)
		{
			T_A1[count] = RXbuff[T_A1_index];
			//cout << T_A1[count];
			count = count + 1;
		}
		//cout << endl;
		count = 0;
		//T-A2
		//ROS_INFO_STREAM("T_A2");
		for(int T_A2_index = 24+Base; T_A2_index < 32+Base; T_A2_index++)
		{
			T_A2[count] = RXbuff[T_A2_index];
			//cout << T_A2[count];
			count = count + 1;
		}
		//cout << endl;
		
		//Distance
		//ROS_INFO_STREAM("T_A0_Distance");
		unsigned int T_A0_Distance = hex2deci(T_A0);
		//ROS_INFO_STREAM("T_A1_Distance");
		unsigned int T_A1_Distance = hex2deci(T_A1);
		//ROS_INFO_STREAM("T_A2_Distance");
		unsigned int T_A2_Distance = hex2deci(T_A2);
		UWB_Value.data.clear();
		UWB_Value.data.push_back(T_A0_Distance);
		UWB_Value.data.push_back(T_A1_Distance);
		UWB_Value.data.push_back(T_A2_Distance);
		chatter_pub.publish(UWB_Value);
		//cout << T_A0_Distance << "mm ," << T_A1_Distance << "mm ," << T_A2_Distance << "mm." << endl;
		
		/*
		//cout << count++ << ":";
        for(int i=0;i<65;i++)
        {
            cout << (unsigned char)RXbuff[i];
        }
		*/
		loop_rate.sleep();
		//ros::Duration(0.01).sleep();
        ros::spinOnce();
    }
    return 0;
}