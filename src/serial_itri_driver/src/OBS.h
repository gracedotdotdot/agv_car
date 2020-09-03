#pragma once
#include "ros/ros.h"
#include <map>
#include <cstring>
using namespace std;
class OBS
{
    private:
        double Dist_th;
        double OBS_w;
        double OBS_v;
        unsigned char OBS_Status;
    public:
        std::map<std::string, double>OBS_v_w_Status;
        double OBS_Raw[6];
        OBS()
        {
            Dist_th = 0.7;
            //Initial
            memset(this->OBS_Raw,0,sizeof(double)*6);
            OBS_w = 0;
            OBS_v = 0;
            OBS_Status = 0;
        }
        void OBS_Phase()
        {
            this->OBS_Raw[0] = this->OBS_Raw[0];
            this->OBS_Raw[1] = this->OBS_Raw[1];
            this->OBS_Raw[2] = this->OBS_Raw[2];
            this->OBS_Raw[3] = this->OBS_Raw[3];
            this->OBS_Raw[4] = this->OBS_Raw[4];
            this->OBS_Raw[5] = this->OBS_Raw[5];
        }
        //
        void Calc_OBS()
        {
            //this->OBS_Phase();
            double Dist_Lidar = this->OBS_Raw[0];
            double Angle_Lidar = this->OBS_Raw[1]; 
            double Dist_Left = this->OBS_Raw[2];
            double Angle_Left = this->OBS_Raw[3];
            double Dist_Right = this->OBS_Raw[4];
            double Angle_Right = this->OBS_Raw[5];
            ROS_INFO("Left Distance: %e, Right Distance: %e",Dist_Left,Dist_Right);
            //Dist Check
            if(Dist_Left == 0 || Dist_Right == 0)
            {
                this->OBS_v_w_Status["w"] = 0;
                this->OBS_v_w_Status["v"] = 0;
                this->OBS_v_w_Status["Status"] = 0;  
            }
            else if(Dist_Left == 0 && Dist_Right == 0)
            {
                this->OBS_v_w_Status["w"] = 0;
                this->OBS_v_w_Status["v"] = 0;
                this->OBS_v_w_Status["Status"] = 0;  
            }
            else if(Dist_Left <= 1e-3 && Dist_Right <= 1e-3)
            {
                this->OBS_v_w_Status["w"] = 0;
                this->OBS_v_w_Status["v"] = 0;
                this->OBS_v_w_Status["Status"] = 0;  
            }
            else if(Dist_Lidar <= 1e-3)
            {
                this->OBS_v_w_Status["w"] = 0;
                this->OBS_v_w_Status["v"] = 0;
                this->OBS_v_w_Status["Status"] = 0;
            }
            else if (Dist_Left <= Dist_th || Dist_Right <= Dist_th || Dist_Lidar <= Dist_th)
            {
                this->OBS_v_w_Status["w"] = 0;
                this->OBS_v_w_Status["v"] = 0;
                this->OBS_v_w_Status["Status"] = -1;
            }
            else
            {
                this->OBS_v_w_Status["w"] = 0;
                this->OBS_v_w_Status["v"] = 0;
                this->OBS_v_w_Status["Status"] = 0;  
            }
            
            /*
            if (Dist_Left < Dist_th && Dist_Right < Dist_th)
            {
                this->OBS_v_w_Status["w"] = 0;
                this->OBS_v_w_Status["v"] = 0;
                this->OBS_v_w_Status["Status"] = 0;
            }
            else if(Dist_Left < Dist_th || Dist_Right < Dist_th)
            {
                //Left
                if(Dist_Left < Dist_Right)
                {
                    this->OBS_v_w_Status["w"] = -0.9;
                    this->OBS_v_w_Status["v"] = 0.1;
                    this->OBS_v_w_Status["Status"] = 1;               
                }
                //Right
                else
                {
                    this->OBS_v_w_Status["w"] = 0.9;
                    this->OBS_v_w_Status["v"] = 0.1;
                    this->OBS_v_w_Status["Status"] = 2;                    
                }
            }
            else
            {
                this->OBS_v_w_Status["w"] = -1;
                this->OBS_v_w_Status["v"] = -1;
                this->OBS_v_w_Status["Status"] = 3;          
            }
            */
        }
        //Display
        void Display()
        {
            ROS_INFO("OBS: %5.3e, %5.3e, %5.3e, %5.3e, %5.3e, %5.3e",this->OBS_Raw[0],this->OBS_Raw[1],this->OBS_Raw[2],this->OBS_Raw[3],this->OBS_Raw[4],this->OBS_Raw[5]);
        }
        /*
        void OBS_Update(double *OBS_Raw)
        {
            memcpy(OBS_Info, OBS_Raw,sizeof(double)*6);
        }
        */
};