/*
 * Init.c
 *
 *  Created on: 2021年3月7日
 *      Author: HP
 */
#include "Init.h"


void elec_init(void)//ADC初始化
{
    ADC_Init(ADC_2, ADC2_CH4_A36);
    ADC_Init(ADC_0, ADC0_CH3_A3);
    ADC_Init(ADC_1, ADC1_CH8_A24);
    ADC_Init(ADC_0, ADC0_CH5_A5);
    ADC_Init(ADC_1, ADC1_CH5_A21);
    ADC_Init(ADC_0, ADC0_CH7_A7);
    ADC_Init(ADC_1, ADC1_CH1_A17);
    ADC_Init(ADC_0, ADC0_CH1_A1);
    ADC_Init(ADC_1, ADC1_CH9_A25);
    ADC_Init(ADC_1, ADC1_CH4_A20);
    ADC_Init(ADC_1, ADC1_CH0_A16);
    ADC_Init(ADC_0, ADC0_CH8_A8);
    ADC_Init(ADC_0, ADC0_CH6_A6);
    ADC_Init(ADC_0, ADC0_CH4_A4);
    ADC_Init(ADC_0, ADC0_CH2_A2);
    ADC_Init(ADC_0, ADC0_CH0_A0);
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
}
//uint8 channel_name[8]={ADC2_CH4_A36,ADC0_CH3_A3,ADC1_CH8_A24,ADC0_CH5_A5,ADC1_CH5_A21,ADC0_CH7_A7,ADC1_CH1_A17,ADC0_CH1_A1};
//uint8 ai_data_flag;//1：ad数据采集完成   0：ad数据未采集完成
//int8 ad_data[7];   //ad数据

//void ai_data(void)//采集adc数据
//{
//    ad_data[0] = (int16)ADC_Get(ADC_0, ADC0_CH0_A0, ADC_8BIT) - 128;
//    ad_data[1] = (int16)ADC_Get(ADC_0, ADC0_CH1_A1, ADC_8BIT) - 128;
//    ad_data[2] = (int16)ADC_Get(ADC_0, ADC0_CH2_A2, ADC_8BIT) - 128;
//    ad_data[3] = (int16)ADC_Get(ADC_0, ADC0_CH3_A3, ADC_8BIT) - 128;
//    ad_data[4] = (int16)ADC_Get(ADC_0, ADC0_CH4_A4, ADC_8BIT) - 128;
//    ad_data[5] = (int16)ADC_Get(ADC_0, ADC0_CH5_A5, ADC_8BIT) - 128;
//    ad_data[6] = (int16)ADC_Get(ADC_0, ADC0_CH6_A6, ADC_8BIT) - 128;
//    ai_data_flag = 1;
//}

void PWM_init(void)
{
    SmartCar_Gtm_Pwm_Init(&Servo_PIN, 50, (uint32)(servo_mid*100));
    SmartCar_Gtm_Pwm_Init(&Motor_PIN_1, 20000, 0);
    SmartCar_Gtm_Pwm_Init(&Motor_PIN_0, 20000, 0);
}

void PIT_init(void)
{
    Pit_Init_ms(CCU6_1, PIT_CH0, 20);//电机中断初始化
    Pit_Init_ms(CCU6_1, PIT_CH1, 5);//舵机中断初始化，中断优先级最低
//    Pit_Init_ms(CCU6_0, PIT_CH0, 100);//按键检测中断初始化，中断优先级最低
}

void Encoder_init(void)
{
    SmartCar_Encoder_Init(GPT12_T2, IfxGpt120_T2INB_P33_7_IN, IfxGpt120_T2EUDB_P33_6_IN);
}

void Eru_init(void)//外部中断初始化
{
    Eru_Init(CH6_P20_0, RISING);//发车启动中断
}

void Uart_init(void)
{
    SmartCar_Uart_Init(IfxAsclin0_TX_P15_2_OUT,IfxAsclin0_RXB_P15_3_IN,921600,0);
}



