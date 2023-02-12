#include <ros/ros.h>
#include "std_msgs/Float64MultiArray.h"
#include "sensor_msgs/LaserScan.h"
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/PoseArray.h>
#include <math.h>
#include "std_msgs/String.h"
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include "navigation_controller/command.h"
#include "navigation_controller/command_status.h"

class NavigationController {
public:
	NavigationController(tf::TransformListener& tf);
	~NavigationController();
	bool set_command(navigation_controller::command::Request &req,navigation_controller::command::Response &res);
	bool set_command_status(navigation_controller::command_status::Request &req_status,navigation_controller::command_status::Response &res_status);
	void run();
private:
	ros::NodeHandle nh_;
	ros::Publisher vel_pub_;
	ros::Subscriber scan_sub_;
        ros::Subscriber scan_sub_camera;
	ros::Subscriber poscmd_sub_;
	ros::Publisher mark_array_pub_;
	ros::Publisher robot_position;
	ros::Publisher obs_position;
        ros::Publisher obstacle_pub;
        ros::Publisher obs_dir_pub;

	tf::TransformListener& tf_;
	sensor_msgs::LaserScan scan_;
        sensor_msgs::LaserScan scan_camera;
	visualization_msgs::MarkerArray mark_array_;
	geometry_msgs::PoseArray obs_map_array;

	enum NavigationState {
		CONTROLLING,
		APPROACHING,
		FINISH,
		CANCEL
	};
	NavigationState state_;

	unsigned int command_type_;
	geometry_msgs::PoseStamped goal_;
	geometry_msgs::Twist last_cmd_;



	void get_scan(const sensor_msgs::LaserScan::ConstPtr& scan);
        void get_scan_camera(const sensor_msgs::LaserScan::ConstPtr& scan__camera);
	void get_pos(const std_msgs::Float64MultiArray::ConstPtr& array);

	void angle_control(double *cmd_w, double des_theta, double current_theta);
	void position_control(double *cmd_v, double *cmd_w, double desX, double desY, double currentX, double currentY, double current_theta);
	void avoid_radiate(double *v_a, double *w_a,double user_velocity,double user_rotation,double obs_r,double obs_theta,double minrange);
	void get_obstacle_vector(double *obs_r,double *obs_theta,double *minrange);

	static const int beam_num;
	//static const double kAvoidRadius_;
        double kAvoidRadius_;
	static const double D2R;
	static const double R2D;
	static const double MaxRobotLinearVelocity;
	static const double MaxRobotAngularVelocity;

        bool switch_avoid;
};
//=========status============//
std::string action_status;
//===========================//
const double NavigationController::D2R = M_PI / 180.0;			// degrees to rad
const double NavigationController::R2D = 180.0 / M_PI;			// rad to degrees
const double NavigationController::MaxRobotLinearVelocity = 0.4;		// m/s 0.47
const double NavigationController::MaxRobotAngularVelocity = 50.0 * D2R;	// radian/s 40

const int NavigationController::beam_num = 30;
//const double NavigationController::kAvoidRadius_ = 1.5;	//m 1.0
int count_angle = 0;
bool nav_or_not = false ;
double bot_pos_obs;
NavigationController::NavigationController(tf::TransformListener& tf) : tf_(tf)
{
	vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);
	scan_sub_ = nh_.subscribe("scan", 1, &NavigationController::get_scan, this);
        scan_sub_camera = nh_.subscribe("scan_camera", 1, &NavigationController::get_scan_camera, this);
	poscmd_sub_ = nh_.subscribe("pos_cmd", 1, &NavigationController::get_pos, this);
	mark_array_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("mark_array", 1 );
	robot_position = nh_.advertise<geometry_msgs::Pose2D>("robot_position", 1 );
	obs_position = nh_.advertise<geometry_msgs::PoseArray>("obs_position", 1 );
        obstacle_pub = nh_.advertise<std_msgs::String>("obstacle", 1);
        obs_dir_pub = nh_.advertise<std_msgs::Float64MultiArray>("obs_dir", 1);

	command_type_ = 0;
	state_ = FINISH;
	action_status = "ActionStatusWaitingForStart";
	ROS_INFO_STREAM("contrust the navigation control class");


        kAvoidRadius_ = 0.65;	//m
        ros::NodeHandle para_node("~");
        // avoidance switch
	if(!para_node.getParam("AvoidSwitch", switch_avoid)){
		switch_avoid = true;
	}
	if(switch_avoid == false)
		kAvoidRadius_ = 0.5;
}

NavigationController::~NavigationController()
{
	ROS_INFO_STREAM("delete the navigation controller class");
}

void NavigationController::get_pos(const std_msgs::Float64MultiArray::ConstPtr& array)
{
	action_status = "ActionStatusWaitingForStart";
	if (array->data.size() == 2) {
		goal_.header.stamp = ros::Time::now();
		goal_.pose.position.x = array->data[0];
		goal_.pose.position.y = array->data[1];
		goal_.pose.orientation.z = 0.0;
		state_ = CONTROLLING;
		command_type_ = 2;
		ROS_INFO_STREAM("Control postition, X = " << goal_.pose.position.x << " Y = " << goal_.pose.position.y );
	}
	else if(array->data.size() == 1) {
		goal_.header.stamp = ros::Time::now();
		goal_.pose.position.x = 0.0;
		goal_.pose.position.y = 0.0;
		goal_.pose.orientation.z = array->data[0];
		state_ = CONTROLLING;
		command_type_ = 1;
		ROS_INFO_STREAM("Control orientation, theta = " << goal_.pose.orientation.z );
	}

}

bool NavigationController::set_command(navigation_controller::command::Request &req,navigation_controller::command::Response &res)
{
	action_status = "ActionStatusWaitingForStart";
	goal_.header.stamp = ros::Time::now();
	goal_.pose.position.x = req.x;
	goal_.pose.position.y = req.y;
	goal_.pose.orientation.z = 0.0;
	state_ = CONTROLLING;
	ROS_INFO_STREAM("Control postition, X = " << goal_.pose.position.x << " Y = " << goal_.pose.position.y );
	goal_.pose.orientation.z = req.theta;
	ROS_INFO_STREAM("Control orientation, theta = " << goal_.pose.orientation.z );
	nav_or_not = req.is_nav;
	return true;
}
bool NavigationController::set_command_status(navigation_controller::command_status::Request &req_status,navigation_controller::command_status::Response &res_status)
{
	geometry_msgs::PoseStamped replace;
	if(req_status.get_status == true){
		res_status.status_is = action_status;
	}
	if(req_status.is_cancel == true){
		state_ = CANCEL;
		res_status.status_is = "ActionStatusStopped";
	}
}

void NavigationController::run()
{
	
	ros::Rate rate(10);

	//total command
	double v_t = 0.0;
	double w_t = 0.0;

	//navigation command
	double v_n = 0.0;
	double w_n = 0.0;

	//avoid command
	double v_a = 0.0;
	double w_a = 0.0;

	double Ea = 0.0;	//confidence factor of avoidance
	double G_u = 0.0;	//gain of user
	double G_r = 0.0;	//gain of robot

	double obs_r = 0.0;
	double obs_theta = 0.0;
	double minrange = 0.0;
        double minrange_camera = 0.0;
	geometry_msgs::Twist cmd;
	geometry_msgs::Pose2D bot_pos;
	//int count = 0 ; // just for tf tree initial
        bool xy_ok = false;
	while (ros::ok()) {

		obs_r = 0.0;
		obs_theta = 0.0;
		minrange = 0.0;
                minrange = 0.0;
		get_obstacle_vector(&obs_r,&obs_theta,&minrange);
		//=================================================//		
		//=================================================//

		tf::StampedTransform transform;
		tf::Quaternion q;
		//get robot position

			try {
				tf_.lookupTransform("/world", "/base_link", ros::Time(0), transform);
				q = transform.getRotation();
			}
			catch (tf::TransformException ex) {
				ROS_ERROR("%s",ex.what());
				ros::Duration(1.0).sleep();
			}
			//ROS_INFO_STREAM("x = " << transform.getOrigin().x() * 100 << " y = " << transform.getOrigin().y() * 100 << " theta = " << tf::getYaw(q) * R2D);
			//===============position===============//
			bot_pos.x = transform.getOrigin().x();
			bot_pos.y = transform.getOrigin().y();
			bot_pos.theta = tf::getYaw(q);
			bot_pos_obs = tf::getYaw(q);
			robot_position.publish(bot_pos);


		//===============================================//
		//===============================================//
		if(state_ == CONTROLLING || state_ == APPROACHING) {
			action_status = "ActionStatusRunning";
			v_t = 0.0;
			w_t = 0.0;

			v_n = 0.0;
			w_n = 0.0;

			v_a = 0.0;
			w_a = 0.0;
			
			if(!xy_ok) {
				position_control(&v_n,&w_n,goal_.pose.position.x,goal_.pose.position.y,transform.getOrigin().x(),transform.getOrigin().y(),tf::getYaw(q));
				if(v_n == 0.0 && w_n == 0.0){
					//state_ = FINISH;
					//action_status = "ActionStatusFinished";
					xy_ok = true;
				}
				else if(sqrt(pow(goal_.pose.position.x-transform.getOrigin().x(),2)+pow(goal_.pose.position.y-transform.getOrigin().y(),2)) < 0.0){
					state_ = APPROACHING;
				}
			//}
			}
			if(xy_ok) {
				angle_control(&w_n,goal_.pose.orientation.z,tf::getYaw(q));
				if(w_n == 0.0){
					state_ = FINISH;
					//std::cout<<"FUCKKKKKKKKKKKKKK"<<std::endl;
					action_status = "ActionStatusFinished";
					xy_ok = false;
				}
				else if(fabs(goal_.pose.orientation.z-tf::getYaw(q)) < 0.0 * D2R){
					state_ = APPROACHING;
				}
			}

			//navigation controller join to control robot
			if (v_n != 0.0 || w_n != 0.0) {
				avoid_radiate(&v_a, &w_a, v_n, w_n, obs_r, obs_theta, minrange);

				//v_t = v_n * G_u + v_a * G_r;
				//v_t = v_n * G_u;
				//w_t = w_n * G_u + w_a * G_r;
				v_t = v_a;
				w_t = w_a;
				if(v_n != 0) {
					;//w_t = w_t *  v_n / v_t;
					//v_t = v_n;
				}
				else if(v_n == 0) {
					;//w_t = w_n;
				}

			}

			//ROS_INFO_STREAM("v_t = " << v_t << " w_t = " << w_t);

			//send command to controller
			//cmd.linear.x = v_t;
			//cmd.angular.z = w_t;
			//vel_pub_.publish(cmd);
			//w_t = -w_t; //!!!!!!!!!!!!!!! because of tf test
		}

		double acc_v = 0.05;
		double acc_w = 10 * D2R;

		if(fabs(v_t - last_cmd_.linear.x) < acc_v)
			cmd.linear.x = v_t;
		else if((v_t - last_cmd_.linear.x) > 0)
			cmd.linear.x = last_cmd_.linear.x + acc_v;
		else if((v_t - last_cmd_.linear.x) < 0)
			cmd.linear.x = last_cmd_.linear.x - acc_v;


		if(fabs(w_t - last_cmd_.angular.z) < acc_w)
			cmd.angular.z = w_t;
		else if((w_t - last_cmd_.angular.z) > 0)
			cmd.angular.z = last_cmd_.angular.z + acc_w;
		else if((w_t - last_cmd_.angular.z) < 0)
			cmd.angular.z = last_cmd_.angular.z - acc_w;

		if (state_ == CANCEL || nav_or_not == false){
			cmd.linear.x = 0.0 ;
			cmd.angular.z = 0.0 ;
		}
		//cmd.angular.z = -cmd.angular.z; // test
		vel_pub_.publish(cmd);
		last_cmd_ = cmd;
		ros::spinOnce();
		rate.sleep();
	}
}

void NavigationController::get_scan(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	scan_ = *scan;
}
void NavigationController::get_scan_camera(const sensor_msgs::LaserScan::ConstPtr& scan__camera)
{
	scan_camera = *scan__camera;
}
void NavigationController::get_obstacle_vector(double *obs_r,double *obs_theta,double *minrange)
{
	int beam_range = scan_camera.ranges.size() / beam_num;
	double beam_minrange[beam_num];		//m
	double beam_minrange_2[beam_num];	//m
	double beam_angle[beam_num];		//rad
	double adjust;				//m
	double adjust_2;			//m

	*minrange = scan_camera.range_max;		//m
	for(int i = 0; i < beam_num; i++) {
		if(i >= (beam_num / 4) && i <= (beam_num * 3 / 4)){	// beam_num / 5 && beam_num* 4 / 5
			if(switch_avoid == true){	
				adjust = 0.4;	//0.8
				adjust_2 = 0.0;
				//ROS_INFO_STREAM("ds");
			}
			else{
				adjust = 0.5;  //0.35
				adjust_2 = 0.0;
			}
		}
		else{
			adjust = 0.4;	//0.8
			adjust_2 = 0.6;
		}
		beam_minrange[i] = scan_camera.range_max;
		beam_minrange_2[i] = scan_camera.range_max;
		for(int j = i * beam_range; j < (i+1) * beam_range; j++) { 
			if(std::isnormal(scan_camera.ranges[j])){
				beam_minrange[i] = std::min(beam_minrange[i],(double)std::max((scan_camera.ranges[j]-adjust), 0.0));
				beam_minrange_2[i] = std::min(beam_minrange_2[i],(double)std::max((scan_camera.ranges[j]-adjust_2), 0.0));
			}
		}
		//ROS_INFO_STREAM("beam_minrange_1 is " << beam_minrange[i]);
		//ROS_INFO_STREAM("beam_minrange_2 is " << beam_minrange_2[i]);
		if(beam_minrange[i]==0.0 && beam_minrange_2[i]!=0.0){
			if(switch_avoid == true){
				beam_minrange[i] = beam_minrange_2[i]-0.2;
				if(beam_minrange[i]<0)
					beam_minrange[i] = 0.1;
			}
			else
				beam_minrange[i] = 0.1;
		}
		beam_angle[i] = scan_camera.angle_min + ((double)i + 0.5) * (double)beam_range * scan_camera.angle_increment;
		//ROS_INFO_STREAM("angle " << i << ": " << beam_angle[i] * R2D << " min : " << beam_minrange[i]);
		if(beam_minrange[i] != 0.0)
			*minrange = std::min(*minrange,beam_minrange[i]);
	}
	//-------------------------------------------------
        //=======================//
        double obs_left[2] = {0.0, 0.0};
        double obs_min[2] = {0.0, 0.0};
        double obs_right[2] = {0.0, 0.0};
        std_msgs::Float64MultiArray obs_array;
        obs_array.data.clear();
        int obs_dir_count[3] = {0.0, 0.0, 0.0};
        //=======================//
	double obs_vector_x = 0.0;
	double obs_vector_y = 0.0;

	int obs_count = 0;
	for (int i = 0; i < beam_num; i++) {
		double temp = 0.0;
		if (beam_minrange[i] < kAvoidRadius_ && beam_minrange[i] > 0) {
			temp = 1.0 - pow((beam_minrange[i] / kAvoidRadius_),0.5);
			obs_vector_x += temp * cos(beam_angle[i]);
			obs_vector_y += temp * sin(beam_angle[i]);
			obs_count++;
                        if(beam_angle[i] * R2D < 30.0 && beam_angle[i]  * R2D >= 28.0/3){
                           obs_left[0] += temp * cos(beam_angle[i]);
			   obs_left[1] += temp * sin(beam_angle[i]);
			   obs_dir_count[0]++;
                        }
                        if(beam_angle[i]  * R2D >= -28.0/3 && beam_angle[i]  * R2D <= 28.0/3){
                           obs_min[0] += temp * cos(beam_angle[i]);
			   obs_min[1] += temp * sin(beam_angle[i]);
			   obs_dir_count[1]++;
                        }
                        if(beam_angle[i]  * R2D >= -30.0 && beam_angle[i]  * R2D < -(28.0)/3){
                           obs_right[0] += temp * cos(beam_angle[i]);
			   obs_right[1] += temp * sin(beam_angle[i]);
			   obs_dir_count[2]++;
                        }
		}
	}
        //================//
        double obs_dir_r[3] = {0.0,  0.0, 0.0};
        double obs_dir_theta[3] = {0.0,  0.0, 0.0};
        //================//
	*obs_r = 0.0;
	*obs_theta = 0.0;
	if(obs_count > 0) {
		obs_vector_x /= obs_count;
		obs_vector_y /= obs_count;
		*obs_r = sqrt(pow(obs_vector_x, 2) + pow(obs_vector_y, 2));
		*obs_theta = atan2(obs_vector_y, obs_vector_x);
	}
        if(obs_dir_count[0] > 0) {
		obs_left[0] /= obs_dir_count[0];
		obs_left[1] /= obs_dir_count[0];
		obs_dir_r[0] = sqrt(pow(obs_left[0], 2) + pow(obs_left[1], 2));
		obs_dir_theta[0] = atan2(obs_left[1], obs_left[0]);
	}
        else{
                obs_dir_r[0] = 0.0;
                obs_dir_theta[0] = 0.0;
        }
        if(obs_dir_count[1] > 0) {
		obs_min[0] /= obs_dir_count[1];
		obs_min[1] /= obs_dir_count[1];
		obs_dir_r[1] = sqrt(pow(obs_min[0], 2) + pow(obs_min[1], 2));
		obs_dir_theta[1] = atan2(obs_min[1], obs_min[0]);
	}
        else{
                obs_dir_r[1] = 0.0;
                obs_dir_theta[1] = 0.0;
        }
        if(obs_dir_count[2] > 0) {
		obs_right[0] /= obs_dir_count[2];
		obs_right[1] /= obs_dir_count[2];
		obs_dir_r[2] = sqrt(pow(obs_right[0], 2) + pow(obs_right[1], 2));
		obs_dir_theta[2] = atan2(obs_right[1], obs_right[0]);
	}
        else{
                obs_dir_r[2] = 0.0;
                obs_dir_theta[2] = 0.0;
        }
        //==============//
        for(int i=0; i<3; i++){
            obs_array.data.push_back(obs_dir_r[i]);
            obs_array.data.push_back(obs_dir_theta[i]);
        }
        obs_dir_pub.publish(obs_array);
        //ROS_INFO_STREAM("obs_count " << obs_count << " obs_r " << *obs_r << " obs_theta : " << *obs_theta);
        //ROS_INFO_STREAM("obs_left_count " << obs_dir_count[0] << "obs_left_r " << obs_dir_r[0] << " obs_left_theta " << obs_dir_theta[0]);
        //ROS_INFO_STREAM("obs_min_count " << obs_dir_count[1] << "obs_min_r " << obs_dir_r[1] << " obs_left_theta " << obs_dir_theta[1]);
        //ROS_INFO_STREAM("obs_right_count " << obs_dir_count[2] << "obs_right_r " << obs_dir_r[2] << " obs_left_theta " << obs_dir_theta[2]);
	//------------------------------------------------
	mark_array_.markers.clear();

	visualization_msgs::Marker marker;
	marker.header.frame_id = "laser";
	marker.header.stamp = ros::Time::now();
	marker.type = visualization_msgs::Marker::ARROW;
	marker.action = visualization_msgs::Marker::ADD;
	marker.pose.position.x = 0.0;
	marker.pose.position.y = 0.0;
	marker.pose.position.z = 0.0;
	marker.pose.orientation.x = 0.0;
	marker.pose.orientation.y = 0.0;
	marker.scale.y = 0.01;
	marker.scale.z = 0.01;
	marker.color.r = 0;
	marker.color.g = 1.0;
	marker.color.b = 1.0;
	marker.color.a = 1.0;


	for(int i = 0; i < beam_num; i++) {
		char temp[50];
		sprintf(temp,"obs_arrow%d",i);
		marker.ns = std::string(temp);
		marker.pose.orientation.z = sin(beam_angle[i]/2);
		marker.pose.orientation.w = cos(beam_angle[i]/2);
		if(beam_minrange[i] <= 0)
			marker.scale.x = 0.00001;
		else
			marker.scale.x = beam_minrange[i];
		mark_array_.markers.push_back(marker);
	}
		
	marker.ns = "obs_arrow";
	marker.pose.orientation.z = sin(*obs_theta/2);
	marker.pose.orientation.w = cos(*obs_theta/2);
	if(*obs_r <= 0)
		marker.scale.x = 0.00001;
	else
		marker.scale.x = *obs_r;
	marker.color.r = 1.0;
	marker.color.g = 0.0;
	marker.color.b = 0.0;
	marker.color.a = 1.0;
	mark_array_.markers.push_back(marker);
	mark_array_pub_.publish(mark_array_);
}

void NavigationController::avoid_radiate(double *v_a, double *w_a,double user_velocity,double user_rotation,double obs_r,double obs_theta,double minrange)
{
	// Status::Topic
	std_msgs::String obstacle;

	minrange = minrange / 1.0;
	//ROS_INFO_STREAM("minrange = " << minrange);
	double Ea = 1;	
	if(switch_avoid == true)
		Ea = pow(std::min(minrange / kAvoidRadius_, 1.0), 0.23) -0.4;
	else
		Ea = pow(std::min(pow(minrange / kAvoidRadius_,2), 1.0), 0.9) - 0.2;  //0.2
	ROS_INFO_STREAM("Ea = " << Ea);	
	//double Ea = 1

	// Status:: obstacle topic
	if(Ea >= 0.85)	
		obstacle.data = "false";
	else
		obstacle.data = "true";
	obstacle_pub.publish(obstacle);


	if(switch_avoid == true){

		*v_a = user_velocity * Ea;
		if(Ea > 0) {
			if (obs_theta >= 0.0 && obs_r != 0)
				*w_a = user_rotation * Ea + (*v_a) / minrange * -1.0 * (1 - Ea);
			else if (obs_theta < 0.0)
				*w_a = user_rotation * Ea + (*v_a) / minrange * 1.0 * (1 - Ea);
			else
				*w_a = user_rotation;
		}
		else {
			*w_a = MaxRobotAngularVelocity;
		}
		if(fabs(*v_a) > MaxRobotLinearVelocity)
			*v_a = *v_a / fabs(*v_a) * fabs(MaxRobotLinearVelocity);
		if(fabs(*w_a) > MaxRobotAngularVelocity)
			*w_a = *w_a / fabs(*w_a) * fabs(MaxRobotAngularVelocity);
	}
	else{
		if(Ea >= 0.85){
			
			*v_a = user_velocity * Ea;
			if(Ea > 0) {
				if (obs_theta >= 0.0 && obs_r != 0)
					*w_a = user_rotation * Ea + (*v_a) / minrange * -1.0 * (1 - Ea);
				else if (obs_theta < 0.0)
					*w_a = user_rotation * Ea + (*v_a) / minrange * 1.0 * (1 - Ea);
				else
					*w_a = user_rotation;
			}
			else {
				*w_a = MaxRobotAngularVelocity;
			}
			if(fabs(*v_a) > MaxRobotLinearVelocity)
				*v_a = *v_a / fabs(*v_a) * fabs(MaxRobotLinearVelocity);
			if(fabs(*w_a) > MaxRobotAngularVelocity)
				*w_a = *w_a / fabs(*w_a) * fabs(MaxRobotAngularVelocity);
		}
		else{
			*v_a = 0;
			*w_a = 0;
		}
	}
}

//(rad/s ,rad ,rad)
void NavigationController::angle_control(double *cmdW, double desTheta, double currentTheta)
{
	double deltaTheta = 0.0;
	double kp = 2.0;
	double w = 0.0;

	//angle between -180 ~ 180 degree
	if (desTheta > M_PI)
		desTheta = desTheta - int((desTheta + M_PI) / (2 * M_PI)) * (2 * M_PI);
	else
		desTheta = desTheta - int((desTheta - M_PI) / (2 * M_PI)) * (2 * M_PI);

	//angle between -180 ~ 180 degree
	if (currentTheta > M_PI)
		currentTheta = currentTheta - int((desTheta + M_PI) / (2 * M_PI)) * (2 * M_PI);
	else
		currentTheta = currentTheta - int((desTheta - M_PI) / (2 * M_PI)) * (2 * M_PI);

	deltaTheta = desTheta - currentTheta;
	//in order not to turn opposite direction
	if (deltaTheta > M_PI)
		deltaTheta -= 2.0 * M_PI;
	else if (deltaTheta < -M_PI)
		deltaTheta += 2.0 * M_PI;

	w = kp * deltaTheta;

	if (fabs(w) > fabs(MaxRobotAngularVelocity))
		w = w / fabs(w) * fabs(MaxRobotAngularVelocity);
	else
		w = w / fabs(w) * std::max(5.0 * D2R, fabs(w));
	
	if(fabs(deltaTheta) <= 9.0 * D2R){
		w = 0.6*w;
		std::cout<<"~~~~~~~~angle!!!!!!!!!!!!!!!!!!!!!! = " << fabs(deltaTheta)<<std::endl;
		if (fabs(deltaTheta) <= 6.0 * D2R){
			count_angle ++;
			std::cout<<"~~~~~~~~angle!!!!!!!!!!!!!!!!!!!!!! = " << count_angle <<std::endl;
			if(count_angle == 80){
				w = 0.0;
				count_angle = 0;
			}
		}
	}
	
         //if (fabs(deltaTheta) <= 1.0 * D2R ) //
		//w = 0.0;
	//return command
	*cmdW = w;
}

//(m/s ,rad/s ,m ,m ,m ,m ,rad)count_angle
void NavigationController::position_control(double *cmdV, double *cmdW, double desX, double desY, double currentX, double currentY, double currentTheta)
{
	double kp = 2.0;		//kp of rotation
	double v = 0.0;
	double w = 0.0;

	double deltaX = 0.0;		//m
	double deltaY = 0.0;		//m
	double deltaR = 0.0;		//m
	double angle = 0.0;		//radian
	double deltaTheta = 0.0;	//radian

	deltaX = desX - currentX;
	deltaY = desY - currentY;
	deltaR = sqrt(pow(deltaX, 2) + pow(deltaY, 2));
	//in radian between -pi ~ pi
	angle = atan2(deltaY, deltaX);
	//angle between -180 ~ 180 degree
	if (currentTheta > M_PI)
		currentTheta = currentTheta - int((currentTheta + M_PI) / (2 * M_PI)) * (2 * M_PI);
	else
		currentTheta = currentTheta - int((currentTheta - M_PI) / (2 * M_PI)) * (2 * M_PI);

	deltaTheta = angle - currentTheta;
	//in order not to turn opposite direction
	if (deltaTheta > M_PI)
		deltaTheta -= 2.0 * M_PI;
	else if (deltaTheta < -M_PI)
		deltaTheta += 2.0 * M_PI;
/*
	if (fabs(deltaTheta) > 40 * D2R) {
		angle_control(&w, angle, currentTheta);
		*cmdV = 0.0;
		*cmdW = w;
		return;
	}
*/
	w = kp * deltaTheta;
	if (w == 0.0)	//not to divide by zero
		;
	else if (fabs(w) > MaxRobotAngularVelocity)
		w = w / fabs(w) * MaxRobotAngularVelocity;
	else
		w = w / fabs(w) * std::max(5.0 * D2R, fabs(w));	//adjustment for both of wheel speed not too low

	if (deltaR == 0.0)	//not to divide by zero
		;
	else if (fabs(deltaR) > fabs(MaxRobotLinearVelocity * 2.0))
		v = deltaR / fabs(deltaR) * fabs(MaxRobotLinearVelocity);
	else
		v = deltaR / fabs(deltaR) * std::max(0.10, fabs(deltaR / 2.0));	//low speed maybe cause robot can not move

	//reach the goal
	
	if (fabs(deltaR) <= 0.05)
		v = 0.0;
	if (fabs(deltaTheta) <= 1.0 * D2R || fabs(deltaR) <= 0.1) //
		w = 0.0;
	
	
	//return command
	*cmdV = v;
	*cmdW = w;
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "navigation_controller");
	ros::NodeHandle nh;
	tf::TransformListener tf(ros::Duration(10));
	NavigationController navigation_controller(tf);
	ros::ServiceServer service = nh.advertiseService("pos_cmd", &NavigationController::set_command, &navigation_controller);
	ros::ServiceServer service_check = nh.advertiseService("get_status", &NavigationController::set_command_status, &navigation_controller);
	navigation_controller.run();

	ros::Rate rate(10);
	while (ros::ok()) {
		ros::spinOnce();
		rate.sleep();
	}
	ros::spin();
	ros::spin();
	return 0;
}

