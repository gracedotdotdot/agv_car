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
const double WheelDistance = 0.204;		//m //0.278
const double DriverSampleTime = 0.081;		//s
const double Linear_Velocity_Kp = 0.1 / DriverSampleTime;
//
geometry_msgs::Twist Encoder;

//void write_serial(const geometry_msgs::Twist::ConstPtr& speed);
void Read_Encoder(const geometry_msgs::Twist::ConstPtr& speed)
{
		Encoder.linear.x = speed->linear.x * 0.784;
		Encoder.linear.y = 0;
		Encoder.linear.z = 0;
		Encoder.angular.x = 0;
		Encoder.angular.y = 0;
		Encoder.angular.z = speed->angular.z;
}

int main (int argc, char** argv)
{
	ros::init(argc, argv, "serial_driver_node");
	ros::NodeHandle nh;
	ros::Subscriber talker1_sub = nh.subscribe("encoder_twist", 1, Read_Encoder);
	//ros::Subscriber cmd_sub = nh.subscribe("cmd_vel", 1000, write_serial);
	ros::Publisher odom_pub = nh.advertise<nav_msgs::Odometry>("odom", 1000);
	tf::TransformBroadcaster odom_broadcaster;
	ros::Rate loop_rate(8.0);
	ros::Time current_time, last_time;
 	current_time = ros::Time::now();
	last_time = ros::Time::now();

	//Odometry
	double Odometry[3] = {0.0};	//x,y,theta (m,m,rad) -pi~pi
	double WV[2] = { 0.0, 0.0 };
	while(ros::ok()){
		ros::spinOnce();
		current_time = ros::Time::now();
		double dt = (current_time - last_time).toSec();
		//WV[1]=((2.0*Encoder.linear.x)+(Encoder.angular.z*WheelDistance))  / 2.0;
		//WV[0]=((2.0*Encoder.linear.x)-(Encoder.angular.z*WheelDistance)) / 2.0;
        
        double deltas = Encoder.linear.x * DriverSampleTime; // S = vt
		double deltatheta = Encoder.angular.z * DriverSampleTime ; // theta = wt

        //ROS_INFO("%e %e ",deltatheta,deltas);
		//double deltatheta = (WV[1] - WV[0]) * DriverSampleTime / WheelDistance;
		//double deltas = (WV[1] + WV[0]) * DriverSampleTime / 2.0;

		//double thetak = Odometry[2];	//theta of last time
		Odometry[2] += deltatheta;//

		//Odometry[0] = Odometry[0] + deltas * cos((Odometry[2] + thetak) / 2.0);
		//Odometry[1] = Odometry[1] + deltas * sin((Odometry[2] + thetak) / 2.0);
		WV[0] = WV[0] + deltas; //S
		WV[1] = WV[1] + deltatheta; //{A

		//Odometry[0] = Odometry[0] + deltas * cos(Odometry[0]+deltatheta);//x
		//Odometry[1] = Odometry[1] + deltas * sin(Odometry[0]+deltatheta);//y
		Odometry[0]=WV[0]*cos(WV[1]);
		Odometry[1]=WV[0]*sin(WV[1]);

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
 
		//Conversation
		double ODOM_POSE_COVARIANCE[36] = {1e-3, 0, 0, 0, 0, 0, 
                        0, 1e-3, 0, 0, 0, 0,
                        0, 0, 1e6, 0, 0, 0,
                        0, 0, 0, 1e6, 0, 0,
                        0, 0, 0, 0, 1e6, 0,
                        0, 0, 0, 0, 0, 1e3};

		double ODOM_POSE_COVARIANCE2[36] = {1e-9, 0, 0, 0, 0, 0, 
                         0, 1e-3, 1e-9, 0, 0, 0,
                         0, 0, 1e6, 0, 0, 0,
                         0, 0, 0, 1e6, 0, 0,
                         0, 0, 0, 0, 1e6, 0,
                         0, 0, 0, 0, 0, 1e-9};
 
		double ODOM_TWIST_COVARIANCE[36] = {1e-3, 0, 0, 0, 0, 0, 
                         0, 1e-3, 0, 0, 0, 0,
                         0, 0, 1e6, 0, 0, 0,
                         0, 0, 0, 1e6, 0, 0,
                         0, 0, 0, 0, 1e6, 0,
                         0, 0, 0, 0, 0, 1e3};

		double ODOM_TWIST_COVARIANCE2[36] = {1e-9, 0, 0, 0, 0, 0, 
                          0, 1e-3, 1e-9, 0, 0, 0,
                          0, 0, 1e6, 0, 0, 0,
                          0, 0, 0, 1e6, 0, 0,
                          0, 0, 0, 0, 1e6, 0,
                          0, 0, 0, 0, 0, 1e-9};
    
		//send the transform
		// odom_broadcaster.sendTransform(odom_trans);
			

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
		odom.twist.twist.linear.x = Encoder.linear.x;
		odom.twist.twist.linear.y = 0.0;
		odom.twist.twist.angular.z = Encoder.angular.z;
		
        if(Encoder.linear.x == 0)
		{
			for(int Pose_Covariance_Index = 0; Pose_Covariance_Index < 36; Pose_Covariance_Index++)
			{
				odom.pose.covariance[Pose_Covariance_Index] = ODOM_POSE_COVARIANCE2[Pose_Covariance_Index];
				odom.twist.covariance[Pose_Covariance_Index] = ODOM_TWIST_COVARIANCE2[Pose_Covariance_Index];
			}
			}
		else
		{
			for(int Pose_Covariance_Index = 0; Pose_Covariance_Index < 36; Pose_Covariance_Index++)
			{
				odom.pose.covariance[Pose_Covariance_Index] = ODOM_POSE_COVARIANCE[Pose_Covariance_Index];
				odom.twist.covariance[Pose_Covariance_Index] = ODOM_TWIST_COVARIANCE[Pose_Covariance_Index];
			}
			//memcpy(odom.pose.covariance,ODOM_POSE_COVARIANCE,sizeof(double)*36);
			//memcpy(odom.twist.covariance,ODOM_TWIST_COVARIANCE,sizeof(double)*36);
			//odom.pose.covariance = ODOM_POSE_COVARIANCE
            //odom.twist.covariance = ODOM_TWIST_COVARIANCE
		}
	//publish the message
		odom_pub.publish(odom);

		loop_rate.sleep();

		last_time = current_time;

	}

}	


