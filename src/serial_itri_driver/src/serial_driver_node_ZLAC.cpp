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
std_msgs::UInt8MultiArray motorRcmd;
std_msgs::UInt8MultiArray motorLcmd;
//std_msgs::UInt8MultiArray rmotorcmd;
unsigned char Right_E_Dist_Now, Left_E_Dist_Now;
unsigned char Right_E_Dist_pre, Left_E_Dist_Pre;
unsigned char Right_CE_Dist_Now, Left_CE_Dist_Now;
unsigned char Right_Dist_Pre, Left_Dist_Pre;
unsigned char RX_Car_Buff[4];
unsigned char RZ_bias, LZ_bias;
void initial();
void driverinitial();
void write_serial(const geometry_msgs::Twist::ConstPtr& speed);

int Get_Car_info(unsigned char *result)
{
	unsigned char RXbuff[30]={0};
    std_msgs::UInt8MultiArray CMD;
    CMD.data.clear();
    //Send Read Command
    CMD.data.push_back(0x80);
	CMD.data.push_back(0x00);
	CMD.data.push_back(0x80);
    //Read Machine Status
	ser.read(RXbuff,30);

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
	Right_E_Dist_Now = 0;
	Left_E_Dist_Now = 0;
	Right_E_Dist_pre = 0;
	Left_E_Dist_Pre = 0;
	Right_CE_Dist_Now = 0;
	Left_CE_Dist_Now = 0;
	Right_Dist_Pre =0 ;
	Left_Dist_Pre = 0;
	RZ_bias = 0;
	LZ_bias = 0;
	try{
		//initial port
        //Left
		Ser_Left.setPort("/dev/ttyUSB0");
		Ser_Left.setBaudrate(57600);
		serial::Timeout Lto = serial::Timeout::simpleTimeout(0);
		Ser_Left.setTimeout(Lto);
		Ser_Left.open();
        //Right
		Ser_Right.setPort("/dev/ttyUSB1");
		Ser_Right.setBaudrate(57600);
		serial::Timeout Lto = serial::Timeout::simpleTimeout(0);
		Ser_Right.setTimeout(Lto);
		Ser_Right.open();
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
	for(int i=0;i<4;i++)
	{
		RX_Car_Buff[i] = 0;
	}
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
void driverinitial(){
	std_msgs::UInt8MultiArray array;
	//Stop motor
	array.data.clear();
	array.data.push_back(0x00);	
	array.data.push_back(0x00);
	array.data.push_back(0x00);
	array.data.push_back(0x00);
    Ser_Right.write(array.data);
    Ser_Left.write(array.data);
    //Setting Speed Mode
    array.data.clear();
	array.data.push_back(0x02);	
	array.data.push_back(0x00);
	array.data.push_back(0xC4);
	array.data.push_back(0xC6);
    Ser_Right.write(array.data);
    Ser_Left.write(array.data);
    //Start motor
	array.data.clear();
	array.data.push_back(0x00);	
	array.data.push_back(0x00);
	array.data.push_back(0x01);
	array.data.push_back(0x01);
    Ser_Right.write(array.data);
    Ser_Left.write(array.data);
	ros::Duration(0.5).sleep();
}

void write_serial(const geometry_msgs::Twist::ConstPtr& speed)
{
	int Stop_flag = 0;
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
	
	//if(leftmotorvalue-rightmotorvalue > 50)rightmotorvalue+=50;
	//else if(leftmotorvalue-rightmotorvalue < -50)leftmotorvalue+=50;

	if(leftmotorvalue == 0 && rightmotorvalue==0)
	{
		Stop_flag =1;
	}
	//Change driver speed type
	// real rpm = (cmd / 16384) * 6000;
	int motorLcmd = (int)(round(leftmotorvalue * 16384.0 / 6000.0));
	int motorRcmd = (int)(round(rightmotorvalue * 16384.0 / 6000.0));
	/********************************************************/
	unsigned char R_Motor_Bias = 0;
	unsigned char L_Motor_Bias = 0;
	//
	Right_E_Dist_Now = (unsigned char)motorRcmd - RX_Car_Buff[0];
	Left_E_Dist_Now = (unsigned char)motorLcmd - RX_Car_Buff[2];
	//
	Right_CE_Dist_Now = Right_E_Dist_Now - Right_E_Dist_pre;
	Left_CE_Dist_Now = Left_E_Dist_Now - Left_E_Dist_Pre;
	//
	Right_E_Dist_pre = Right_E_Dist_Now;
	Left_E_Dist_Pre = Left_E_Dist_Now;
	//
	Right_Dist_Pre = (unsigned char)motorRcmd;
	Left_Dist_Pre = (unsigned char)motorLcmd;
	//cout << (int)motorRcmd << endl;
	//cout << (int)Right_E_Dist_Now << "," << (int)Right_CE_Dist_Now << endl;
	//Right

	if(Right_E_Dist_Now>0 && Right_CE_Dist_Now>0)
	{
		RZ_bias += 0x02;
		if(RZ_bias>0x20)RZ_bias = 0x20;
		//cout << "R Increase" << endl;
		//R_Motor_Bias = (unsigned char)motorRcmd + RZ_bias;
	}
	else if (Right_E_Dist_Now<0 && Right_CE_Dist_Now<0)
	{
		RZ_bias -= 0x01;
		//if(RZ_bias<1)RZ_bias=1;
		//cout << "R Decrease" << endl;
		//R_Motor_Bias = (unsigned char)motorRcmd - RZ_bias;
	}
	else
	{
		RZ_bias += 0;
			
	}
	R_Motor_Bias = (unsigned char)motorRcmd + RZ_bias;
	if(R_Motor_Bias<0)
	{
		R_Motor_Bias = 0;
	}
	//Left
	if(Left_E_Dist_Now>0 && Left_CE_Dist_Now>0)
	{
		LZ_bias += 0x02;
		if(LZ_bias>0x20)LZ_bias = 0x20;
		//cout << "L Increase" << endl;
		//L_Motor_Bias = (unsigned char)motorLcmd + LZ_bias;
	}
	else if (Left_E_Dist_Now<0 && Left_CE_Dist_Now<0)
	{
		LZ_bias -= 0x01;
		//if(LZ_bias<1)LZ_bias=1;
		//cout << "L Decrease" << endl;
		//L_Motor_Bias = (unsigned char)motorLcmd - LZ_bias;
	}
	else
	{
		LZ_bias += 0;
		//L_Motor_Bias = (unsigned char)motorLcmd;
	}
	L_Motor_Bias = (unsigned char)motorLcmd + LZ_bias;
	if(L_Motor_Bias<0)
	{
		L_Motor_Bias = 0;
	}
	if(Stop_flag==1)
	{
		L_Motor_Bias = 0;
		R_Motor_Bias = 0;
	}
	/********************************************************/
    //Left
    unsigned char Left_CMD[4]={0};
    Left_CMD[0] = 0x06;
    Left_CMD[1] = L_Motor_Bias >> 8; //MSB
    Left_CMD[2] = (L_Motor_Bias & 0x0F) >> 8 ; //LSB
    Left_CMD[3] = (Left_CMD[1] + Left_CMD[2]) & 0x0F;
	motorLcmd.data.clear();
	motorLcmd.data.push_back(Left_CMD[0]);	
	motorLcmd.data.push_back(Left_CMD[1]);
    motorLcmd.data.push_back(Left_CMD[2]);
    motorLcmd.data.push_back(Left_CMD[3]);
    //Right
    unsigned char Right_CMD[4]={0};
    Right_CMD[0] = 0x06;
    Right_CMD[1] = R_Motor_Bias >> 8; //MSB
    Right_CMD[2] = (R_Motor_Bias & 0x0F) >> 8 ; //LSB
    Right_CMD[3] = (Left_CMD[1] + Left_CMD[2]) & 0x0F;
    motorRcmd.data.clear();
    motorRcmd.data.push_back(Right_CMD[0]);
    motorRcmd.data.push_back(Right_CMD[1]);
    motorRcmd.data.push_back(Right_CMD[2]);
    motorRcmd.data.push_back(Right_CMD[3]);
	Ser_Right.write(motorRcmd.data);
    Ser_Left.write(motorLcmd.data);
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

