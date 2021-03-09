/*
 * Init.c
 *
 *  Created on: 2021��3��7��
 *      Author: HP
 */
#include "Init.h"


void elec_init(void)//ADC��ʼ��
{
    ADC_Init(ADC_0, ADC0_CH0_A0);
    ADC_Init(ADC_0, ADC0_CH1_A1);
    ADC_Init(ADC_0, ADC0_CH2_A2);
    ADC_Init(ADC_0, ADC0_CH3_A3);
    ADC_Init(ADC_0, ADC0_CH4_A4);
    ADC_Init(ADC_0, ADC0_CH5_A5);
    ADC_Init(ADC_0, ADC0_CH6_A6);
}

//uint8 ai_data_flag;//1��ad���ݲɼ����   0��ad����δ�ɼ����
//int8 ad_data[7];   //ad����

//void ai_data(void)//�ɼ�adc����
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
    Pit_Init_ms(CCU6_1, PIT_CH0, 5);//����жϳ�ʼ��
    Pit_Init_ms(CCU6_1, PIT_CH1, 20);//����жϳ�ʼ�����ж����ȼ����
//    Pit_Init_us(CCU6_0, PIT_CH0, 100);//��������жϳ�ʼ�����ж����ȼ����
}

void Encoder_init(void)
{
    SmartCar_Encoder_Init(GPT12_T2, IfxGpt120_T2INB_P33_7_IN, IfxGpt120_T2EUDB_P33_6_IN);
}

void Eru_init(void)//�ⲿ�жϳ�ʼ��
{
    Eru_Init(CH6_P20_0, RISING);//���������ж�
}

void Uart_init(void)
{
    SmartCar_Uart_Init(IfxAsclin0_TX_P15_2_OUT,IfxAsclin0_RXB_P15_3_IN,115200,0);
}



