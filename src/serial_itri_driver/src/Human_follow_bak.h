#include <std_msgs/UInt32MultiArray.h>
#include <iostream>
#include <string>
#include <cmath>
#include <numeric>
#define Speed_Max 0.2
#define Limit_Angle 0.3
#define Limit_Angle_Work 90
//Speed Obj
#define Long_Dist_obj (Speed_Max)
#define Medium_Dist_obj  (Speed_Max-0.05)
#define Long_Dist 300
#define Medium_Dist 200
#define Short_Dist 50
#define First_v 0.1
#define First_w 0.0
#define De_in_crease_Speed 0.001
#define De_in_crease_W 0.05
#define Dist 60
using namespace std;
//Ultrasonic
//Ultrasonic
class Ultra_Box
{
	public:
		static const int Average_Th = 50;
        //Right Ultrasonic,Mid Ultrasonic,Left Ultrasonic
		std::vector<unsigned int> UltraBox_Right;
		std::vector<unsigned int> UltraBox_Mid;
		std::vector<unsigned int> UltraBox_Left;
		unsigned int UltraBox_Mean[3];
	public:
		Ultra_Box()
		{
			//Initial Box
			UltraBox_Right.clear();
			UltraBox_Mid.clear();
			UltraBox_Left.clear();
			for(int Ultra_index = 0;Ultra_index < 3;Ultra_index++)
			{
				UltraBox_Mean[Ultra_index] = 0;
			}
		}
        //Average Filter 
		void Filter(int th = 5)
		{
			//Average
			float R_Average = std::accumulate(UltraBox_Right.begin(),UltraBox_Right.end(),0.0)/(UltraBox_Right.size());
			float M_Average = std::accumulate(UltraBox_Mid.begin(),UltraBox_Mid.end(),0.0)/(UltraBox_Mid.size());
			float L_Average = std::accumulate(UltraBox_Left.begin(),UltraBox_Left.end(),0.0)/(UltraBox_Left.size());
			//Right Filter
			for(std::vector<unsigned int>::iterator it = UltraBox_Right.begin(); it!=UltraBox_Right.end() ; it++)
			{
				if(std::abs(*it-R_Average)>th)*it = R_Average;
			}
			//Mid Filter
			for(std::vector<unsigned int>::iterator it = UltraBox_Mid.begin(); it!=UltraBox_Mid.end() ; it++)
			{
				if(std::abs(*it-M_Average)>th)*it = M_Average;
			}
			//Left Filter
			for(std::vector<unsigned int>::iterator it = UltraBox_Left.begin(); it!=UltraBox_Left.end() ; it++)
			{
				if(std::abs(*it-L_Average)>th)*it = L_Average;
			}
			R_Average = std::accumulate(UltraBox_Right.begin(),UltraBox_Right.end(),0.0)/(UltraBox_Right.size());
			M_Average = std::accumulate(UltraBox_Mid.begin(),UltraBox_Mid.end(),0.0)/(UltraBox_Mid.size());
			L_Average = std::accumulate(UltraBox_Left.begin(),UltraBox_Left.end(),0.0)/(UltraBox_Left.size());
			UltraBox_Mean[0] = L_Average;
			UltraBox_Mean[1] = M_Average;
			UltraBox_Mean[2] = R_Average;
		}
        //Collect Ultrasonic val until more or equal th
		void UltraBox_Mean_func(int th = 5)
		{
			//Check 
			if(UltraBox_Right.size()>=Average_Th && UltraBox_Left.size()>=Average_Th && UltraBox_Mid.size()>=Average_Th)
			{
				this->Filter();
				UltraBox_Right.erase(UltraBox_Right.begin());
				UltraBox_Left.erase(UltraBox_Left.begin());
				UltraBox_Mid.erase(UltraBox_Mid.begin());
				/*
				UltraBox_Right.clear();
				UltraBox_Mid.clear();
				UltraBox_Left.clear();
				*/
			}
		}
		void Put_value(unsigned int L_Value,unsigned int M_Value,unsigned int R_Value)
		{
			UltraBox_Right.push_back(R_Value);
			UltraBox_Left.push_back(L_Value);
			UltraBox_Mid.push_back(M_Value);
		}
};
//AOA Angle
class AOA_Box
{
	public:
		static const int Average_Th = 50;
		std::vector<int> AOABOX;
		std::vector<int> AOABOX_CM;
		int AOABOX_Mean;
		int AOABOX_Mean_CM;
	public:
		AOA_Box()
		{
			//Initial AOABOX
			AOABOX.clear();
		}
		void Filter(float th = 0.02)
		{
			//Average
			float AOABOX_Average = std::accumulate(AOABOX.begin(),AOABOX.end(),0.0)/(AOABOX.size());
			for(std::vector<int>::iterator it = AOABOX.begin(); it!=AOABOX.end() ; it++)
			{
				if(std::abs(*it-AOABOX_Average)>th)*it = AOABOX_Average;
			}
			AOABOX_Average = std::accumulate(AOABOX.begin(),AOABOX.end(),0.0)/(AOABOX.size());
			AOABOX_Mean = AOABOX_Average;
			//
			float AOABOX_Average_CM = std::accumulate(AOABOX_CM.begin(),AOABOX_CM.end(),0.0)/(AOABOX_CM.size());
			for(std::vector<int>::iterator it = AOABOX_CM.begin(); it!=AOABOX_CM.end() ; it++)
			{
				if(std::abs(*it-AOABOX_Average)>th)*it = AOABOX_Average_CM;
			}
			AOABOX_Average_CM = std::accumulate(AOABOX_CM.begin(),AOABOX_CM.end(),0.0)/(AOABOX_CM.size());
			AOABOX_Mean_CM = AOABOX_Average_CM;
		}
		void AOABox_Mean_func(float th = 0.005)
		{
			//Check 
			if(AOABOX.size()>=Average_Th)
			{
				this->Filter();
				AOABOX.erase(AOABOX.begin());
				AOABOX_CM.erase(AOABOX_CM.begin());
				//UltraBox_Mid.erase(UltraBox_Mid.begin());
				//AOABOX.clear();
			}
		}
		void Put_value(int AOA_V,int AOA_CM)
		{
			AOABOX.push_back(AOA_V);
			AOABOX_CM.push_back(AOA_CM);
		}
};

class Move_Control
{
    public:
        //const 
        //static const int De_in_crease_Speed = 0.01;
        //Input
        unsigned int UltraBox_Mean[3];
        unsigned int UWB[3];
        int AOA_Val[2];
        //Output
        double v,w;
    public:
        Move_Control()
        {
            this->v = 0.1;
            this->w = 0;
            for(int i = 0; i<3; i++)
            {
                this->UWB[i]=0;
                this->UltraBox_Mean[i]=0;
            }
			for(int i=0;i<2;i++)this->AOA_Val[i] = 0;
        };
    void Updata(unsigned int *ptr_UWB_Val,unsigned int *ptr_Ultra_Val,int *ptr_AOA_Val)
    {
        for(int i = 0; i<3; i++)
        {
            this->UWB[i]=ptr_UWB_Val[i];
            this->UltraBox_Mean[i]=ptr_Ultra_Val[i];
        }
		//w
        this->AOA_Val[0] = ptr_AOA_Val[0];
		//v
		this->AOA_Val[1] = ptr_AOA_Val[1];
    }
    void Control()
    {
	    if((UltraBox_Mean[0]*UltraBox_Mean[1]*UltraBox_Mean[2])>0)
	    {
            this->w = ((double)this->AOA_Val[0]*(double)M_PI/180.0);
			if(this->w >= std::abs((double)Limit_Angle_Work*(double)M_PI/180.0))
			{
				this->w = 0;
				this->v = 0;
			}
			else
			{
				//Limit w
				if(this->w < -1*Limit_Angle)this->w= -1*Limit_Angle;
				else if(this->w >= Limit_Angle)this->w=Limit_Angle;
				//Tweak v
				int _CM = this->AOA_Val[1];
				//Slow down
				if(UltraBox_Mean[1] <= Short_Dist)
				{
					this->v -= De_in_crease_Speed;
					this->w = 0;
					if(this->v <= 0.01)
					{
						this->v = 0;
						this->w = 0;
					}
				}
				//Speed up 
				else if(_CM > Medium_Dist && _CM < Long_Dist)
				{
					if(this->v < Medium_Dist_obj)
					{
						this->v += De_in_crease_Speed;
					}
					else if(this->v >= Medium_Dist_obj)
					{
						this->v -= De_in_crease_Speed;
					}
					//Limit
					if(this->v >= Speed_Max)this->v=Speed_Max;
				}
				else if(_CM >= Long_Dist)
				{
					if(this->v < Long_Dist_obj)
					{
						this->v += De_in_crease_Speed;
					}
					//Limit
					if(this->v >= Speed_Max)this->v=Speed_Max;
				}
				else
				{
					this->v = First_v;
					/*
					if(this->v < First_v)
					{
						this->v += De_in_crease_Speed;
					}
					else if(this->v > First_v)
					{
						this->v -= De_in_crease_Speed;
					}
					*/
				}
			}

	    }
    }
};