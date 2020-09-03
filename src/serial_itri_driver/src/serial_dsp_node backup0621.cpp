#include <ros/ros.h>
#include <serial/serial.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/MultiArrayLayout.h>
#include <std_msgs/MultiArrayDimension.h>
#include <std_msgs/UInt8MultiArray.h>
#include <math.h>

serial::Serial ser;

const double WheelDistance = 0.44;		//m
const double WheelRadius = 0.075;		//m
const double EncoderPlus = 500.0 * 4;
const double GearRatio = 14.0;
const double DSPSampleTime = 0.05;		//s

std::string port;
int baudrate;

void initial();
void write_serial(const geometry_msgs::Twist::ConstPtr& speed);

int main (int argc, char** argv)
{
    
	ros::init(argc, argv, "serial_dsp_node");
	ros::NodeHandle nh;

	ros::Subscriber cmd_sub = nh.subscribe("cmd_vel", 1000, write_serial);
	ros::Publisher odom_pub = nh.advertise<nav_msgs::Odometry>("odom", 1000);
	tf::TransformBroadcaster odom_broadcaster;

	initial();
	//connect to serial
	try{
		ser.setPort(port);
		ser.setBaudrate(baudrate);
		serial::Timeout to = serial::Timeout::simpleTimeout(1000);
		ser.setTimeout(to);
		ser.open();
	}
	catch (serial::IOException& e){
		ROS_ERROR_STREAM("Unable to open port ");
		return -1;
	}
	//open or not
	if(ser.isOpen()){
		ROS_INFO_STREAM("Serial Port initialized");
	}
	else{
		return -1;
	}

	ros::Rate loop_rate(20);

	std_msgs::UInt8MultiArray array;

	//initial dsp board command

	array.data.clear();
	array.data.push_back(0x0a);	//stop send encoder
	ser.write(array.data);
	array.data.clear();
	array.data.push_back(0x07);	//vr,vl = 0
	array.data.push_back(0x00);
	array.data.push_back(0x00);
	ser.write(array.data);
	array.data.clear();
	array.data.push_back(0x09);	//start send encoder
	ser.write(array.data);

	double Odometry[3] = {0.0};	//x,y,theta (m,m,rad) -pi~pi
	ros::Time current_time;
	while(ros::ok()){
		ros::spinOnce();
		current_time = ros::Time::now();

		if(ser.available()>=4){
			//ROS_INFO_STREAM("Reading from serial port");
			array.data.clear();
			ser.read(array.data,4);	//read 4 unsigned char
			
			//decode encoder data
			int WPluse[2] = { 0, 0 };
			double WV[2] = { 0.0, 0.0 };
			if (array.data[0] & 0x80)
				WPluse[0] = (int)(((unsigned int)array.data[0]) * 256 + (unsigned int)array.data[1] - 65536);
			else
				WPluse[0] = (int)(((unsigned int)array.data[0]) * 256 + (unsigned int)array.data[1]);

			if (array.data[2] & 0x80)
				WPluse[1] = (int)(((unsigned int)array.data[2]) * 256 + (unsigned int)array.data[3] - 65536);
			else
				WPluse[1] = (int)(((unsigned int)array.data[2]) * 256 + (unsigned int)array.data[3]);

			WV[0] = ((double)WPluse[0] / EncoderPlus) * (2.0 * M_PI * WheelRadius) / GearRatio / DSPSampleTime;
			WV[1] = ((double)WPluse[1] / EncoderPlus) * (2.0 * M_PI * WheelRadius) / GearRatio / DSPSampleTime;

			//ROS_INFO_STREAM("Wheel 1: " << WV[0] << " Wheel 2: " << WV[1] );


			double LinearVelocity = (WV[1] + WV[0]) / 2.0;
			double AngularVelocity = (WV[1] - WV[0]) / WheelDistance;


			double deltatheta = (WV[1] - WV[0]) * DSPSampleTime / WheelDistance * 1.036;
			double deltas = (WV[1] + WV[0]) * DSPSampleTime / 2.0 * 1.036;

			double thetak = Odometry[2];	//theta of last time
			Odometry[2] = Odometry[2] + deltatheta;
			Odometry[0] = Odometry[0] + deltas * cos((Odometry[2] + thetak) / 2.0);
			Odometry[1] = Odometry[1] + deltas * sin((Odometry[2] + thetak) / 2.0);
			if (Odometry[2] > M_PI)
				Odometry[2] -= (2.0 * M_PI);
			else if (Odometry[2] < -M_PI)
				Odometry[2] += (2.0 * M_PI);

			ROS_INFO_STREAM("odom x: " << Odometry[0] << " odom y: " << Odometry[1] << " odom theta: " << Odometry[2]);

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
		}
		loop_rate.sleep();
	}
}

void initial()
{
	ros::NodeHandle para_node("~");
	if(!para_node.getParam("port", port))
    		port = "/dev/ttyS0";
	if(!para_node.getParam("baudrate", baudrate))
    		baudrate = 19200;
}

void write_serial(const geometry_msgs::Twist::ConstPtr& speed)
{
	double vr = (speed->linear.x * 2.0 + speed->angular.z * WheelDistance) / 2.0;
	double vl = (speed->linear.x * 2.0 - speed->angular.z * WheelDistance) / 2.0;

	int leftmotorvalue = (int)(round(vl * EncoderPlus / 1000.0 * GearRatio / (2.0 * M_PI) / WheelRadius));	//總pluse=2000; 命令1 = 每秒轉1000pluse
	int rightmotorvalue = (int)(round(vr * EncoderPlus / 1000.0 * GearRatio / (2.0 * M_PI) / WheelRadius));

	if (leftmotorvalue > 127)
		leftmotorvalue = 127;
	else if (leftmotorvalue < -128)
		leftmotorvalue = -128;

	if (rightmotorvalue > 127)
		rightmotorvalue = 127;
	else if (rightmotorvalue < -128)
		rightmotorvalue = -128;

	unsigned char MotorCmd[3] = { 0x07, 0x00, 0x00 };
	MotorCmd[2] = (unsigned char)leftmotorvalue;
	MotorCmd[1] = (unsigned char)rightmotorvalue;

	//ROS_INFO_STREAM("Writing to serial port ...");
	ser.write(MotorCmd, 3);
}

