#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <std_msgs/Bool.h>
#include <math.h>
#include <stdio.h>
#include <termios.h>

#define KEYCODE_R 0x43
#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_Q 0x71

const double MaxSpeed = 2.0;	//m
const double MaxRotation = 50.0 / 180.0 * M_PI;
bool start_or_not_ = false ;
//non-blocking getch()
int getch()
{
	static struct termios oldt, newt;
	tcgetattr( STDIN_FILENO, &oldt);           // save old settings
	newt = oldt;
	newt.c_lflag &= ~(ICANON);                 // disable buffering
	tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings

	int c = getchar();  // read character (non-blocking)

	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings
	return c;
}
void check(const std_msgs::Bool msg){
	
	start_or_not_ = msg.data;
}
int main(int argc, char **argv)
{

	ros::init(argc, argv, "serial_dsp_keyboard");

	ros::NodeHandle n;

	ros::Publisher pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
	ros::Rate loop_rate(50);
	geometry_msgs::Twist cmd;
	while (ros::ok())
	{
		//======switch========//
		
		unsigned char key=getch();
		switch(key) {
			case KEYCODE_U: // UP
				cmd.linear.x += 0.10;
				break;
			case KEYCODE_D: // DOWN
				cmd.linear.x -= 0.10;
				break;
			case KEYCODE_R: // Right
				cmd.angular.z -= 10.0 / 180.0 * M_PI;
				break;
			case KEYCODE_L: // Left
				cmd.angular.z += 10.0 / 180.0 * M_PI;
				break;
			case 'r': //rotate up
				cmd.linear.x = 0.0;
				cmd.angular.z += 20.0 / 180.0 * M_PI;
				break;
			case 't': //rotate down
				cmd.linear.x = 0.0;
				cmd.angular.z -= 20.0 / 180.0 * M_PI;
				break;
			case 'p': // Stop
				cmd.linear.x = 0.0;
				cmd.angular.z = 0.0;
				break;
		}
		if(cmd.linear.x >= MaxSpeed)
			cmd.linear.x = MaxSpeed;
		else if(cmd.linear.x <= -MaxSpeed)
			cmd.linear.x = -MaxSpeed;

		if(cmd.angular.z >= MaxRotation)
			cmd.angular.z = MaxRotation;
		else if(cmd.angular.z <= -MaxRotation)
			cmd.angular.z = -MaxRotation;

		ROS_INFO_STREAM(" v = " << cmd.linear.x << " z = " << cmd.angular.z);
		pub.publish(cmd);
		
		//=====================//
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
