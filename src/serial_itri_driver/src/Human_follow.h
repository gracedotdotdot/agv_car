#include <std_msgs/UInt32MultiArray.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <numeric>
#include "OBS.h"
using namespace std;
//AOA Stop Dist
#define AOA_Stop_Dist 100
#define Stable_Angle 0.03
//
#define Speed_Max 0.7
#define Speed_Min 0.4
#define Limit_Angle 0.8
#define Limit_Angle_Work 90
//Speed Obj
#define Long_Dist_obj (Speed_Max)
#define Medium_Dist_obj  (Speed_Max-0.001)
#define Distance_Limit 330
#define Long_Dist 250
#define Medium_Dist 200
#define Short_Dist 100
#define First_v 0.4
#define First_w 0.0
#define De_in_crease_Speed 0.001
//#define De_in_crease_W 0.05
#define Dist 80
#define Angle_Mean_Error_th 10
#define Angle_Slope_th 15
//UltraSonic
#define Stop_Dist 100
#define Avoid_Dist 100
// Amount of UltraSonic
const static int UltraSonic_Count = 4;
//AOA Angle
class AOA_Box
{
	public:
		static const int Average_Th = 15;
		//Box
		std::vector<int> Angle;
		std::vector<int> Distance;
		//Slope
		float Angle_Slope;
		float Distance_Slope;
		float Angle_Slope_Pre;
		//Average Error
		int Angle_Mean_Error;
		//Mean
		int Angle_Mean;
		//
		float Distance_Mean;
		//Signal
		bool AOA_Status;
		//Std th
		static const float Std_th = 10;
		//Sensation
		static const int Sensation = 90;
	public:
		AOA_Box()
		{
			//Initial AOABOX
			Angle.clear();
			Distance.clear();

			//Turn on AOA
			AOA_Status = true;
		}
		//Increase Box Value
		void Update_Value(int Angle_Val,int Distance_Val)
		{
			Angle.push_back(Angle_Val);
			Distance.push_back(Distance_Val);
		}
		//Calc abs Sum
		int Calc_abs_Sum(std::vector<int> items)
		{	
			int Sum = 0;
			std::vector<int>::iterator it_i;
			for(it_i=items.begin(); it_i!=items.end(); ++it_i) 
			{
				Sum += abs(*it_i);	
			}
			return Sum;
		}
		//Check AOA Status
		void Check_AOA_Status()
		{
			float Std = this->Calc_Std();
			if(Std==-1)return;
			ROS_INFO("Std=%f",Std);
			if(this->AOA_Status == true)
			{
				if(Std > Std_th || Std==0)
				{
					AOA_Status = false;
				}
			}
			else
			{
				//Clac Now Mean
				int Angle_Sum = (int)std::accumulate(Angle.begin(), Angle.end(),0);
				int Angle_Mean_Temp = (float)Angle_Sum/(float)Angle.size();
				float Std = this->Calc_Std();
				//ROS_INFO("Std_Diff=%f",Std);
				//Wait Other Sensor
				//Check
				if(abs(Angle_Mean_Temp - Angle_Mean) < Sensation && Std < Std_th)
				{
					Angle_Mean = Angle_Mean_Temp;
					AOA_Status = true;
				}
			}
			Angle.clear();
		}
		//Calc Std
		float Calc_Std()
		{
			if(Angle.size() >= Average_Th)
			{
				int Angle_Sum = (int)std::accumulate(Angle.begin(), Angle.end(),0);
				//Clac Average
				int Angle_Mean_Temp = (float)Angle_Sum/(float)Angle.size();
				//Clac Sum of Error
				int Sum_Error = 0;
				vector<int>::iterator it_i;
				for(it_i=Angle.begin(); it_i!=Angle.end(); ++it_i)
				{
					Sum_Error += pow(*it_i-Angle_Mean_Temp,2);
				}
				//Calc Variance
				float Variance = Sum_Error/(float)Angle.size();
				//Calc std
				float Std = sqrt(Variance);
				//Angle.clear();
				return Std; 
				/*
				Angle.clear();
				if(Std > 10 || Std==0)
				{
					Angle_Mean = Angle_Mean_Temp;
					AOA_Status = false;
				}
				cout << Std << "," << Angle_Mean << endl;
				*/
				/*
				int Angle_Sum = Calc_abs_Sum(Angle);
				//Calc Average
				Angle_Mean = abs(Angle_Sum/(int)Angle.size());
				
				//Calc Sum of Error
				int Sum_Error = 0;
				vector<int>::iterator it_i;
				for(it_i=Angle.begin(); it_i!=Angle.end(); ++it_i)
				{
					Sum_Error += pow(abs(*it_i)-abs(Angle_Mean),2);
				}
				//int Sum_Error = abs(abs((float)Angle_Sum)-abs((Angle_Mean*Angle.size()));
				//Sum_Error = pow(Sum_Error,2);
				//Calc Variance
				float Variance = Sum_Error/(float)Angle.size();
				Variance = sqrt(Variance);
				//cout << Angle_Sum << "," << Angle_Mean << "," << Sum_Error << "," << Variance << " ";
				cout << Variance << endl;
				//vector<int>::iterator it_i;
				//for(it_i=Angle.begin(); it_i!=Angle.end(); ++it_i) cout << *it_i << ",";
				//cout << endl; 
				Angle.clear();
				//Angle.erase(Angle.begin(),Angle.begin()+(int)Angle.size()/2);
				*/
			}
			return -1; 
			/*
			if(Angle.size() > Average_Th && Distance.size() > Average_Th)
			{
				//cout << max_element(Angle.begin(), Angle.end()) << endl;
				Angle_Slope = *max_element(Angle.begin(), Angle.end()) -  *min_element(Angle.begin(), Angle.end());
				Distance_Slope = *max_element(Distance.begin(), Distance.end()) - *min_element(Distance.begin(), Distance.end());
				//cout << Angle_Slope << endl;
				if(abs(Angle_Slope) < Angle_Slope_th)
				{
					//Get Angle Mean
					Angle_Mean = (int)std::accumulate(Angle.begin(), Angle.end(),0)/(int)Angle.size();
					Angle_Mean_Error = abs(Angle_Mean-Angle_Slope_Pre);
					if(Angle_Mean_Error <  Angle_Mean_Error_th)
					{
						1==1;
						cout << Angle_Slope_Pre << "," << Angle_Mean_Error << "," << Angle_Mean << endl;
					}
					Angle_Slope_Pre = Angle_Mean;
					Angle.clear();
					Distance.clear();
				}
				else
				{
					Angle.erase(Angle.begin());
					Distance.erase(Distance.begin());
				}
			}
			*/
		}

};
class UltraSonic_Process
{
	public:
		//Th
		const static int th = 5;
		//Sum Variable
		//std::array<unsigned int,UltraSonic_Count> UltraSonic_Sum;
		int *UltraSonic_Sum;
		int *UltraSonic_Mean;
		//Box
		std::vector<std::vector<int> > UltraSonic_Data;
	public:
		UltraSonic_Process()
		{
			//Initial
			UltraSonic_Data.clear();
			UltraSonic_Sum = new int(UltraSonic_Count);
			UltraSonic_Mean = new int(UltraSonic_Count);
			for(int UltraSonic_Sum_Sub = 0; UltraSonic_Sum_Sub < UltraSonic_Count ; UltraSonic_Sum_Sub++)
			{
				//Sum
				UltraSonic_Sum[UltraSonic_Sum_Sub] = 0;
				//Mean
				UltraSonic_Mean[UltraSonic_Sum_Sub] = 0;
			}
		};
		//Increase UltraSonic Value
		void Update_Value(int *UltraSonic_Raw)
		{
			std::vector<int> Temp;
			//Push Raw data to first vector
			for(int Sub_UltraSonic = 0; Sub_UltraSonic < UltraSonic_Count; Sub_UltraSonic++)
			{
				Temp.push_back(UltraSonic_Raw[Sub_UltraSonic]);
			}
			UltraSonic_Data.push_back(Temp);
		}
		//Meam
		void Mean()
		{
			if(UltraSonic_Data.size()>=th)
			{
				//Initial
				for(int UltraSonic_Sum_Sub = 0; UltraSonic_Sum_Sub < UltraSonic_Count ; UltraSonic_Sum_Sub++)
				{
					//Sum
					UltraSonic_Sum[UltraSonic_Sum_Sub] = 0;
					//Mean
					UltraSonic_Mean[UltraSonic_Sum_Sub] = 0;
				}
				//Summary
				for(std::vector<std::vector<int> >::iterator it = UltraSonic_Data.begin(); it != UltraSonic_Data.end(); it++)
				{
					for(int UltraSonic_Data_Sub = 0; UltraSonic_Data_Sub < UltraSonic_Count; UltraSonic_Data_Sub++)
					{
						UltraSonic_Sum[UltraSonic_Data_Sub] += (*it)[UltraSonic_Data_Sub];
					}
				}
				//Mean
				for(int UltraSonic_Data_Sub = 0; UltraSonic_Data_Sub < UltraSonic_Count; UltraSonic_Data_Sub++)
				{
					UltraSonic_Mean[UltraSonic_Data_Sub] = (UltraSonic_Sum[UltraSonic_Data_Sub]/UltraSonic_Data.size());
				}
				UltraSonic_Data.clear();
			}
		}
		//
		void Smooth()
		{
			//Get UltraSonic from raw data
			for(std::vector<std::vector<int> >::iterator it = UltraSonic_Data.begin(); it != UltraSonic_Data.end(); it++)
			{
				cout << (*it)[0] << endl;
				/*
				for(std::vector<unsigned int>::iterator its = (*it).begin(); its!= (*it).end(); its++)
				{
					cout << (*its) << endl;
				}
				*/
			}
			//
		}
};
class Move_Control
{
    public:
        //const 
        //static const int De_in_crease_Speed = 0.01;
		//std::vector<Sensor_Control_Priority*> Excute_list;
		//Argument
		/*
		std::vector<unsigned int> UltraBox_Ripple0;
		std::vector<unsigned int> UltraBox_Ripple1;
		std::vector<unsigned int> UltraBox_Ripple2;
		unsigned int UltraBox_Ripple_Mean[3];
		int UltraBox_Ripple_Th = 200;
        */
		//Input
		/*
        unsigned int UltraBox_Mean[3];
		std::vector<unsigned int> UltraBox1;
		std::vector<unsigned int> UltraBox2;
		std::vector<unsigned int> UltraBox3;
        unsigned int UWB[3];
		*/
        int AOA_Val[2];
		int UltraSonic_Val[UltraSonic_Count];
		OBS *OBS_obj;
        //Output
        double v,w;
		double w_pre;
		double w_now;
		double d_pre,d_now;
		double Error_Distance_pre,CError_Distance;
    public:
        Move_Control(OBS* OBS_obj)
        {
            this->v = 0.1;
            this->w = 0;
			this->w_now = 0;
			this->w_pre = 0;
			this->OBS_obj = OBS_obj;
			/*
            for(int i = 0; i<3; i++)
            {
                this->UWB[i]=0;
                this->UltraBox_Mean[i]=0;
            }
			*/
			for(int i=0;i<2;i++)
			{
				this->AOA_Val[i] = 0;
			}
			d_pre = 0;
			d_now = 0;
			Error_Distance_pre = 0;
			CError_Distance = 0;
        };
    void Updata(int *ptr_AOA_Val,int *ptr_UltraSonic_Val)
    {
		/*
        for(int i = 0; i<3; i++)
        {
            this->UWB[i]=ptr_UWB_Val[i];
            this->UltraBox_Mean[i]=ptr_Ultra_Val[i];
        }
		*/
		//w
        this->AOA_Val[0] = ptr_AOA_Val[0];
		//v
		this->AOA_Val[1] = ptr_AOA_Val[1];
		//UltraSonic
		for(int UltraSonic_index = 0 ; UltraSonic_index < UltraSonic_Count;UltraSonic_index++)
		{
			UltraSonic_Val[UltraSonic_index] = ptr_UltraSonic_Val[UltraSonic_index];
		}
    }
	int UltraSonic_Arib()
	{
		/*
		if(UltraSonic_Val[0]<=Avoid_Dist && UltraSonic_Val[1]>=Avoid_Dist && UltraSonic_Val[2]>=Avoid_Dist && UltraSonic_Val[3]>=Avoid_Dist)
		{
			return 2;
		}
		else if (UltraSonic_Val[0]>=Avoid_Dist && UltraSonic_Val[1]>=Avoid_Dist && UltraSonic_Val[2]>=Avoid_Dist && UltraSonic_Val[3]<=Avoid_Dist)
		{
			return 3;
		}
		else
		{
			return 0;
		}
		*/
		/*
		if((UltraSonic_Val[0]<=Avoid_Dist || UltraSonic_Val[1]<=Avoid_Dist) && UltraSonic_Val[2]>=Avoid_Dist && UltraSonic_Val[3]>=Avoid_Dist)
		{
			return 2;
		}
		else if (UltraSonic_Val[0]>=Avoid_Dist && UltraSonic_Val[1]>=Avoid_Dist && (UltraSonic_Val[2]<=Avoid_Dist || UltraSonic_Val[3]<=Avoid_Dist))
		{
			return 3;
		}
		else
		{
			return 0;
		}
		*/
		for(int UltraSonic_Index = 0; UltraSonic_Index < 4; UltraSonic_Index++)
		{
			if(UltraSonic_Val[UltraSonic_Index] <= Avoid_Dist)return 0;
		}
		return 1;
		
	}
	int AOA_Arib()
	{
		//Tweak v
		int _CM = this->AOA_Val[1];
		this->v += De_in_crease_Speed;
		if(this->v >= Speed_Max)this->v=Speed_Max;
		if(this->v <= Speed_Min)this->v=Speed_Min;
		if (_CM <= AOA_Stop_Dist)
		{
			return -1;
		}
		//Speed up
		/* 
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
		}
		*/	
		return 1;
	}
    void Control()
    {
        this->w_now = ((double)this->AOA_Val[0]*(double)M_PI/180.0);
		//Error distance
		//cout << this->AOA_Val[0] << "," << this->AOA_Val[1] << endl;
 		double Error_w = this->w_now - this->w_pre;
		double Error_Distance = this->d_now - this->d_pre;
		this->CError_Distance = Error_Distance - this->Error_Distance_pre;
		int w_flag = 0;
		//W
		if(Error_w > 0)
		{
			this->w += 0.2;
			w_flag = 0;
		}
		else
		{
			this->w -= 0.2;
			w_flag = 1;
		}
		this->w_pre = this->w;
		this->d_pre = this->d_now;
		this->Error_Distance_pre = Error_Distance;
		if(this->w_now >= std::abs((double)Limit_Angle_Work*(double)M_PI/180.0))
		{
			this->w = 0;
			this->v = 0;
		}
		else if (std::abs(this->w_now)<=Stable_Angle)
		{
			this->w = 0;
			this->v = First_v;
		}
		else
		{
			//Limit w
			if(this->w_now < -1*Limit_Angle)this->w= -1*Limit_Angle;
			else if(this->w_now >= Limit_Angle)this->w=Limit_Angle;
		}
		//Dangerous 
		//OBS
		OBS_obj->Calc_OBS();
		//ROS_INFO("OBS Excute: %d, %e, %e",int(OBS_obj->OBS_v_w_Status["Status"]),OBS_obj->OBS_v_w_Status["w"],OBS_obj->OBS_v_w_Status["v"]);
		//AOA > OBS
		if(this->AOA_Arib()==-1)
		{
			this->w = 0;
			this->v = 0;
			ROS_INFO("AOA Stop Signal");
		}
		else if(int(OBS_obj->OBS_v_w_Status["Status"])==-1)
		{
			this->w = 0;
			this->v = 0;
			ROS_INFO("OBS STOP");
		}
		else if(!this->UltraSonic_Arib())
		{
			this->w = 0;
			this->v = 0;
			ROS_INFO("UltraSonic Stop Signal");
		}
    }
};
