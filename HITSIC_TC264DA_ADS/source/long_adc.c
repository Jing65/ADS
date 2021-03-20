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


//AI采数数组和控制采数数组分开
uint16 LV_control[AD_NUM][SampleTimes];
uint16 LV_AI[AI_NUM][SampleTimes_AI];
float transfer[AD_NUM];
float transfer_AI[AI_NUM];
float AD[(AD_NUM+AI_NUM)];
float MinLVGot=0.05;
float err_ad_now[2]={0,0};
float err_synthetical_now = 0;
//static float Max[(AD_NUM+AI_NUM)];
_Bool Flag_Find_Max = 0;//一开始不开始寻找
uint8 If_Start = 0;//延时发车
uint8 channel_name[AD_NUM]={ADC2_CH4_A36,ADC0_CH3_A3,ADC1_CH8_A24,ADC0_CH5_A5,ADC1_CH5_A21,ADC0_CH7_A7,ADC1_CH1_A17};
uint8 channel_adc[AD_NUM]={ADC_2,ADC_0,ADC_1,ADC_0,ADC_1,ADC_0,ADC_1};
//使AD通道 0到6分别对应 左1横、右一横、左竖、右竖、左二横、右二横、中间电感

//AI电感采数数组
uint8 channel_AI[AI_NUM]={ADC0_CH1_A1,ADC1_CH9_A25,ADC1_CH4_A20,ADC1_CH0_A16,ADC0_CH8_A8,ADC0_CH6_A6,ADC0_CH4_A4,ADC0_CH2_A2,ADC0_CH0_A0};
uint8 channel_AI_adc[AI_NUM]={ADC_0,ADC_1,ADC_1,ADC_1,ADC_0,ADC_0,ADC_0,ADC_0,ADC_0};

//typedef enum
//{
//    //ADC0可选引脚
//    ADC0_CH0_A0   = 0*16 + 0,
//    ADC0_CH1_A1,
//    ADC0_CH2_A2,
//    ADC0_CH3_A3,
//    ADC0_CH4_A4,
//    ADC0_CH5_A5,
//    ADC0_CH6_A6,
//    ADC0_CH7_A7,
//    ADC0_CH8_A8,
//    ADC0_CH10_A10 = 0*16 + 10,
//    ADC0_CH11_A11,
//    ADC0_CH12_A12,
//    ADC0_CH13_A13,
//
//    //ADC1可选引脚
//    ADC1_CH0_A16  = 1*16 + 0,
//    ADC1_CH1_A17  = 1*16 + 1,
//    ADC1_CH4_A20  = 1*16 + 4,
//    ADC1_CH5_A21  = 1*16 + 5,
//    ADC1_CH8_A24  = 1*16 + 8,
//    ADC1_CH9_A25  = 1*16 + 9,
//
//    //ADC2可选引脚
//    ADC2_CH3_A35  = 2*16 + 3,
//    ADC2_CH4_A36,
//    ADC2_CH5_A37,
//    ADC2_CH6_A38,
//    ADC2_CH7_A39,
//    ADC2_CH10_A44 = 2*16 + 10,
//    ADC2_CH11_A45,
//    ADC2_CH12_A46,
//    ADC2_CH13_A47,
//    ADC2_CH14_A48,
//    ADC2_CH15_A49,
//}VADC_CHN_enum;

//上位机传回的数组
int type_of_road = 0;
int sum_of_SCFTM=0;
_Bool send_data_flag=0;//1：ad数据采集完成   0：ad数据未采集完成
uint8 collect_max_flag = 0;
uint8 send_buff[SendDataTime];//WIFI传adc数据
float Max[(AD_NUM+AI_NUM)];
void swap(uint16 *a,uint16 *b)
{
    uint16 temp=*a;
    *a=*b;
    *b=temp;
}


void LV_Sample(void)                             // adc采集函数
{
    for (uint8 h=0;h<AD_NUM;h++)
    {
        for(uint8 i=0;i<=SampleTimes-1;i++)
        {
         /*获取采样初值*/
            LV_control[h][i] = ADC_Get(channel_adc[h], channel_name[h], ADC_8BIT);

        }
    }
    for (uint8 h=0;h<AI_NUM;h++)
    {
        for(uint8 i=0;i<=SampleTimes_AI-1;i++)
        {
         /*获取采样初值*/
            LV_AI[h][i] = ADC_Get(channel_AI_adc[h], channel_AI[h], ADC_8BIT);
        }
    }

}


void LV_Get_Val(void)//约0.3mS                  //对采集的值滤波
{
    //有时会在0-65535(255)间跳动

    for(uint8 i=0;i<AD_NUM;i++)
    {
        for(uint8 j=0;j<=SampleTimes-1;j++)
        {
             if(LV_control[i][j]>500)//剔除毛刺信号
             {
                 LV_control[i][j]=500;
             }
        }
    }
    //排序
    for(uint8 k=0;k<AD_NUM;k++)
    {
        for(uint8 i=0;i<=SampleTimes-2;i++)
        {
            for(uint8 j=i+1;j<=SampleTimes-1;j++)
            {
                if(LV_control[k][i]>LV_control[k][j])
                swap(&LV_control[k][i],&LV_control[k][j]);//交换，swap函数自己写
            }
        }
    }

    for(uint8 k=0;k<AD_NUM;k++)
    {
        for(uint8 i=4;i<=SampleTimes-5;i++)
        {
            transfer[k]+=(float)LV_control[k][i];
        }
        transfer[k]=transfer[k]/(SampleTimes-8);
        if( transfer[k] < MinLVGot )
        {
            transfer[k] = MinLVGot;
        }
    }
}


void Get_AI_AD (void)
{
    for(uint8 i=0;i<AI_NUM;i++)
       {
           for(uint8 j=0;j<=SampleTimes_AI-1;j++)
           {
                if(LV_AI[i][j]>500)//剔除毛刺信号
                {
                    LV_AI[i][j]=500;
                }
           }
       }

       for(uint8 k=0;k<AI_NUM;k++)
       {
           for(uint8 i=0;i<=SampleTimes_AI-2;i++)
           {
               for(uint8 j=i+1;j<=SampleTimes_AI-1;j++)
               {
                   if(LV_AI[k][i]>LV_AI[k][j])
                   swap(&LV_AI[k][i],&LV_AI[k][j]);//交换，swap函数自己写
               }
           }
       }

       for(uint8 k=0;k<AI_NUM;k++)
       {
           for(uint8 i=4;i<=SampleTimes-5;i++)
           {
               transfer_AI[k]+=(float)LV_AI[k][i];
           }
           transfer_AI[k]=transfer_AI[k]/(SampleTimes-8);
           if( transfer_AI[k] < MinLVGot )
           {
               transfer_AI[k] = MinLVGot;
           }
       }
       if (collect_max_flag!=1)
       {
           if(!send_data_flag)
           {
       //
                   for(uint8 i= 0;i<AD_NUM;i++)
                   {
                       //暂时去掉归一化 AD[i] = (100*LV[i])/Max[i];//(K = 100)
                       AD[i] = (100*transfer[i])/Max[i];
                       //AD[i] = transfer[i];

                   }
                   for(uint8 i= 0;i<AI_NUM;i++)
                   {
                       AD[i+AD_NUM] = (127*transfer_AI[i])/Max[i+AD_NUM];//(K = 100)
                       //AD[i+AD_NUM] = transfer_AI[i];
                   }
                   send_data_flag=1;
              }
       }
       else if (collect_max_flag==1)
       {
           for(uint8 i=0;i<AD_NUM;i++)
           {
               if(transfer[i] > Max[i])
               {
                 Max[i] = transfer[i];
               }
               AD[i] = transfer[i];
           }
           for(uint8 i=0;i<AI_NUM;i++)
           {
               if(transfer_AI[i] > Max[i+AD_NUM])
               {
                 Max[i+AD_NUM] = transfer_AI[i];
               }
               AD[i+AD_NUM] = transfer_AI[i];
           }

       }


}

void get_err(void)
{
   //横电感差比积，右一减左一
   err_ad_now[0]=(AD[1]-AD[0])/(AD[1]*AD[0]+1);
   //竖横电感差比积，右减左+10减小竖电感差比积抖动
   err_ad_now[1]=(AD[3]-AD[2])/(AD[3]*AD[2]+10);
   //给竖电感较小的权重
   err_synthetical_now=0*err_ad_now[1]+1*err_ad_now[0];
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
   if (AD[2]-AD[3]<-40&&AD[2]<15)
   {
       type_of_road=11;
   }
   if (AD[2]-AD[3]>40&&AD[3]<15)
   {
       type_of_road=10;
   }
   if(type_of_road!=21)
   {
       if (AD[6]>=200&&AD[0]-AD[1]>10)
       {
           type_of_road=20;
       }
   }
   if(type_of_road!=20)
   {
       if (AD[6]>=200&&AD[0]-AD[1]<-10)
       {
           type_of_road=21;
       }
   }

   //清除直角弯标志位
   if (type_of_road==11||type_of_road==10)
   {
       sum_of_SCFTM++;
       if (AD[6]>50&&sum_of_SCFTM>10)
       {
           type_of_road=0;
           sum_of_SCFTM=0;
       }
   }
   if (type_of_road==20||type_of_road==21)
   {
       sum_of_SCFTM++;
       if (AD[6]<=130&&AD[1]+AD[0]<100&&sum_of_SCFTM>10)
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
//        for(uint8 i=0;i<=6;i++)
//        {
//            send_buff[(i+2)]=(uint8)((int8)AD[(i+7)]-128);
//        }
        send_buff[2]=(uint8)((int8)AD[7]-128);
        send_buff[3]=(uint8)((int8)AD[8]-128);
        send_buff[4]=(uint8)((int8)AD[9]-128);
        send_buff[5]=(uint8)((int8)AD[11]-128);
        send_buff[6]=(uint8)((int8)AD[13]-128);
        send_buff[7]=(uint8)((int8)AD[14]-128);
        send_buff[8]=(uint8)((int8)AD[15]-128);
        send_buff[9]=(uint8)((int8)(127*(pwm_servo-servo_mid)/1.8));
        send_buff[10]=0x5a;
        SmartCar_Uart_Transfer(send_buff,11,0);
        send_data_flag=0;
    }
}

void Save_ADMAX(void)
{
    Sector_Erase(1);
    uint32 MAX[(AD_NUM+AI_NUM)];
    for(uint8 i=0;i<(AD_NUM+AI_NUM);i++)
    {
        MAX[i]=float_conversion_uint32(Max[i]);
        Page_Program(1,i,&MAX[i]);
    }
}

void Read_AD(void)
{
    for(uint8 i=0;i<(AD_NUM+AI_NUM);i++)
    {
        Max[i]=Page_Read(0,i,float);
    }

}

void Elec_process(void)
{
    LV_Sample();
    LV_Get_Val();
    Get_AI_AD();
    recognize_road();
    get_err();
    out_of_road();
    Send_Data();
}



