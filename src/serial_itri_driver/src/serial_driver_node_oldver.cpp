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

const double WheelDistance = 0.235;		//m
const double WheelRadius = 0.099;		//m
const double EncoderPlus = 2000.0*2.0; 
const double GearRatio = 1.0;
const double DriverSampleTime = 0.01;		//s
const unsigned long fourthmultiple = pow(2, 24);
const unsigned int thirdmultiple = pow(2, 16);
const unsigned long secondmultiple = pow(2, 8);

//serial parameter
/////////////////////////
serial::Serial serL;
serial::Serial serR;
std::string Lport, Rport;
int baudrate;

bool writing = false;
bool speedset_flag = false;

//Motor Cmd
std_msgs::UInt8MultiArray lmotorcmd;
std_msgs::UInt8MultiArray rmotorcmd;

void initial();
void driverinitial();
void write_serial(const geometry_msgs::Twist::ConstPtr& speed);


int main (int argc, char** argv)
{
    
	ros::init(argc, argv, "serial_driver_node");
	ros::NodeHandle nh;

	ros::Subscriber cmd_sub = nh.subscribe("cmd_vel", 1000, write_serial);
	ros::Publisher odom_pub = nh.advertise<nav_msgs::Odometry>("odom", 1000);
	tf::TransformBroadcaster odom_broadcaster;
	
	initial();
	//connect to serial
	try{
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
	}
	catch (serial::IOException& e){
		ROS_ERROR_STREAM("Unable to open port ");
		return -1;
	}
	//open or not
	if(serL.isOpen()){
		//serL.setBreak(true);
		ROS_INFO_STREAM("Serial Leftwheel Port initialized");
	}
	else{
		return -1;
	}
	if(serR.isOpen()){
		//serR.setBreak(true);
		ROS_INFO_STREAM("Serial Rightwheel Port initialized");
	}
	else{
		return -1;
	}
	
	ros::Rate loop_rate(15);
	
	driverinitial();
		
	std_msgs::UInt8MultiArray array;
	std_msgs::UInt8MultiArray arrayR;
	int count[2] = {0, 0};
	bool initialenc = true;

	vector<unsigned char> lencoder(4);
	vector<unsigned char> rencoder(4);
	long int encoderPulse[2] = {0, 0};
	long int tencoderPulse[2] = {0, 0};
	ros::Time current_time;

	while(ros::ok()){
/*
		if(serL.isOpen()||serR.isOpen()){
			;
		}else{
			serL.open();
			serR.open();			
			cout << "LClose\n";
		}
*/
		if(speedset_flag == false){	
			array.data.clear();
			array.data.push_back(0x80);
			array.data.push_back(0x00);	
			array.data.push_back(0x80);
			serL.write(array.data);
			//ros::Duration(0.1).sleep();
			serR.write(array.data);
			ros::Duration(0.1).sleep();
			for(int i = 0; i < 100; i++){
				//Read left
				if(serL.available()>=4 ){
					array.data.clear();
					serL.read(array.data,4);	//read 4 unsigned char
					std::cout<<"L_DriverInfo = "<<(int)(unsigned int)array.data[0]<<" ;"<< (int)(unsigned int)array.data[1] << " ;" << (int)(unsigned int)array.data[2] << " ;"<< (int)(unsigned int)array.data[3] << "\n";
					count[0]++;
					if(count[0] == 8){
						count[0] = 0;
						//left encoder Low 16
						lencoder[2] = array.data[1];
						lencoder[3] = array.data[2];
						cout << "=======================\n";
						cout<<"LeftEncoder="<< (int)lencoder[0]<<" ;"<< (int)lencoder[1] << " ;" << (int)lencoder[2]<< " ;"<< (int)lencoder[3] << "\n";
						cout << "-----------------------\n";
						break;
					}else if(count[0] == 7){
						//right encoder High 16
						lencoder[0] = array.data[1];
						lencoder[1] = array.data[2];
					}
				}else{
					break;
				}
				
				
			}
			for(int i = 0; i < 100; i++){
				//Read right
				if(serR.available()>=4 ){
					arrayR.data.clear();
					serR.read(arrayR.data,4);	//read 4 unsigned char
					std::cout<<"R_DriverInfo = "<<(int)(unsigned int)arrayR.data[0]<<" ;"<< (int)(unsigned int)arrayR.data[1] << " ;" << (int)(unsigned int)arrayR.data[2] << " ;"<< (int)(unsigned int)arrayR.data[3] << "\n";
					count[1]++;
					if(count[1] == 8){
						count[1] = 0;
						//left encoder Low 16
						rencoder[2] = arrayR.data[1];
						rencoder[3] = arrayR.data[2];
						cout << "=======================\n";
						cout<<"RightEncoder="<< (int)rencoder[0]<<" ;"<< (int)rencoder[1] << " ;" << (int)rencoder[2]<< " ;"<< (int)rencoder[3] << "\n";
						cout << "-----------------------\n";
						break;
					}else if(count[1] == 7){
						//right encoder High 16
						rencoder[0] = arrayR.data[1];
						rencoder[1] = arrayR.data[2];
					}
				}else{
					break;
				}							
			}
		}else{
			serL.write(lmotorcmd.data);
			serR.write(rmotorcmd.data);
		
			//Display write sucess	
			//ros::Duration(0.1).sleep();
			short countsix = 0;
			short concount = 0;
			short identifytemp[4] = {0, 0, 0, 0};
			//right
			short rcountsix = 0;
			short rconcount = 0;
			short ridentifytemp[4] = {0, 0, 0, 0};
			
			
			while(1){
				if(serL.available() > 0){
						//ROS_INFO_STREAM("Reading from serial port");
						lmotorcmd.data.clear();
						serL.read(lmotorcmd.data,1);	//read 2 unsigned char
						concount++;
						if(lmotorcmd.data[0] == 0x06){						
							identifytemp[countsix] = lmotorcmd.data[0]; 
							identifytemp[countsix+1] = concount;
							countsix+=2;
							//cout << "0xe6? "<< (int)(unsigned int)identifytemp[0] <<", "<< identifytemp[2] <<", "<<(identifytemp[3] - identifytemp[1]) << "\n";
							if((identifytemp[2] == identifytemp[0]) && (identifytemp[3] - identifytemp[1]) == 1){
								countsix = 0;
								concount = 0;
								identifytemp[0] = 0;
								identifytemp[2] = 0;
								//speedset_flag = false;
								break;
							}else{
								if(countsix == 4){
									identifytemp[0] = 0;
									identifytemp[2] = 0;
									countsix = 0;
									cout << "LZero!!\n";
								}
							}
						}
							
					}else{
						cout << "nodata\n" ;
					}			
				}
				//right
				while(1){
					if(serR.available() > 0){
						//ROS_INFO_STREAM("Reading from serial port");
						rmotorcmd.data.clear();
						serR.read(rmotorcmd.data,1);	//read 2 unsigned char
						rconcount++;
						if(rmotorcmd.data[0] == 0x06){						
							ridentifytemp[rcountsix] = rmotorcmd.data[0]; 
							ridentifytemp[rcountsix+1] = rconcount;
							rcountsix+=2;
							//cout << "0xe6? "<< (int)(unsigned int)ridentifytemp[0] <<", "<< ridentifytemp[2] <<", "<<(ridentifytemp[3] - ridentifytemp[1]) << "\n";
							if((ridentifytemp[2] == ridentifytemp[0]) && (ridentifytemp[3] - ridentifytemp[1]) == 1){
								rcountsix = 0;
								rconcount = 0;
								ridentifytemp[0] = 0;
								ridentifytemp[2] = 0;
								break;
							}else{
								if(rcountsix == 4){
									ridentifytemp[0] = 0;
									ridentifytemp[2] = 0;
									rcountsix = 0;
									cout << "RZero!!\n";
								}
							}
							
						}else{
							cout << "nodata\n" ;
						}			
					speedset_flag = false;		
				}				
			}
		}
		//Compute Odometry
		/////////////////////////////////////////
		current_time = ros::Time::now();
		//cout << "Multiple:" << fourthmultiple << " ;" << thirdmultiple << " ;" << secondmultiple << "\n";
		
		//left
		if ((~lencoder[0]+1) < 0){
			encoderPulse[0] = lencoder[0] * fourthmultiple + lencoder[1] * thirdmultiple +lencoder[2] * secondmultiple + lencoder[3] - 4294967296;
		}else{
			encoderPulse[0] = lencoder[0] * fourthmultiple + lencoder[1] * thirdmultiple +lencoder[2] * secondmultiple + lencoder[3];
		}
		//right
		if ((~rencoder[0]+1) < 0){
			encoderPulse[1] = rencoder[0] * fourthmultiple + rencoder[1] * thirdmultiple +rencoder[2] * secondmultiple + rencoder[3] - 4294967296;
		}else{
			encoderPulse[1] = rencoder[0] * fourthmultiple + rencoder[1] * thirdmultiple +rencoder[2] * secondmultiple + rencoder[3];
		}
/*
		if(initialenc == true){
			initialenc = false;
			
			tencoderPulse[0] = encoderPulse[0];
			tencoderPulse[1] = encoderPulse[1];
			cout << "InitialEncoder\n";
		}	
*/
		
		tencoderPulse[0] = encoderPulse[0];
		tencoderPulse[1] = encoderPulse[1];
		
		encoderPulse[0] = encoderPulse[0] - tencoderPulse[0];
		encoderPulse[1] = encoderPulse[1] - tencoderPulse[1];
		
		

		cout << "LencoderPulse : " << encoderPulse[0] << ", RencoderPulse : " << encoderPulse[1] << "\n\n";
		cout << "TempLenc : " << tencoderPulse[0] << ", TempRenc : " << tencoderPulse[1] << "\n\n";

		cout << "LencoderPulse : " << encoderPulse[0] << ", RencoderPulse : " << encoderPulse[1] << "\n\n";
		
		//Odometry
		cout << "Odometry \n";
		double Odometry[3] = {0.0};	//x,y,theta (m,m,rad) -pi~pi
		double WV[2] = { 0.0, 0.0 };
		WV[0] = ((double)encoderPulse[0] / EncoderPlus) * (2.0 * M_PI * WheelRadius) / GearRatio / DriverSampleTime;
		WV[1] = ((double)encoderPulse[1] / EncoderPlus) * (2.0 * M_PI * WheelRadius) / GearRatio / DriverSampleTime;
		
		double LinearVelocity = (WV[1] + WV[0]) / 2.0;
		double AngularVelocity = (WV[1] - WV[0]) / WheelDistance;

		double deltatheta = (WV[1] - WV[0]) * DriverSampleTime / WheelDistance;
		double deltas = (WV[1] + WV[0]) * DriverSampleTime / 2.0;

		double thetak = Odometry[2];	//theta of last time
		Odometry[2] = (Odometry[2] + deltatheta) *0.845;
		Odometry[0] = (Odometry[0] + deltas * cos((Odometry[2] + thetak) / 2.0)) * 0.864;
		Odometry[1] = Odometry[1] + deltas * sin((Odometry[2] + thetak) / 2.0);
		if (Odometry[2] > M_PI)
			Odometry[2] -= (2.0 * M_PI);
		else if (Odometry[2] < -M_PI)
			Odometry[2] += (2.0 * M_PI);

		ROS_INFO_STREAM("odom x: " << Odometry[0] << " odom y: " << Odometry[1] << " odom theta: " << Odometry[2] * 180.0 / M_PI);
		//since all odometry is 6DOF we'll need a quaternion created from yaw
		geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(Odometry[2]);

			
		//first, we'll publish the transform over tf
		geometry_msgs::TransformStamped odom_trans;
		odom_trans.header.stamp = current_time;
		odom_trans.header.frame_id = "odom";
		odom_trans.child_frame_id = "base_link";

		odom_trans.transform.translation.x = Odometry[0];
		odom_trans.transform.translation.y = Odometry[1];
		odom_trans.transform.translation.z = 0.0;
		odom_trans.transform.rotation = odom_quat;

		//send the transform
		odom_broadcaster.sendTransform(odom_trans);
			

		//next, we'll publish the odometry message over ROS
		nav_msgs::Odometry odom;
		odom.header.stamp = current_time;
		odom.header.frame_id = "odom";

		//set the position
		odom.pose.pose.position.x = Odometry[0];
		odom.pose.pose.position.y = Odometry[1];
		odom.pose.pose.position.z = 0.0;
		odom.pose.pose.orientation = odom_quat;

		//set the velocity
		odom.child_frame_id = "base_link";
		odom.twist.twist.linear.x = LinearVelocity;
		odom.twist.twist.linear.y = 0.0;
		odom.twist.twist.angular.z = AngularVelocity;

		//publish the message
		odom_pub.publish(odom);
		

		loop_rate.sleep();
		ros::spinOnce();

	}
	serL.close();
	serR.close();
}	


void initial(){
	ros::NodeHandle para_node("~");
	if(!para_node.getParam("Lport", Lport))
    		Lport = "/dev/ttyUSB0";
	if(!para_node.getParam("baudrate", baudrate))
    		baudrate = 57600;
  if(!para_node.getParam("Rport", Rport))
    		Rport = "/dev/ttyUSB1";
}

void driverinitial(){
	std_msgs::UInt8MultiArray array;
	//stop motor
	///////////////////////
	array.data.clear();
	array.data.push_back(0x00);	
	array.data.push_back(0x00);
	array.data.push_back(0x00);
	array.data.push_back(0x00);
	//array.data.push_back(0x2F);  //Choose speed mode
	serL.write(array.data);
	serR.write(array.data);
	array.data.clear();
	ros::Duration(0.1).sleep();
	
	//Read then identify
	///////////////////
	for(int i = 0; i<50; i++){
		if(serL.available()>=2){
				//ROS_INFO_STREAM("Reading from serial port");
				array.data.clear();
				serL.read(array.data,2);	//read 4 unsigned char
				serR.read(array.data,2);	//read 4 unsigned char
				break;
		}		
	}
	std::cout<<"ReceiveR="<<(int)(unsigned int)array.data[0]<<" ;"<< (int)(unsigned int)array.data[0] << "\n";
	
	//change motor speed mode
	///////////////////////
	array.data.clear();
	array.data.push_back(0x02);	
	array.data.push_back(0x00);
	array.data.push_back(0xc4);
	array.data.push_back(0xc6);
	serL.write(array.data);
	serR.write(array.data);
	ros::Duration(0.1).sleep();

	for(int i = 0; i<50; i++){
		if(serL.available()>=2){
				//ROS_INFO_STREAM("Reading from serial port");
				array.data.clear();
				serL.read(array.data,2);	//read 4 unsigned char
				serR.read(array.data,2);	//read 4 unsigned char
				break;
		}		
	}
	std::cout<<"ReceiveR="<<(int)(unsigned int)array.data[0]<<" ;"<< (int)(unsigned int)array.data[1] << "\n";
			
	//motor speed setup
	///////////////////////
	lmotorcmd.data.clear();
	lmotorcmd.data.push_back(0x06);	
	lmotorcmd.data.push_back(0x00);
	lmotorcmd.data.push_back(0x00);
	lmotorcmd.data.push_back(0x06);
	serL.write(lmotorcmd.data);
	serR.write(lmotorcmd.data);
	ros::Duration(0.1).sleep();
	for(int i = 0; i<50; i++){
		if(serL.available()>=2){
				//ROS_INFO_STREAM("Reading from serial port");
				array.data.clear();
				serL.read(array.data,2);	//read 4 unsigned char
				serR.read(array.data,2);	//read 4 unsigned char
				break;
		}		
	}
	std::cout<<"ReceiveR="<<(int)(unsigned int)array.data[0]<<" ;"<< (int)(unsigned int)array.data[1] << "\n";

	
	//Run
	///////////////////////
	array.data.clear();
	array.data.push_back(0x00);	
	array.data.push_back(0x00);
	array.data.push_back(0x01);
	array.data.push_back(0x01);
	serL.write(array.data);
	serR.write(array.data);
	array.data.clear();
	ros::Duration(0.1).sleep();
	for(int i = 0; i<50; i++){
		if(serL.available()>=2){
				//ROS_INFO_STREAM("Reading from serial port");
				array.data.clear();
				serL.read(array.data,2);	//read 4 unsigned char
				serR.read(array.data,2);	//read 4 unsigned char
				break;
		}		
	}
	std::cout<<"ReceiveR="<<(int)(unsigned int)array.data[0]<<" ;"<< (int)(unsigned int)array.data[1] << "\n";
}

void write_serial(const geometry_msgs::Twist::ConstPtr& speed)
{
	//m/s
	double vr = (speed->linear.x * 2.0 + speed->angular.z * WheelDistance) / 2.0;
	double vl = (speed->linear.x * 2.0 - speed->angular.z * WheelDistance) / 2.0;
	//rpm
	int leftmotorvalue = (int)(round(vl / (2.0 * M_PI *  WheelRadius) * 60.0));
	int rightmotorvalue = (int)(round(vr / (2.0 * M_PI *  WheelRadius) * 60.0));
	//Max rpm
	if (leftmotorvalue > 250)
		leftmotorvalue = 250;
	else if (leftmotorvalue < -250)
		leftmotorvalue = -250;

	if (rightmotorvalue > 250)
		rightmotorvalue = 250;
	else if (rightmotorvalue < -250)
		rightmotorvalue = -250;
		
	//Change driver speed type
	// real rpm = (cmd / 16384) * 6000;
	int motorLcmd = (int)(round(leftmotorvalue * 16384.0 / 6000.0));
	int motorRcmd = (int)(round(rightmotorvalue * 16384.0 / 6000.0));
	
	
	std::cout<<"RMotorRPM = "<< rightmotorvalue <<" ;LMotorRPM = "<< leftmotorvalue << " ;" << " ;motorRcmd = "<< motorRcmd << " ;motorLcmd = "<< motorLcmd << "\n";
	
	//MotorCmd[1, 0]:left,  MotorCmd[3, 2]:right
	//////////////////////////////////////////////////////
	unsigned int MotorCmd[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	//left
	MotorCmd[1] = motorLcmd&0XFF;	//low 8
  MotorCmd[0] = motorLcmd>>8;	//high 8
	MotorCmd[2] = 6 + MotorCmd[1] + MotorCmd[0];	//checksum
	MotorCmd[2] = MotorCmd[2]&0XFF;	//checksum low 8

	//right
	MotorCmd[4] = motorRcmd&0XFF;	//low 8
  MotorCmd[3] = motorRcmd>>8;	//high 8
	MotorCmd[5] = 6 + MotorCmd[4] + MotorCmd[3];	//checksum
	MotorCmd[5] = MotorCmd[5]&0XFF;	//checksum low 8
	
	/*
  //display 8bit high and low
  printf("left_high = %d\n", MotorCmd[0]);
  printf("left_low = %d\n", MotorCmd[1]);
  printf("checksum=%d\n", MotorCmd[2]);
	*/
	
	//Left motor speed setup
	///////////////////////
	lmotorcmd.data.clear();
	lmotorcmd.data.push_back(0x06);	
	lmotorcmd.data.push_back(MotorCmd[0]);
	lmotorcmd.data.push_back(MotorCmd[1]);
	lmotorcmd.data.push_back(MotorCmd[2]);
	
	//Right motor speed setup
	///////////////////////
	rmotorcmd.data.clear();
	rmotorcmd.data.push_back(0x06);	
	rmotorcmd.data.push_back(MotorCmd[3]);
	rmotorcmd.data.push_back(MotorCmd[4]);
	rmotorcmd.data.push_back(MotorCmd[5]);
	speedset_flag = true;

}

