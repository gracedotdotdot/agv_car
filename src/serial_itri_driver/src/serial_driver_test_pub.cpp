#include "ros/ros.h"
#include <geometry_msgs/Twist.h>

int main(int argc, char **argv)
{

	ros::init(argc, argv, "serial_dsp_pub");

	ros::NodeHandle n;

	ros::Publisher pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

	ros::Rate loop_rate(20);

	double v = 0.1;		//cm
  	double w = 0.0;		//rad
	while (ros::ok())
	{
		geometry_msgs::Twist cmd;
		ROS_INFO("Enter v(cm/s) & w(rad/s):");
		//scanf("%lf %lf",&v,&w);
		ROS_INFO("x = %.3lf y = %.3lf",v,w);
		
		cmd.linear.x = v ;
		cmd.angular.z = w;
		pub.publish(cmd);

		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
