#include <std_msgs/UInt32MultiArray.h>
#include <iostream>
#include <string>
#include <cmath>
#include <numeric>
#define Speed_Max 0.2
#define Speed_Min 0.01
#define Limit_Angle 0.13
#define Limit_Angle_Work 90
//Speed Obj
#define Long_Dist_obj (Speed_Max)
#define Medium_Dist_obj  (Speed_Max-0.001)
#define Distance_Limit 330
#define Long_Dist 250
#define Medium_Dist 200
#define Short_Dist 80
#define First_v 0.1
#define First_w 0.0
#define De_in_crease_Speed 0.003
#define De_in_crease_W 0.05
#define Dist 60
using namespace std;
//UWB
class UWB_Box
{
	public:
		static const int Average_Th = 20;
        //Right Ultrasonic,Mid Ultrasonic,Left Ultrasonic
		std::vector<unsigned int> UWB_A0;
		std::vector<unsigned int> UWB_A1;
		std::vector<unsigned int> UWB_A2;
		double UWB_Mean[3];
	public:
		UWB_Box()
		{
			//Initial Box
			UWB_A0.clear();
			UWB_A1.clear();
			UWB_A2.clear();
			for(int Ultra_index = 0;Ultra_index < 3;Ultra_index++)
			{
				UWB_Mean[Ultra_index] = 0;
			}
		}
		//Avoidance
		double Angle(int Distance_A2A)
		{
			return ((UWB_Mean[1]*UWB_Mean[1])+(Distance_A2A*Distance_A2A)-(UWB_Mean[0]*UWB_Mean[0]))/(2.0*UWB_Mean[1]*Distance_A2A);
		}	
        //Average Filter 
		void Filter(int th = 5)
		{
			//Average
			float A0_Average = std::accumulate(UWB_A0.begin(),UWB_A0.end(),0.0)/(UWB_A0.size());
			float A1_Average = std::accumulate(UWB_A1.begin(),UWB_A1.end(),0.0)/(UWB_A1.size());
			float A2_Average = std::accumulate(UWB_A2.begin(),UWB_A2.end(),0.0)/(UWB_A2.size());
			//Right Filter
			for(std::vector<unsigned int>::iterator it = UWB_A0.begin(); it!=UWB_A0.end() ; it++)
			{
				if(std::abs(*it-A0_Average)>th)*it = A0_Average;
			}
			//Mid Filter
			for(std::vector<unsigned int>::iterator it = UWB_A1.begin(); it!=UWB_A1.end() ; it++)
			{
				if(std::abs(*it-A1_Average)>th)*it = A1_Average;
			}
			//Left Filter
			for(std::vector<unsigned int>::iterator it = UWB_A2.begin(); it!=UWB_A2.end() ; it++)
			{
				if(std::abs(*it-A2_Average)>th)*it = A2_Average;
			}
			A0_Average = std::accumulate(UWB_A0.begin(),UWB_A0.end(),0.0)/(UWB_A0.size());
			A1_Average = std::accumulate(UWB_A1.begin(),UWB_A1.end(),0.0)/(UWB_A1.size());
			A2_Average = std::accumulate(UWB_A2.begin(),UWB_A2.end(),0.0)/(UWB_A2.size());
			UWB_Mean[0] = A0_Average;
			UWB_Mean[1] = A1_Average;
			UWB_Mean[2] = A2_Average;
		}
        //Collect Ultrasonic val until more or equal th
		void UWB_Mean_func(int th = 5)
		{
			//Check 
			if(UWB_A0.size()>=Average_Th && UWB_A1.size()>=Average_Th && UWB_A2.size()>=Average_Th)
			{
				this->Filter();
				UWB_A0.erase(UWB_A0.begin());
				UWB_A1.erase(UWB_A1.begin());
				UWB_A2.erase(UWB_A2.begin());
			}
			cout << this->Angle(320) << endl;
		}
		void Put_value(unsigned int A0_Value,unsigned int A1_Value,unsigned int A2_Value)
		{
			UWB_A0.push_back(A0_Value);
			UWB_A1.push_back(A1_Value);
			UWB_A2.push_back(A2_Value);
		}
};

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
			int Record_invalid_Mid = 0;
			int Record_invalid_Right = 0;
			int Record_invalid_Left = 0;
			/*
			float Acc_R = 0.0; 
			float Acc_M = 0.0;
			float Acc_L = 0.0;
			//Average
			float R_Average = std::accumulate(UltraBox_Right.begin(),UltraBox_Right.end(),0.0)/(UltraBox_Right.size());
			float M_Average = std::accumulate(UltraBox_Mid.begin(),UltraBox_Mid.end(),0.0)/(UltraBox_Mid.size());
			float L_Average = std::accumulate(UltraBox_Left.begin(),UltraBox_Left.end(),0.0)/(UltraBox_Left.size());
			//Right Filter
			for(std::vector<unsigned int>::iterator it = UltraBox_Right.begin(); it!=UltraBox_Right.end() ; it++)
			{
				Acc_R += std::abs(((*it)-R_Average));
				if(std::abs(*it-R_Average)>th)*it = R_Average;
			}
			//Mid Filter
			for(std::vector<unsigned int>::iterator it = UltraBox_Mid.begin(); it!=UltraBox_Mid.end() ; it++)
			{
				Acc_M += std::abs(((*it)-M_Average));
				if(std::abs(*it-M_Average)>th)*it = M_Average;
			}
			//Left Filter
			for(std::vector<unsigned int>::iterator it = UltraBox_Left.begin(); it!=UltraBox_Left.end() ; it++)
			{
				Acc_L += std::abs(((*it)-L_Average));
				if(std::abs(*it-L_Average)>th)*it = L_Average;
			}
			R_Average = std::accumulate(UltraBox_Right.begin(),UltraBox_Right.end(),0.0)/(UltraBox_Right.size());
			M_Average = std::accumulate(UltraBox_Mid.begin(),UltraBox_Mid.end(),0.0)/(UltraBox_Mid.size());
			L_Average = std::accumulate(UltraBox_Left.begin(),UltraBox_Left.end(),0.0)/(UltraBox_Left.size());
			//UltraBox_Mean[0] = L_Average;
			//UltraBox_Mean[1] = M_Average;
			//UltraBox_Mean[2] = R_Average;
			UltraBox_Mean[0] = Acc_L;
			UltraBox_Mean[1] = Acc_M;
			UltraBox_Mean[2] = Acc_R;
			cout << "L: "<< R_Average << "," << "M:" << M_Average << "," << "R: " << L_Average << endl;
			*/
			for(std::vector<unsigned int>::iterator it = UltraBox_Mid.begin(); it!=UltraBox_Mid.end() ; it++)
			{
				//Acc_R += std::abs(((*it)-R_Average));
				//if(std::abs(*it-R_Average)>th)*it = R_Average;
				if(*it==1)Record_invalid_Mid++;
			}
			if(Record_invalid_Mid < Average_Th/3)
			{
				//cout << std::accumulate(UltraBox_Mid.begin(),UltraBox_Mid.end(),0.0)/(UltraBox_Mid.size());
				UltraBox_Mean[1] = std::accumulate(UltraBox_Mid.begin(),UltraBox_Mid.end(),0.0)/(UltraBox_Mid.size());	
			}
			if(Record_invalid_Left < Average_Th/3)
			{
				UltraBox_Mean[0] = std::accumulate(UltraBox_Left.begin(),UltraBox_Left.end(),0.0)/(UltraBox_Left.size());	
			}
			if(Record_invalid_Right < Average_Th/3)
			{
				UltraBox_Mean[2] = std::accumulate(UltraBox_Right.begin(),UltraBox_Right.end(),0.0)/(UltraBox_Right.size());	
			}
			//UltraBox_Mean[1] = ;
			//UltraBox_Mean[2] = ;
			//cout << endl;
		}
        //Collect Ultrasonic val until more or equal th
		void UltraBox_Mean_func(int th = 5)
		{
			//cout << UltraBox_Right.size() << endl;
			//Check 
			if(UltraBox_Right.size()>=Average_Th && UltraBox_Left.size()>=Average_Th && UltraBox_Mid.size()>=Average_Th)
			{
				this->Filter(th);
				UltraBox_Right.erase(UltraBox_Right.begin(),UltraBox_Right.begin()+UltraBox_Right.size()/2);
				UltraBox_Left.erase(UltraBox_Left.begin(),UltraBox_Left.begin()+UltraBox_Left.size()/2);
				UltraBox_Mid.erase(UltraBox_Mid.begin(),UltraBox_Mid.begin()+UltraBox_Mid.size()/2);
				//UltraBox_Right.clear();
				//UltraBox_Mid.clear();
				//UltraBox_Left.clear();
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
		static const int Average_Th = 10000;
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
		void AOABox_Mean_func(float th = 0.0001)
		{
			//Check 
			if(AOABOX.size()>=Average_Th)
			{
				this->Filter(th);
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
class Sensor_Control_Priority
{
	public:
		int Major_Prior;
		int Minor_Prior;
		void(*ptr_func)(void);
	public:
		Sensor_Control_Priority(int Major_Prior,int Minor_Prior,void(*ptr_func)(void))
		{
			this->Major_Prior = Major_Prior;
			this->Minor_Prior = Minor_Prior;
			this->ptr_func = ptr_func;
		};
		Sensor_Control_Priority(int Major_Prior,void(*ptr_func)(void))
		{
			this->Major_Prior = Major_Prior;
			this->Minor_Prior = 0;
			this->ptr_func = ptr_func;
		};
};
class Move_Control
{
    public:
        //const 
        //static const int De_in_crease_Speed = 0.01;
		//std::vector<Sensor_Control_Priority*> Excute_list;
		//Argument
		std::vector<unsigned int> UltraBox_Ripple0;
		std::vector<unsigned int> UltraBox_Ripple1;
		std::vector<unsigned int> UltraBox_Ripple2;
		unsigned int UltraBox_Ripple_Mean[3];
		int UltraBox_Ripple_Th = 200;
        //Input
        unsigned int UltraBox_Mean[3];
		std::vector<unsigned int> UltraBox1;
		std::vector<unsigned int> UltraBox2;
		std::vector<unsigned int> UltraBox3;
        unsigned int UWB[3];
        int AOA_Val[2];
        //Output
        double v,w;
		double w_pre;
		double w_now;
		double d_pre,d_now;
		double Error_Distance_pre,CError_Distance;
    public:
        Move_Control()
        {
            this->v = 0.1;
            this->w = 0;
			this->w_now = 0;
			this->w_pre = 0;
            for(int i = 0; i<3; i++)
            {
                this->UWB[i]=0;
                this->UltraBox_Mean[i]=0;
            }
			for(int i=0;i<2;i++)
			{
				this->AOA_Val[i] = 0;
			}
			d_pre = 0;
			d_now = 0;
			Error_Distance_pre = 0;
			CError_Distance = 0;
			//Excute_list.push_back();
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
	int AOA_Arib()
	{
		//Tweak v
		int _CM = this->AOA_Val[1];
		//Speed up 
		if(_CM > Medium_Dist && _CM < Long_Dist)
		{
			if(this->v < Medium_Dist_obj)
			{
				this->v += De_in_crease_Speed;
			}
			else if(this->v >= Medium_Dist_obj)
			{
				this->v -= De_in_crease_Speed;
			}
			//if(this->v<=0.005)
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
		else if(_CM <= Short_Dist + 100 || _CM > Distance_Limit)
		{
			this->w = 0;
			this->v = 0;
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
		return 1;
	}
	int Ultra_Arib()
	{
		int Ultra_Enable = 0;
		//cout << UltraBox_Mean[1] << endl;
		//cout << UltraBox_Mean[2] << endl;
		//Slow down
		UltraBox_Ripple0.push_back(UltraBox_Mean[0]);
		UltraBox_Ripple1.push_back(UltraBox_Mean[1]);
		UltraBox_Ripple2.push_back(UltraBox_Mean[2]);
		if(UltraBox_Ripple0.size() > UltraBox_Ripple_Th && UltraBox_Ripple1.size() > UltraBox_Ripple_Th && UltraBox_Ripple1.size() > UltraBox_Ripple_Th)
		{
			int Ultra_invaild = 0;
			for(std::vector<unsigned int>::iterator it = UltraBox_Ripple0.begin(); it!=UltraBox_Ripple0.end() ; it++)
			{
				if(*it==1)Ultra_invaild++;
			}
			if(Ultra_invaild > UltraBox_Ripple_Th/2)
			{
				UltraBox_Ripple_Mean[0] = 5000;
			}
			else
			{
				UltraBox_Ripple_Mean[0] = std::accumulate(UltraBox_Ripple0.begin(),UltraBox_Ripple0.end(),0.0)/(UltraBox_Ripple0.size());
			}
			Ultra_invaild = 0;
			for(std::vector<unsigned int>::iterator it = UltraBox_Ripple1.begin(); it!=UltraBox_Ripple1.end() ; it++)
			{
				if(*it==1)Ultra_invaild++;
			}
			if(Ultra_invaild > UltraBox_Ripple_Th/2)
			{
				UltraBox_Ripple_Mean[1] = 5000;
			}
			else
			{
				UltraBox_Ripple_Mean[1] = std::accumulate(UltraBox_Ripple1.begin(),UltraBox_Ripple1.end(),0.0)/(UltraBox_Ripple1.size());
			}
			Ultra_invaild = 0;
			for(std::vector<unsigned int>::iterator it = UltraBox_Ripple2.begin(); it!=UltraBox_Ripple2.end() ; it++)
			{
				if(*it==1)Ultra_invaild++;
			}
			if(Ultra_invaild > UltraBox_Ripple_Th/2)
			{
				UltraBox_Ripple_Mean[2] = 5000;
			}
			else
			{
				UltraBox_Ripple_Mean[2] = std::accumulate(UltraBox_Ripple2.begin(),UltraBox_Ripple2.end(),0.0)/(UltraBox_Ripple2.size());
			}
			UltraBox_Ripple0.erase(UltraBox_Ripple0.begin());
			UltraBox_Ripple1.erase(UltraBox_Ripple1.begin());
			UltraBox_Ripple2.erase(UltraBox_Ripple2.begin());
			//UltraBox_Ripple_Mean[0] = 1;
			//UltraBox_Ripple_Mean[1] = 1;
		}
		//cout << UltraBox_Ripple_Mean[0] << "," << UltraBox_Ripple_Mean[1] << "," << UltraBox_Ripple_Mean[2] << endl;
		if((UltraBox_Ripple_Mean[1] <= Short_Dist && UltraBox_Ripple_Mean[1]>2) || (UltraBox_Ripple_Mean[0] <= Short_Dist && UltraBox_Ripple_Mean[0]>2) || (UltraBox_Ripple_Mean[2] <= Short_Dist && UltraBox_Ripple_Mean[2]>2))
		{
			this->v -= De_in_crease_Speed;
			this->w = 0;
			if(this->v <= 0.005)
			{
				this->v = 0;
				this->w = 0;
			}
			return 1;
		}
		return 0;
	}
    void Control()
    {
	    if((UltraBox_Mean[0]*UltraBox_Mean[1]*UltraBox_Mean[2])>0)
	    {
            this->w_now = ((double)this->AOA_Val[0]*(double)M_PI/180.0);
			//Error distance
			double Error_w = this->w_now - this->w_pre;
			double Error_Distance = this->d_now - this->d_pre;
			this->CError_Distance = Error_Distance - this->Error_Distance_pre;
			int w_flag = 0;
			//W
			if(Error_w > 0)
			{
				this->w += 0.001;
				w_flag = 0;
			}
			else
			{
				this->w -= 0.001;
				w_flag = 1;
			}
			/*************************************************/
			//Distance
			/*
			if(CError_Distance > 0 && this->d_now > 300 && Error_Distance > 0)
			{
				//cout << "Enter" << endl;
				if(w_flag)
				{
					this->w += 0.003;
				}
				else
				{
					this->w -= 0.003;
				}
			}
			/***************************************************/
			this->w_pre = this->w;
			this->d_pre = this->d_now;
			this->Error_Distance_pre = Error_Distance;
			//this->w = (double)(((int)((double)this->AOA_Val[0]*(double)M_PI/180.0)*100.0)/100);
			//cout << this->w << endl;
			if(this->w_now >= std::abs((double)Limit_Angle_Work*(double)M_PI/180.0))
			{
				this->w = 0;
				this->v = 0;
			}
			else if (std::abs(this->w_now)<=0.04)
			{
				this->w = 0;
				this->v = First_v;
			}
			else
			{
				//Limit w
				if(this->w_now < -1*Limit_Angle)this->w= -1*Limit_Angle;
				else if(this->w_now >= Limit_Angle)this->w=Limit_Angle;
				//if(!this->Ultra_Arib())
				//{
					this->AOA_Arib();
				//}
			}
	    }
    }
};