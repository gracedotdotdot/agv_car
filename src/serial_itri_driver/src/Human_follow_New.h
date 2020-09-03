#include <std_msgs/UInt32MultiArray.h>
#include <geometry_msgs/Twist.h>
#include <iostream>
#include <string>
#include <cmath>
#include "laser_obs_msg/laser_obs_array.h"
#include "laser_obs_msg/laser_obs.h"
using namespace std;
//
class laser_interval_pair
{
    private:
        // global laser_obs_msg::laser_obs
        laser_obs_msg::laser_obs laser_interval_object;
        double Angle_Increment;
        // 
        laser_obs_msg::laser_obs laser_interval_Ripple_object;
    public:
        laser_interval_pair(){};
        //Update
        void Update(laser_obs_msg::laser_obs laser_interval_object)
        {
            this->laser_interval_object = laser_interval_object;
            this->Angle_Increment = laser_interval_object.Angle_increment;
        }
        // Angle_LowerBound need to negative, Angle_UpperBound need to positive
        laser_obs_msg::laser_angle_diatance Find_min_Distance_Condition(double Angle_LowerBound = -2.0, double Angle_UpperBound = 2.0)
        {
            //Laser Interval begin
            laser_obs_msg::laser_angle_diatance begin = this->laser_interval_object.laser_pair[0];
            double angle_begin = begin.angle;
            //Laser Interval end
            laser_obs_msg::laser_angle_diatance end = this->laser_interval_object.laser_pair[this->laser_interval_object.laser_pair.size()-1];
            double angle_end = end.angle;
            //Laser Lower Bound 
            laser_obs_msg::laser_angle_diatance start;
            // 
            int LowerBound_index = abs((Angle_LowerBound - angle_begin ) / this->Angle_Increment);
            int UpperBound_index = abs((Angle_UpperBound - 3.14 ) / this->Angle_Increment) + this->laser_interval_object.laser_pair.size() / 2;
            //ROS_INFO("LowerBound %d, UpperBound %d", LowerBound_index, UpperBound_index);
            //
            laser_interval_Ripple_object.laser_pair.clear();
            for(int Laser_Interval_Index = LowerBound_index; Laser_Interval_Index < UpperBound_index; Laser_Interval_Index++)
            {
                laser_interval_Ripple_object.laser_pair.push_back(this->laser_interval_object.laser_pair[Laser_Interval_Index]);
            }
            //Find min Distance
            double min_Distance = 1e+5;
            laser_obs_msg::laser_angle_diatance min_angle_diatance;
            for(int laser_interval_Ripple_object_Index = 0; laser_interval_Ripple_object_Index < laser_interval_Ripple_object.laser_pair.size(); laser_interval_Ripple_object_Index++)
            {
                if(laser_interval_Ripple_object.laser_pair[laser_interval_Ripple_object_Index].distance <= min_Distance)
                {
                    min_Distance = laser_interval_Ripple_object.laser_pair[laser_interval_Ripple_object_Index].distance;
                    min_angle_diatance = laser_interval_Ripple_object.laser_pair[laser_interval_Ripple_object_Index];
                }
            }
            return min_angle_diatance;
        }
    
};
class AGV_Follow_Control
{
    private:
        geometry_msgs::Twist Control_Twist;
        /*
        //Sample Time
        static const double Sample_Time = 1; //ms 
        //Direct Tweak
        static const double Acceleration_Angle_Speed = 0.1; // rad/s
        static const double Upper_Bound_Limit_Angle = -0.6; // rad/s
        static const double Lower_Bound_Limit_Angle = 0.6; // rad/s
        //Linear Tweak
        static const double Initial_Acceleration_Acceleration = 0.01; // m/s
        static const double Initial_Speed = 0.05; // m/s
        //
        static const double Distance_Limit = 1.0; //m
        static const double Angle_Safe = 0.3; // rad/s
        */
        //Sample Time
        static const double Sample_Time = 1; //ms
        //Direct Tweak
        static const double Acceleration_Angle_Speed = 0.5; // rad/s
        static const double Upper_Bound_Limit_Angle = -0.9; // rad/s
        static const double Lower_Bound_Limit_Angle = 0.9; // rad/s
        //Linear Tweak
        static const double Initial_Acceleration_Acceleration = 0.5; // m/s
        static const double Initial_Speed = 0.05; // m/s
        //
        static const double Distance_Limit = 0.8; //m
        //static const double Angle_Safe = 0.3; // rad/s
        double Angle_Safe; // rad/s
        // 
        double Initial_Acceleration; // m/s
		//
        static const double Upper_Bound_Limit_Speed = 0.8;// m/s
    public:
        AGV_Follow_Control()
        {
            //Initial Twist
            Control_Twist.linear.x = 0;
            Control_Twist.linear.y = 0;
            Control_Twist.linear.z = 0;
            Control_Twist.angular.x = 0;
            Control_Twist.angular.y = 0;
            Control_Twist.angular.z = 0;
            Initial_Acceleration = 0;
            // default
            Angle_Safe = 0.3;
        }
        geometry_msgs::Twist Dangerous_Forced_Deceleration()
        {
            //Speed down
            this->Control_Twist.linear.x -= (0.7* (Sample_Time / 1));
            if(this->Control_Twist.linear.x <= 0)
            {
                this->Control_Twist.linear.x = 0;
            }
            this->Control_Twist.angular.z = 0;
            return Control_Twist;
        }
        geometry_msgs::Twist Forced_Deceleration()
        {
            //Speed down
            this->Control_Twist.linear.x -= (0.5* (Sample_Time / 1));
            if(this->Control_Twist.linear.x <= 0)
            {
                this->Control_Twist.linear.x = 0;
            }
            this->Control_Twist.angular.z = 0;
            return Control_Twist;
        }
        // Due to rad change
        void Dynamic_Safe_Angle(double Distance)
        {
            if(Distance < 1.5)
            {
                Angle_Safe = 0.5;
            }
            else
            {
                Angle_Safe = 0.3;
            }
        }
        //X -> Center_X, Y -> Center_Y, theta -> Percent
        geometry_msgs::Twist Control_Calc(std_msgs::UInt32MultiArray AOA_Information)
        {
            //Information
            double Angle = AOA_Information.data[0];
            double Distance = AOA_Information.data[1];
            Distance /= 100; // Convert to m
            //
            this->Dynamic_Safe_Angle(Distance);
            //Middle Convert
            //Left
            int S_Function = 0;
            if(180.0-Angle < 0)
            {
                S_Function = -1;
            }
            //Right
            else if(180.0-Angle > 0)
            {
                S_Function = 0;
            }
            //Convert to rad
            if(S_Function == -1)
            {
                Angle = (Angle - 360);
            }
            else if(S_Function == 1)
            {
                Angle = S_Function * (Angle);
            }
            //
            Angle = Angle * M_PI / 180.0;
            //ROS_INFO("%e,%e",Angle,Distance);
            //
            if(Angle > 1*Angle_Safe)
            {
                //Turn Right
                this->Control_Twist.angular.z -= (Acceleration_Angle_Speed * (Sample_Time/1));
            }
            else if(Angle < -1*Angle_Safe)
            {
                //Turn Left
                this->Control_Twist.angular.z += (Acceleration_Angle_Speed * (Sample_Time/1));
            }
            else
            {
                //Forward
                this->Control_Twist.angular.z = 0;
            }
            //
            if(this->Control_Twist.angular.z != 0)
            {
                //Upper
                if(this->Control_Twist.angular.z < Upper_Bound_Limit_Angle)
                {
                    this->Control_Twist.angular.z = Upper_Bound_Limit_Angle;
                }
                //Lower
                if(this->Control_Twist.angular.z > Lower_Bound_Limit_Angle)
                {
                    this->Control_Twist.angular.z = Lower_Bound_Limit_Angle;
                }
            }
            //Linear Velocity
            if(Distance > Distance_Limit)
            {
                //Speed up
                Initial_Acceleration += (Initial_Acceleration_Acceleration * (Sample_Time/1000));
                this->Control_Twist.linear.x += Initial_Acceleration;
                //Limit Bound
                if(this->Control_Twist.linear.x >= Upper_Bound_Limit_Speed)
                {
                    this->Control_Twist.linear.x = Upper_Bound_Limit_Speed;
                }
            }
            else
            {
                //Speed down
                this->Control_Twist.linear.x -= (0.5* (Sample_Time / 1));
                if(this->Control_Twist.linear.x <= 0)
                {
                    this->Control_Twist.linear.x = 0;
                }
                ROS_INFO_STREAM("Slow down");
            }
            //Linear to Limit Angle Speed
            if(this->Control_Twist.linear.x == 0)
            {
                this->Control_Twist.angular.z = 0;
            }
            return Control_Twist;
        }
};
