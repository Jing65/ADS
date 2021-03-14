/*
 * long_adc.c
 *
 *  Created on: 2021��3��7��
 *      Author: HP
 */

/*
 * sc_adc.c
 *
 *  Created on: 2021��2��27��
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


//AI��������Ϳ��Ʋ�������ֿ�
uint32 LV_control[AD_NUM][SampleTimes];
uint32 LV_AI[AI_NUM][SampleTimes_AI];
float transfer[AD_NUM];
float transfer_AI[AI_NUM];
float AD[(AD_NUM+AI_NUM)];
float MinLVGot=0.05;
float err_ad_now[2]={0,0};
float err_synthetical_now = 0;
_Bool Flag_Find_Max = 1;//һ��ʼ����ʼѰ��
uint8 If_Start = 0;//��ʱ����
//����ad��������
uint8 channel_name[AD_NUM]={ADC2_CH4_A36,ADC0_CH3_A3,ADC1_CH8_A24,ADC0_CH5_A5,ADC1_CH5_A21,ADC0_CH7_A7,ADC1_CH1_A17};
uint8 channel_adc[AD_NUM]={ADC_2,ADC_0,ADC_1,ADC_0,ADC_1,ADC_0,ADC_1};
//ʹADͨ�� 0��6�ֱ��Ӧ ��1�ᡢ��һ�ᡢ����������������ᡢ�Ҷ��ᡢ�м���

//AI��в�������
uint8 channel_AI[AI_NUM]={ADC0_CH1_A1,ADC1_CH9_A25,ADC1_CH4_A20,ADC1_CH0_A16,ADC0_CH8_A8,ADC0_CH6_A6,ADC0_CH4_A4,ADC0_CH2_A2,ADC0_CH0_A0};
uint8 channel_AI_adc[AI_NUM]={ADC_0,ADC_1,ADC_1,ADC_1,ADC_0,ADC_0,ADC_0,ADC_0,ADC_0};

//typedef enum
//{
//    //ADC0��ѡ����
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
//    //ADC1��ѡ����
//    ADC1_CH0_A16  = 1*16 + 0,
//    ADC1_CH1_A17  = 1*16 + 1,
//    ADC1_CH4_A20  = 1*16 + 4,
//    ADC1_CH5_A21  = 1*16 + 5,
//    ADC1_CH8_A24  = 1*16 + 8,
//    ADC1_CH9_A25  = 1*16 + 9,
//
//    //ADC2��ѡ����
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

//��λ�����ص�����
int type_of_road = 0;
int sum_of_SCFTM=0;
_Bool send_data_flag=0;//1��ad���ݲɼ����   0��ad����δ�ɼ����
uint8 send_buff[11];//WIFI��adc����

void swap(uint32 *a,uint32 *b)
{
    uint32 temp=*a;
    *a=*b;
    *b=temp;
}


void LV_Sample(void)                             // adc�ɼ�����
{
    for (uint8 h=0;h<AD_NUM;h++)
    {
        for(uint8 i=0;i<=SampleTimes-1;i++)
        {
         /*��ȡ������ֵ*/
            LV_control[h][i] = ADC_Get(channel_adc[h], channel_name[h], ADC_8BIT);

        }
    }
    for (uint8 h=0;h<AI_NUM;h++)
    {
        for(uint8 i=0;i<=SampleTimes_AI-1;i++)
        {
         /*��ȡ������ֵ*/
            LV_AI[h][i] = ADC_Get(channel_AI_adc[h], channel_AI[h], ADC_8BIT);
        }
    }

}


void LV_Get_Val(void)//Լ0.3mS                  //�Բɼ���ֵ�˲�
{
    //��ʱ����0-65535(255)������
    for(uint8 i=0;i<AD_NUM;i++)
    {
        for(uint8 j=0;j<=SampleTimes-1;j++)
        {
             if(LV_control[i][j]>500)//�޳�ë���ź�
             {
                 LV_control[i][j]=500;
             }
        }
    }
    for(uint8 i=0;i<AI_NUM;i++)
    {
        for(uint8 j=0;j<=SampleTimes_AI-1;j++)
        {
             if(LV_AI[i][j]>500)//�޳�ë���ź�
             {
                 LV_AI[i][j]=500;
             }
        }
    }
    //����
    for(uint8 k=0;k<AD_NUM;k++)
    {
        for(uint8 i=0;i<=SampleTimes-2;i++)
        {
            for(uint8 j=i+1;j<=SampleTimes-1;j++)
            {
                if(LV_control[k][i]>LV_control[k][j])
                swap(&LV_control[k][i],&LV_control[k][j]);//������swap�����Լ�д
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
                swap(&LV_AI[k][i],&LV_AI[k][j]);//������swap�����Լ�д
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
    for(uint8 i= 0;i<AD_NUM;i++)
    {
        //��ʱȥ����һ�� AD[i] = (100*LV[i])/Max[i];//(K = 100)
        AD[i] = transfer[i];
    }
    for(uint8 i= 0;i<AI_NUM;i++)
    {
        //��ʱȥ����һ�� AD[i] = (100*LV[i])/Max[i];//(K = 100)
        AD[i+AD_NUM] = transfer_AI[i];
    }
///****************************������****************************************/
//    if(!send_data_flag)
//    {
//        for(uint8 i= 0;i<=7;i++)
//        {
//            //��ʱȥ����һ�� AD[i] = (100*LV[i])/Max[i];//(K = 100)
//            AD[i] = LV[i];
//        }
//        send_data_flag=1;
//    }
}

void get_err(void)
{
   //���в�Ȼ�����һ����һ
   err_ad_now[0]=(AD[1]-AD[0])/(AD[1]*AD[0]+1);
   //�����в�Ȼ����Ҽ���+10��С����в�Ȼ�����
   err_ad_now[1]=(AD[3]-AD[2])/(AD[3]*AD[2]+10);
   //������н�С��Ȩ��
   err_synthetical_now=0*err_ad_now[1]+1*err_ad_now[0];
}


/**********************************************************************************************************************
*  @brief      ���ʶ���·����
*  @since      v1.1
*  Sample usage:  0      ֱ��+45��ת��
*                 10/11  90��ת�����Ҵ���
*                 20/21  20�󻷡�21�һ�
**********************************************************************************************************************/
void recognize_road(void)
{
   //ֱ�����־λ
   if (AD[2]-AD[3]<-50&&AD[2]<8)
   {
       type_of_road=11;
   }
   if (AD[2]-AD[3]>50&&AD[3]<8)
   {
       type_of_road=10;
   }
   if (AD[6]>=170&&AD[0]-AD[1]>10)
   {
       type_of_road=20;
   }
   if (AD[6]>=170&&AD[0]-AD[1]<-10)
   {
       type_of_road=21;
   }
   //���ֱ�����־λ
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



   //����Ѱ��ƫ�����������
//   if (type_of_road==0)
//   {
//          float ad_sort[5]={};
//          ad_sort[0]=AD[0];//��һ
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
*  @brief      ����������
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
//        SmartCar_Uart_Upload(send_buff,11);
        send_data_flag=0;
    }
}

void Elec_process(void)
{
    LV_Sample();
    LV_Get_Val();
    recognize_road();
    get_err();
//    out_of_road();
//    Send_Data();
}



