#include <ros/ros.h>
//#include <serial/serial.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/UInt8MultiArray.h>
#include <math.h>
#include <vector>
#include "MySerial.c"

using namespace std;

const double WheelDistance = 0.235;		//m //0.224
const double WheelRadius = 0.099;		//m
const double EncoderPlus = 1024.0 * 4.0; //4 pole
const double GearRatio = 1.0;
const double DriverSampleTime = 0.01;		//s
const unsigned long fourthmultiple = pow(2, 24);
const unsigned int thirdmultiple = pow(2, 16);
const unsigned long secondmultiple = pow(2, 8);

//serial parameter
/////////////////////////
//serial::Serial ser;
//serial::Serial serR;
std::string Lport, Rport;
int baudrate;

bool writing = false;
bool speedset_flag = false;

//Motor Cmd
std_msgs::UInt8MultiArray motorcmd;
//std_msgs::UInt8MultiArray rmotorcmd;

void initial();
int driverinitial();
void write_serial(const geometry_msgs::Twist::ConstPtr& speed);


int main (int argc, char** argv)
{
    
	ros::init(argc, argv, "serial_driver_node");
	ros::NodeHandle nh;

	ros::Subscriber cmd_sub = nh.subscribe("cmd_vel", 1000, write_serial);
	ros::Publisher odom_pub = nh.advertise<nav_msgs::Odometry>("odom", 1000);
	tf::TransformBroadcaster odom_broadcaster;
	driverinitial();
		
	std_msgs::UInt8MultiArray array;
	bool Error[4] = {false, false, false, false};

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
	ROS_INFO_STREAM("Test");
	while(ros::ok())
	{
		ros::spinOnce();
	}
/*
	while(ros::ok()){
	
		//Read Encoder
		array.data.clear();
		array.data.push_back(0x80);
		array.data.push_back(0x00);	
		array.data.push_back(0x80);
		
		serL.write(array.data);
		ros::Duration(0.01).sleep();
		serR.write(array.data);		
		ros::Duration(0.01).sleep();
		Lsave[0] = 0;
		Lsave[1] = 0;	
		Rsave[0] = 0;
		Rsave[1] = 0;	
		
		//Receive Left Encoder
		lencoder.clear();
		while(1){
			if(serL.available()>=1){
				array.data.clear();
				serL.read(array.data,1);	//read 1 unsigned char
				//usleep(1000);
				//cout<<"LeftData = "<< (int)array.data[0] << "\n";
				if(array.data[0] == 228  && lencoder[0] == 232){
					lencoder.clear();
					Lsave[0] = 0;
					continue;
				}
				if(array.data[0] == 232 || Lsave[0] > 0){
					
					Lsave[0] ++;
					lencoder.push_back(array.data[0]);
					
					if(Lsave[0] == 4){
						//cout<<"DataL = "<< (int)lencoder[0]<<" ;"<< (int)lencoder[1] << " ;" << (int)lencoder[2]<< " ;"<< (int)lencoder[3] << "\n";
						long int Ltotal_high = lencoder[0] + lencoder[1] + lencoder[2];
						Ltotal_high = Ltotal_high & 0XFF;	//low 8
						if(Ltotal_high == lencoder[3]){
							//cout<<"LeftEncoder_H = "<< (int)lencoder[0]<<" ;"<< (int)lencoder[1] << " ;" << (int)lencoder[2]<< " ;"<< (int)lencoder[3] << "\n";
							Lsave[0] = 0;
							//cout << "HCorrect\n";
							Error[0] = false;
							break;
						}else{
							//cout << "HError\n";
							Error[0] = true;
							Lsave[0] = -1;
							break;
						}								
					}
				}
			}else{
				break;
			}
		}
		while(1){
			if(serL.available()>=1){
				array.data.clear();
				serL.read(array.data,1);	//read 1 unsigned char
				if(array.data[0] == 233 || Lsave[1] > 0){
					Lsave[1] ++;
					lencoder.push_back(array.data[0]);
					if(Lsave[1] == 4){
						long int Ltotal_low = lencoder[4] + lencoder[5] + lencoder[6];
						Ltotal_low = Ltotal_low & 0XFF;	//low 8
						if(Ltotal_low == lencoder[7]){
							//cout<<"LeftEncoder_L = "<< (int)lencoder[4]<<" ;"<< (int)lencoder[5] << " ;" << (int)lencoder[6]<< " ;"<< (int)lencoder[7] << "\n";
							//cout << "LCorrect\n";
							Error[1] = false;
							Lsave[1] = 0;
							break;
						}else{
							//cout << "LError\n";
							Error[1] = true;
							Lsave[1] = -1;
							break;
						}
						
					}
				}
			}else{
				break;
			}
		}
		
		//Receive right Encoder
		rencoder.clear();
		while(1){
			if(serR.available()>=1){
				array.data.clear();
				serR.read(array.data,1);	//read 1 unsigned char
				
				if(array.data[0] == 228 && rencoder[0] == 232){
					rencoder.clear();
					Rsave[0] = 0;
					continue;
				}
				if(array.data[0] == 232 || Rsave[0] > 0){
					
					Rsave[0] ++;
					rencoder.push_back(array.data[0]);
					
					if(Rsave[0] == 4){
						//cout<<"DataR = "<< (int)rencoder[0]<<" ;"<< (int)rencoder[1] << " ;" << (int)rencoder[2]<< " ;"<< (int)rencoder[3] << "\n";
						long int Rtotal_high = rencoder[0] + rencoder[1] + rencoder[2];
						Rtotal_high = Rtotal_high & 0XFF;	//low 8
						if(Rtotal_high == rencoder[3]){
							//cout<<"RightEncoder_H = "<< (int)rencoder[0]<<" ;"<< (int)rencoder[1] << " ;" << (int)rencoder[2]<< " ;"<< (int)rencoder[3] << "\n";
							Rsave[0] = 0;
							//cout << "HCorrect\n";
							Error[2] = false;
							break;
						}else{
							//cout << "HError\n";
							Error[2] = true;
							Rsave[0] = -1;
							break;
						}								
					}
				}
			}else{
				break;
			}
		}
		while(1){
			if(serR.available()>=1){
				array.data.clear();
				serR.read(array.data,1);	//read 1 unsigned char
				if(array.data[0] == 233 || Rsave[1] > 0){
					Rsave[1] ++;
					rencoder.push_back(array.data[0]);
					if(Rsave[1] == 4){
						long int Rtotal_low = rencoder[4] + rencoder[5] + rencoder[6];
						Rtotal_low = Rtotal_low & 0XFF;	//low 8
						if(Rtotal_low == rencoder[7]){
							//cout<<"RightEncoder_L = "<< (int)rencoder[4]<<" ;"<< (int)rencoder[5] << " ;" << (int)rencoder[6]<< " ;"<< (int)rencoder[7] << "\n";
							//cout << "LCorrect\n";
							Error[3] = false;
							Rsave[1] = 0;
							break;
						}else{
							//cout << "LError\n";
							Error[3] = true;
							Rsave[1] = -1;
							break;
						}
						
					}
				}
			}else{
				break;
			}
		}

		current_time = ros::Time::now();
		//Compute Encoder to decimal
		/////////////////////////////////////////////////////////////
		if(Error[0] == false && Error[1] == false && Error[2] == false && Error[3] == false){
			//left encoder(Dec)
			if ((~lencoder[1]+1) < 0){
				encoderPulse[0] = lencoder[1] * fourthmultiple + lencoder[2] * thirdmultiple +lencoder[5] * secondmultiple + lencoder[6] - 4294967296;
			}else{
				encoderPulse[0] = lencoder[1] * fourthmultiple + lencoder[2] * thirdmultiple +lencoder[5] * secondmultiple + lencoder[6];
			}
			//right encoder(Dec)
			if ((~rencoder[1]+1) < 0){
				encoderPulse[1] = rencoder[1] * fourthmultiple + rencoder[2] * thirdmultiple +rencoder[5] * secondmultiple + rencoder[6] - 4294967296;
			}else{
				encoderPulse[1] = rencoder[1] * fourthmultiple + rencoder[2] * thirdmultiple +rencoder[5] * secondmultiple + rencoder[6];
			}
		}
		//Display Encoder(Dec)
		//cout << "LencoderPulse : " << encoderPulse[0] << ", RencoderPulse : " << encoderPulse[1] << "\n\n";
		
		d_encoderPulse[0] = encoderPulse[0] - tencoderPulse[0];
		d_encoderPulse[1] = encoderPulse[1] - tencoderPulse[1];
		tencoderPulse[0] = encoderPulse[0];
		tencoderPulse[1] = encoderPulse[1];
		//Odometry
		////////////////////////////////////////////////////////////
		//cout << "OdometryResult : \n";
		
		WV[0] = ((double)d_encoderPulse[0] / EncoderPlus) * 0.841 *(2.0 * M_PI * WheelRadius) / GearRatio / DriverSampleTime;
		WV[1] = ((double)d_encoderPulse[1] / EncoderPlus) * 0.841 *(2.0 * M_PI * WheelRadius) / GearRatio / DriverSampleTime;
		
		double LinearVelocity = (WV[1] + WV[0]) / 2.0;
		double AngularVelocity = (WV[1] - WV[0]) / WheelDistance;

		double deltatheta = (WV[1] - WV[0]) * DriverSampleTime / WheelDistance;
		double deltas = (WV[1] + WV[0]) * DriverSampleTime / 2.0;

		double thetak = Odometry[2];	//theta of last time
		Odometry[2] = Odometry[2] + deltatheta;
		Odometry[0] = Odometry[0] + deltas * cos((Odometry[2] + thetak) / 2.0);
		Odometry[1] = Odometry[1] + deltas * sin((Odometry[2] + thetak) / 2.0);
		if (Odometry[2] > M_PI)
			Odometry[2] -= (2.0 * M_PI);
		else if (Odometry[2] < -M_PI)
			Odometry[2] += (2.0 * M_PI);

		//ROS_INFO_STREAM("odom x: " << Odometry[0] << " odom y: " << Odometry[1] << " odom theta: " << Odometry[2] * 180.0 / M_PI);
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
	*/
}	

int driverinitial(){
	std_msgs::UInt8MultiArray array;
	//stop motor
	int Status = Serial_Write(0x00,0x00,0x00,0x00);
	ros::Duration(1).sleep();
	return Status;
	//std::cout<<"ReceiveR="<<(int)(unsigned int)array.data[0]<<" ;"<< (int)(unsigned int)array.data[1] << "\n";
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
 
	int result = Serial_Write(motorLcmd,0x00,motorRcmd,0x00);
  	ROS_INFO("Status: %ld moto %ld %ld",(long int)result,(long int)motorLcmd,(long int)motorRcmd);
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

