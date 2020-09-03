#include <ros/ros.h>
#include <serial/serial.h>
#include "std_msgs/String.h"
#include <sstream>
#include <std_msgs/UInt32MultiArray.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/UInt8MultiArray.h>

using namespace std;
//serial parameter
/////////////////////////
serial::Serial ser;
int baudrate;

//Motor Ultrasonic
std_msgs::UInt8MultiArray Ultrasoniccmd;
/**@brief Get Ultrasonic
*	@param	Ultra_LMR: Distance from ultrasonic sensor(m)
*	@return 0: Success
*/
int Get_Ultrasonic(unsigned int *Ultra_LMR)
{
	//Send Command
	Ultrasoniccmd.data.clear();
	Ultrasoniccmd.data.push_back(0x01);
	Ultrasoniccmd.data.push_back(0x03);
	Ultrasoniccmd.data.push_back(0x00);
	Ultrasoniccmd.data.push_back(0x00);
	Ultrasoniccmd.data.push_back(0x00);
	Ultrasoniccmd.data.push_back(0x03);
	Ultrasoniccmd.data.push_back(0x05);
	Ultrasoniccmd.data.push_back(0xCB);
	ser.write(Ultrasoniccmd.data);
	ros::Duration(0.05).sleep();
	//Receive
	unsigned char RXbuff[11]={0};
	ser.read(RXbuff,11);
#ifdef debug
	for(int i=0;i<11;i++)cout<<(int)RXbuff[i] << " ";
	cout<<endl;
#endif
	//Analysis
	unsigned int Left_Ultra = (unsigned int)((RXbuff[3]<<8)|RXbuff[4]);
	unsigned int Mid_Ultra = (unsigned int)((RXbuff[5]<<8)|RXbuff[6]);
	unsigned int Right_Ultra = (unsigned int)((RXbuff[7]<<8)|RXbuff[8]);
	//Long
	//Short
	if(Left_Ultra >= 450 || Left_Ultra <= 20)Left_Ultra = 1;
	if(Mid_Ultra >= 450 || Mid_Ultra <= 20)Mid_Ultra = 1;
	if(Right_Ultra >= 450 || Right_Ultra <= 20)Right_Ultra = 1;
	Ultra_LMR[0] = Left_Ultra;
	Ultra_LMR[1] = Mid_Ultra;
	Ultra_LMR[2] = Right_Ultra;
	/*
	//Get
	for(int RXbuff_index = 0; RXbuff_index < (sizeof(RXbuff)/sizeof(unsigned char))-8 ; RXbuff_index++)
	{	//cout <<  (int)*(RXbuff+RXbuff_index)<<" ";
		//cout << endl;
		//cout << *(RXbuff+RXbuff_index) << endl;
		//Get Header
		if(*(RXbuff+RXbuff_index)==0x01 && *(RXbuff+RXbuff_index+1)==0x03)
		{
			result[0]=*(RXbuff+RXbuff_index+2);
			result[1]=*(RXbuff+RXbuff_index+3);
			result[2]=*(RXbuff+RXbuff_index+4);
			result[3]=*(RXbuff+RXbuff_index+5);
			return 0;
		}
	}*/
	//for(int i=0;i<8;i++)cout << (int)buff[i] << " ";
	//cout << endl;
	//ros::Duration(1).sleep();
	return 0;
}

int main (int argc, char** argv)
{
	ros::init(argc, argv, "serial_Ultrasonic_node");
	ros::NodeHandle nh;
	ros::Publisher chatter_pub = nh.advertise<std_msgs::UInt32MultiArray>("serial_Ultrasonic_node", 100);
	try{
		//initial port
		ser.setPort("/dev/ttyUSB1");
		ser.setBaudrate(9600);
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
	unsigned int RX_Ultra_Buff[3]={0};
	//Get Ultrasonic
	std_msgs::UInt32MultiArray UltrasonicValue;
	while(ros::ok())
	{
		//ROS_INFO_STREAM("Enter");
		UltrasonicValue.data.clear();
		int RX_Status = Get_Ultrasonic(RX_Ultra_Buff);
		if(!RX_Status)
		{
			UltrasonicValue.data.push_back(RX_Ultra_Buff[0]);
			UltrasonicValue.data.push_back(RX_Ultra_Buff[1]);
			UltrasonicValue.data.push_back(RX_Ultra_Buff[2]);
			chatter_pub.publish(UltrasonicValue);
			//ROS_INFO_STREAM("Have UltrasonicValue");
		}
		else
		{
			UltrasonicValue.data.push_back(0x00);
			UltrasonicValue.data.push_back(0x00);
			UltrasonicValue.data.push_back(0x00);
			chatter_pub.publish(UltrasonicValue);
			//ROS_INFO_STREAM("Not UltrasonicValue");
		}
		//ros::Duration().sleep();
		ros::spinOnce();
    }
}
