/*
 * Init.c
 *
 *  Created on: 2021年3月7日
 *      Author: HP
 */
#include "Init.h"


void elec_init(void)//ADC初始化
{
    ADC_Init(ADC_0, ADC0_CH0_A0);
    ADC_Init(ADC_0, ADC0_CH1_A1);
    ADC_Init(ADC_0, ADC0_CH2_A2);
    ADC_Init(ADC_0, ADC0_CH3_A3);
    ADC_Init(ADC_0, ADC0_CH4_A4);
    ADC_Init(ADC_0, ADC0_CH5_A5);
    ADC_Init(ADC_0, ADC0_CH6_A6);
}

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
    SmartCar_Gtm_Pwm_Init(&Servo_PIN, 50, servo_mid);
    SmartCar_Gtm_Pwm_Init(&Motor_PIN_0, 50, 0);
    SmartCar_Gtm_Pwm_Init(&Motor_PIN_1, 50, 0);
}

void PIT_init(void)
{
    Pit_Init_ms(CCU6_1, PIT_CH0, 5);//电机中断初始化
    Pit_Init_ms(CCU6_1, PIT_CH1, 20);//舵机中断初始化，中断优先级最低
//    Pit_Init_us(CCU6_0, PIT_CH0, 100);//按键检测中断初始化，中断优先级最低
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
    SmartCar_Uart_Init(IfxAsclin0_TX_P15_2_OUT,IfxAsclin0_RXB_P15_3_IN,115200,0);
}



