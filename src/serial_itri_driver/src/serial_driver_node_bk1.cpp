#include <ros/ros.h>
#include <serial/serial.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/UInt8MultiArray.h>
#include <math.h>
#include <vector>

using namespace std;

//const double WheelDistance = 0.235;		//m //0.224
const double WheelDistance = 0.55;
const double WheelRadius = 0.099;		//m
const double EncoderPlus = 1024.0 * 4.0; //4 pole
const double GearRatio = 1.0;
const double DriverSampleTime = 0.01;		//s
const unsigned long fourthmultiple = pow(2, 24);
const unsigned int thirdmultiple = pow(2, 16);
const unsigned long secondmultiple = pow(2, 8);

//serial parameter
/////////////////////////
serial::Serial ser;
//serial::Serial serRX;
//serial::Serial serR;
std::string Lport, Rport;
int baudrate;

bool writing = false;
bool speedset_flag = false;

//Motor Cmd
std_msgs::UInt8MultiArray motorcmd;
//std_msgs::UInt8MultiArray rmotorcmd;

void initial();
void driverinitial();
void write_serial(const geometry_msgs::Twist::ConstPtr& speed);

int Get_Car_info(unsigned char *result)
{
	unsigned char RXbuff[16]={0};
	ser.read(RXbuff,16);
	//Get Speed,Disection
	for(int RXbuff_index = 0; RXbuff_index < 16 ; RXbuff_index++)
	{	//cout <<  (int)*(RXbuff+RXbuff_index)<<" ";
		//cout << endl;
		//cout << *(RXbuff+RXbuff_index) << endl;
		//Get Header
		if(*(RXbuff+RXbuff_index)==0xFF && *(RXbuff+RXbuff_index+1)==0xFE)
		{
			result[0]=*(RXbuff+RXbuff_index+2);
			result[1]=*(RXbuff+RXbuff_index+3);
			result[2]=*(RXbuff+RXbuff_index+4);
			result[3]=*(RXbuff+RXbuff_index+5);
			return 0;
		}
	}
	//for(int i=0;i<8;i++)cout << (int)buff[i] << " ";
	//cout << endl;
	//ros::Duration(1).sleep();
	return 1;
}
int main (int argc, char** argv)
{
	ros::init(argc, argv, "serial_driver_node");
	ros::NodeHandle nh;
	ros::Subscriber cmd_sub = nh.subscribe("cmd_vel", 1000, write_serial);
	ros::Publisher Encode_Data = nh.advertise<std_msgs::UInt8MultiArray>("Encode_val", 100);
	//ros::Subscriber cmd_sub = nh.subscribe("cmd_vel", 1000, write_serial);
	//ros::Publisher odom_pub = nh.advertise<nav_msgs::Odometry>("odom", 1000);
	//tf::TransformBroadcaster odom_broadcaster;
	
	//initial();
	//connect to serial
	
	try{
		//initial port
		ser.setPort("/dev/ttyUSB0");
		ser.setBaudrate(115200);
		serial::Timeout Lto = serial::Timeout::simpleTimeout(0);
		ser.setTimeout(Lto);
		ser.open();
		/*
		//Left port
		serL.setPort(Lport);
		serL.setBaudrate(baudrate);
		serial::Timeout Lto = serial::Timeout::simpleTimeout(0);
		serL.setTimeout(Lto);
		serL.open();
		//Right port
		serR.setPort(Rport);
		serR.setBaudrate(baudrate);
		serial::Timeout Rto = serial::Timeout::simpleTimeout(0);
		serR.setTimeout(Rto);
		serR.open();
	*/	
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
	/*	
	if(serRX.isOpen()){
		//serL.setBreak(true);
		ROS_INFO_STREAM("SerialRX Port initialized");
	}
	else{
		return -1;
	}*/
	//driverinitial();
	/*
	std_msgs::UInt8MultiArray array;

	vector<unsigned char> lencoder(8);
	vector<unsigned char> rencoder(8);
	long int encoderPulse[2] = {0, 0};
	long int tencoderPulse[2] = {0, 0};
	long int d_encoderPulse[2] = {0, 0};
	short Lsave[2] = {0};
	short Rsave[2] = {0};
	
	//Odometry
	double Odometry[3] = {0.0};	//x,y,theta (m,m,rad) -pi~pi
	double WV[2] = { 0.0, 0.0 };
	
	ros::Rate loop_rate(15);
	ros::Time current_time;
	motorcmd.data.clear();
	
	*/
	std_msgs::UInt8MultiArray Encode_Val;
	Encode_Val.data.clear();
	ros::Rate loop_rate(10);
	unsigned char RX_Car_Buff[4]={0};
	while(ros::ok())
	{
		int RX_Status = Get_Car_info(RX_Car_Buff);
		if(!RX_Status)
		{
			Encode_Val.data.clear();
			for(int i=0;i<4;i++)
			{
				Encode_Val.data.push_back(*(RX_Car_Buff+i));
				//cout << (int)*(RX_Car_Buff+i)<<" ";
			}
			/*
				cout<<endl;
			*/
		}
		//ros::Duration().sleep();
		//publish the message
		Encode_Data.publish(Encode_Val);
		loop_rate.sleep();
		ros::spinOnce();
	}

}
/*
void driverinitial(){
	std_msgs::UInt8MultiArray array;
	//stop motor
	///////////////////////
	array.data.clear();
	array.data.push_back(0xFE);	
	array.data.push_back(0xFF);
	//Speed
	array.data.push_back(0x00);
	array.data.push_back(0x00);
	//Direct
	array.data.push_back(0x00);
	array.data.push_back(0x00);
	//Reserve
	array.data.push_back(0x00);
	array.data.push_back(0x00);
	array.data.push_back(0x00);
	array.data.push_back(0x00);
	//array.data.push_back(0x2F);  //Choose speed mode
	//array.data.clear();
	ser.write(array.data);
	ros::Duration(0.5).sleep();
	
	//std::cout<<"ReceiveR="<<(int)(unsigned int)array.data[0]<<" ;"<< (int)(unsigned int)array.data[1] << "\n";
}
*/
void write_serial(const geometry_msgs::Twist::ConstPtr& speed)
{
	//ROS_INFO_STREAM("Enter...");
	//m/s
	double vr = (speed->linear.x * 2.0 + speed->angular.z * WheelDistance) / 2.0;
	double vl = (speed->linear.x * 2.0 - speed->angular.z * WheelDistance) / 2.0;
	//rpm
	int leftmotorvalue = (int)(round(vl / (2.0 * M_PI *  WheelRadius) * 60.0));
	int rightmotorvalue = (int)(round(vr / (2.0 * M_PI *  WheelRadius) * 60.0));
	//Max rpm
	if (leftmotorvalue > 150)
		leftmotorvalue = 150;
	else if (leftmotorvalue < 0)
		leftmotorvalue = 0;

	if (rightmotorvalue > 150)
		rightmotorvalue = 150;
	else if (rightmotorvalue < 0)
		rightmotorvalue = 0;
	
	if(leftmotorvalue-rightmotorvalue > 50)rightmotorvalue+=50;
	else if(leftmotorvalue-rightmotorvalue < -50)leftmotorvalue+=50;
	//Change driver speed type
	// real rpm = (cmd / 16384) * 6000;
	int motorLcmd = (int)(round(leftmotorvalue * 16384.0 / 6000.0));
	int motorRcmd = (int)(round(rightmotorvalue * 16384.0 / 6000.0));
	//ROS_INFO("moto %d %d",(int)motorLcmd,( int)motorRcmd);
	//Left motor speed setup
	///////////////////////
	motorcmd.data.clear();
	motorcmd.data.push_back(0xFF);	
	motorcmd.data.push_back(0xFE);
	//motorcmd.data.push_back(motorLcmd);
	motorcmd.data.push_back((unsigned char)motorLcmd);
	motorcmd.data.push_back((unsigned char)motorRcmd);
	
	//motorcmd.data.push_back(motorRcmd);
	motorcmd.data.push_back(0x00);
	motorcmd.data.push_back(0x00);
	//motorcmd.data.push_back(0x00);
	//Reserve
	motorcmd.data.push_back(0x00);
	motorcmd.data.push_back(0x00);
	motorcmd.data.push_back(0x00);
	motorcmd.data.push_back(0x00);
	speedset_flag = true;
	ser.write(motorcmd.data);
	#if 0
	//Left
	while(1){
				if(serL.available() >= 2){
						//ROS_INFO_STREAM("Reading from serial port");
						lmotorcmd.data.clear();
						serL.read(lmotorcmd.data,2);	//read 2 unsigned char
								break;
					}
	}
							
				//right
	while(1){
			if(serR.available() >= 2){
						//ROS_INFO_STREAM("Reading from serial port");
						rmotorcmd.data.clear();
						serR.read(rmotorcmd.data,2);	//read 2 unsigned char
					break;
				}				
			}
		#endif
}

