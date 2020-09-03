#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
int main(int argc, char **argv)
{
    ros::init(argc, argv, "Human_follow");
	ros::NodeHandle nh;
	//Car move control
	ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
	ros::Rate loop_rate(100);
	//
	while (ros::ok())
    {
		geometry_msgs::Twist cmd;
		cmd.linear.x = 0.1;
		cmd.angular.z = -0.5;
		pub.publish(cmd);
        ros::spinOnce();
		loop_rate.sleep();
    }
    return 0;
}