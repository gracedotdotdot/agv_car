#include "ros/ros.h"
#include <std_msgs/String.h>
#include "navigation_controller/command.h"
#include <iostream>
#include <fstream>

class CommandPubSrvGui {
public:
	CommandPubSrvGui();
	~CommandPubSrvGui();

private:
	ros::NodeHandle nh_;
	ros::Subscriber place_cmd_;
	ros::ServiceClient client_;
	ros::Publisher reach_pub_;

	void get_place(const std_msgs::String::ConstPtr& place);
};

CommandPubSrvGui::CommandPubSrvGui()
{
	place_cmd_ = nh_.subscribe<std_msgs::String>("place", 1, boost::bind(&CommandPubSrvGui::get_place, this, _1));
	client_ = nh_.serviceClient<navigation_controller::command>("pos_cmd");
}

CommandPubSrvGui::~CommandPubSrvGui()
{
	
}


void CommandPubSrvGui::get_place(const std_msgs::String::ConstPtr& place)
{
	int num = 0;
	double x = 0.0;
	double y = 0.0;
	double theta = 0.0;
	navigation_controller::command srv;

	std::string path = "/home/an/ITRI_NCTU/catkin_ws/src/navigation_controller/cmd_txt/cmd" + place->data + ".txt";
	std::fstream myfile;
	myfile.open(path.c_str());

	while (myfile >> num && ros::ok()) {
	//while (ros::ok()) {	
		x = 0.0;
		y = 0.0;
		theta = 0.0;
		
		if(num == 1 || num == 4 || num == 6|| num == 9)
			myfile >> theta;
		else if(num == 2 || num == 3 || num == 5|| num == 7|| num == 8)
			myfile >> x >> y;
		else if(num == 0) {
			continue;
		}
	
		ROS_INFO_STREAM("x = " << x << " y = " << y << " theta = " << theta);
		
		srv.request.type = num;
		srv.request.x = x;
		srv.request.y = y;
		srv.request.theta = theta;
		srv.request.is_nav = true;

		if(client_.call(srv)) {
			ROS_INFO("call service success");
			srv.request.type = 0;
			while(srv.response.run_completed == false) {
				usleep(100000);
				//ROS_INFO("hello servie");
				client_.call(srv);
			}
		}
		else {
			ROS_INFO("call service fail");
		}
	}
	
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "command_pub_srv_gui");
	CommandPubSrvGui commandpubsrvgui;

	ros::spin();	
	return 0;
}
