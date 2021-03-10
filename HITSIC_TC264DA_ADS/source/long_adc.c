/*
 * long_adc.c
 *
 *  Created on: 2021年3月7日
 *      Author: HP
 */

/*
 * sc_adc.c
 *
 *  Created on: 2021年2月27日
 *      Author: SYX
 */
#include "long_adc.h"
#include "common.h"
#include "Vadc/Adc/IfxVadc_Adc.h"
#include "stdio.h"
#include "SmartCar_ADC.h"
#include "control.h"
#include "Ifx_Types.h"
#include "SmartCar_Uart.h"


const uint8 SampleTimes=40;
uint32 LV_Temp[8][40] = {0};
float LV[8]={0};
float AD[8]={0};
float MinLVGot=0.05;
float err_ad_now[2]={0,0};
float err_synthetical_now = 0;
_Bool Flag_Find_Max = 1;//一开始不开始寻找
int If_Start = 0;//延时发车
//原来测好的ad口
//uint8_t channel_name[8]={16,23,17,18,10,11,12,13};
//使AD通道 0到6分别对应 左1横、右一横、左竖、右竖、左二横、右二横、中间电感

//尚未实现
//char channel_name[8][12]={"ADC0_CH0_A0","ADC0_CH1_A1","ADC0_CH2_A2","ADC0_CH3_A3","ADC0_CH4_A4","ADC0_CH5_A5","ADC0_CH6_A6","ADC0_CH7_A7"};
//上位机传回的数组
float Wifi_Data[10] = {0};
float Max[8] = {1,1,1,1,1,1,1,1};
int type_of_road = 0;
int sum_of_SCFTM=0;
_Bool send_data_flag=0;//1：ad数据采集完成   0：ad数据未采集完成
uint8 send_buff[11];//WIFI传adc数据

void swap(uint32 *a,uint32 *b)
{
    uint32 temp=*a;
    *a=*b;
    *b=temp;
}


void LV_Sample(void)                             // adc采集函数
{
//    for (uint8 h=0;h<=7;h++)
//    {
//        for(uint8 i=0;i<=SampleTimes-1;i++)
//        {
//         /*获取采样初值*/
//           // LV_Temp[h][i] = ADC_Get(ADC_0, channel_name[h], ADC_8BIT);
//        }
//    }
    for(uint8 i = 0;i<=SampleTimes-1;i++)
    {
        LV_Temp[0][i] = ADC_Get(ADC_0, ADC0_CH0_A0, ADC_8BIT);
    }
    for(uint8 i = 0;i<=SampleTimes-1;i++)
    {
        LV_Temp[0][i] = ADC_Get(ADC_0, ADC0_CH1_A1, ADC_8BIT);
    }
    for(uint8 i = 0;i<=SampleTimes-1;i++)
    {
        LV_Temp[0][i] = ADC_Get(ADC_0, ADC0_CH2_A2, ADC_8BIT);
    }
    for(uint8 i = 0;i<=SampleTimes-1;i++)
    {
        LV_Temp[0][i] = ADC_Get(ADC_0, ADC0_CH3_A3, ADC_8BIT);
    }
    for(uint8 i = 0;i<=SampleTimes-1;i++)
    {
        LV_Temp[0][i] = ADC_Get(ADC_0, ADC0_CH4_A4, ADC_8BIT);
    }
    for(uint8 i = 0;i<=SampleTimes-1;i++)
    {
        LV_Temp[0][i] = ADC_Get(ADC_0, ADC0_CH5_A5, ADC_8BIT);
    }
    for(uint8 i = 0;i<=SampleTimes-1;i++)
    {
        LV_Temp[0][i] = ADC_Get(ADC_0, ADC0_CH6_A6, ADC_8BIT);

    }
    for(uint8 i = 0;i<=SampleTimes-1;i++)
    {
        LV_Temp[0][i] = ADC_Get(ADC_0, ADC0_CH7_A7, ADC_8BIT);
    }
}


void LV_Get_Val(void)//约0.3mS                  //对采集的值滤波
{
    //有时会在0-65535(255)间跳动
    for(uint8 i=0;i<=7;i++)
    {
        for(uint8 j=0;j<=SampleTimes-1;j++)
        {
             if(LV_Temp[i][j]>500)//剔除毛刺信号
             {
                 LV_Temp[i][j]=500;
             }
        }
    }
    //排序
    for(uint8 k=0;k<=7;k++)
    {
        for(uint8 i=0;i<=SampleTimes-2;i++)
        {
            for(uint8 j=i+1;j<=SampleTimes-1;j++)
            {
                if(LV_Temp[k][i]>LV_Temp[k][j])
                swap(&LV_Temp[k][i],&LV_Temp[k][j]);//交换，swap函数自己写
            }
        }
    }

    for(uint8 k=0;k<=7;k++)
    {
        LV[k]=0;
        for(uint8 i=4;i<=SampleTimes-5;i++)
        {
             LV[k]+=(float)LV_Temp[k][i];
        }
        LV[k]=LV[k]/(SampleTimes-8);
        if( LV[k] < MinLVGot )
        {
           LV[k] = MinLVGot;
        }
    }
    //归一化处理
    if(!send_data_flag)
    {
        for(uint8 i= 0;i<=7;i++)
        {
            //暂时去掉归一化 AD[i] = (100*LV[i])/Max[i];//(K = 100)
            AD[i] = LV[i];
        }
        send_data_flag=1;
    }
}

void get_err(void)
{
   //横电感差比积，右一减左一
   err_ad_now[0]=(AD[1]-AD[0])/(AD[1]*AD[0]+1);
   //竖横电感差比积，右减左+10减小竖电感差比积抖动
   err_ad_now[1]=(AD[3]-AD[2])/(AD[3]*AD[2]+10);
   //给竖电感较小的权重
   err_synthetical_now=0.3*err_ad_now[1]+1*err_ad_now[0];
}


/**********************************************************************************************************************
*  @brief      电磁识别道路类型
*  @since      v1.1
*  Sample usage:  0      直道+45度转角
*                 10/11  90度转角左、右打死
*                 20/21  20左环、21右环
**********************************************************************************************************************/
void recognize_road(void)
{
   //直角弯标志位
   if (AD[2]-AD[3]<-50&&AD[2]<8)
   {
       type_of_road=11;
   }
   if (AD[2]-AD[3]>50&&AD[3]<8)
   {
       type_of_road=10;
   }
   if (AD[6]>=170&&AD[4]-AD[5]>10)
   {
       type_of_road=20;
   }
   if (AD[6]>=170&&AD[4]-AD[5]<-10)
   {
       type_of_road=21;
   }
   //清除直角弯标志位
   if (type_of_road==11||type_of_road==10)
   {
       sum_of_SCFTM++;
       if (AD[6]>55&&sum_of_SCFTM>10)
       {
           type_of_road=0;
           sum_of_SCFTM=0;
       }
   }
   if (type_of_road==20||type_of_road==21)
   {
       sum_of_SCFTM++;
       if (AD[6]<=120&&AD[1]+AD[0]<100&&sum_of_SCFTM>10)
       {
           type_of_road=0;
           sum_of_SCFTM=0;
       }
   }



   //正常寻迹偏离赛道的情况
//   if (type_of_road==0)
//   {
//          float ad_sort[5]={};
//          ad_sort[0]=AD[0];//左一
//          ad_sort[1]=AD[1];
//          ad_sort[2]=AD[4];
//          ad_sort[3]=AD[5];
//          ad_sort[4]=AD[6];
//          float max=ad_sort[0];
//          for(uint8_t i=1;i<=4;i++)
//          {
//              if(ad_sort[i]>max)
//              {
//                  max=ad_sort[i];
//              }
//              if(max==ad_sort[0]&&ad_sort[0]<40)
//              {
//                  type_of_road=10;
//              }
//              if(max==ad_sort[1]&&ad_sort[1]<40)
//              {
//                  type_of_road=11;
//              }
//          }
//    }
}

/**********************************************************************************************************************
*  @brief      出赛道保护
*  @since      v1.1
*
**********************************************************************************************************************/
void out_of_road(void)
{
    if(type_of_road==0)
    {
        uint8 count=0;
        for(uint8 i= 0;i<=6;i++)
        {
             if (AD[i]<=1)
             {
                  count++;
             }
        }
        if(count>=7)
        {
           If_Start = 0 ;
        }
    }
}

void Send_Data(void)
{
    if(send_data_flag)
    {
        send_buff[0]=0;
        send_buff[1]=0;
        send_buff[2]=(uint8)AD[0]-128;
        send_buff[3]=(uint8)AD[1]-128;
        send_buff[4]=(uint8)AD[2]-128;
        send_buff[5]=(uint8)AD[3]-128;
        send_buff[6]=(uint8)AD[4]-128;
        send_buff[7]=(uint8)AD[5]-128;
        send_buff[8]=(uint8)AD[6]-128;
        send_buff[9]=(uint8)(128*(pwm_servo-servo_mid)/1.8);
        send_buff[10]=0x5a;
        SmartCar_Uart_Upload(send_buff,11);
        send_data_flag=0;
    }
}

void Elec_process(void)
{
//    LV_Sample();//滤波
//    LV_Get_Val();//需用到归一化的最大值
//    recognize_road();
//    get_err();
//    out_of_road();
    Send_Data();
}



