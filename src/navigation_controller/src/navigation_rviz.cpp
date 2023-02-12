#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <stdlib.h>
#include <geometry_msgs/Pose.h>

void Send_goal(const geometry_msgs::PoseStamped::ConstPtr& array)
{ 
	ROS_INFO("Enter");
  	/*
	ac.sendGoal(goal);

  	ac.waitForResult();

  	if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("Hooray, the base moved 1 meter forward");
  	else
		ROS_INFO("The base failed to move forward 2 meter for some reason");
	*/
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "navigation_rviz");
	ros::NodeHandle nh;
    ros::Subscriber simple_goal = nh.subscribe("/move_base_simple/goal", 1000, Send_goal);
	ROS_INFO("Excute...");
	ros::Rate rate(10);
	while (ros::ok()) {
		//ROS_INFO("THIS is OK");
		ros::spin();
		rate.sleep();
	}
}
