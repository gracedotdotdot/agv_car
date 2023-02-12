#include "ros/ros.h"
#include <std_msgs/Int32.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseGoal.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
MoveBaseClient *ptr_ac;
geometry_msgs::Pose Navigation_Pose;
move_base_msgs::MoveBaseGoal Navigation_MoveBase;
//
void Location_Position_func(const geometry_msgs::Pose::ConstPtr& Location_Position_Pose)
{
	Navigation_Pose = *Location_Position_Pose;
	//we'll send a goal to the robot to move 1 meter forward
  	Navigation_MoveBase.target_pose.header.frame_id = "base_link";
  	Navigation_MoveBase.target_pose.header.stamp = ros::Time::now();
  	Navigation_MoveBase.target_pose.pose = Navigation_Pose;
	(*ptr_ac).sendGoal(Navigation_MoveBase);
    (*ptr_ac).waitForResult();
    if((*ptr_ac).getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("Hooray, the base moved 1 meter forward");
    else
        ROS_INFO("The base failed to move forward 2 meter for some reason");
	/*
	geometry_msgs/Point position
	geometry_msgs/Quaternion orientation
    (*ptr_ac).sendGoal(*Location_Position_Pose);
    (*ptr_ac).waitForResult();
    if((*ptr_ac).getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("Hooray, the base moved 1 meter forward");
    else
        ROS_INFO("The base failed to move forward 2 meter for some reason");
	*/
}
int main(int argc, char **argv)
{
  	ros::init(argc, argv, "Location_Position_Pose_Node");
	ros::NodeHandle nh;
	ros::Subscriber Location_Position_sub = nh.subscribe("location_position", 100, Location_Position_func);
	ros::Publisher Location_Position_pub = nh.advertise<geometry_msgs::Pose>("location_position_echo", 100);
	MoveBaseClient ac("move_base", true);
	ptr_ac = &ac;
    //wait for the action server to come up
	
    while(!ac.waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up");
    }
	
	ros::Rate loop_rate(100);
	while(ros::ok())
	{
		Location_Position_pub.publish(Navigation_Pose);
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}