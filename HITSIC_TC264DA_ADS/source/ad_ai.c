/*
 * ad_ai.c
 *
 *  Created on: 2021Äê3ÔÂ16ÈÕ
 *      Author: HP
 */
#include "ad_ai.h"

static int8 ai_data[10][9];
static uint8 ai_data_flag;
int16 temp;
int16 servo_value;
uint8 ch_AI[AI_NUM]={ADC0_CH1_A1,ADC1_CH9_A25,ADC1_CH4_A20,ADC1_CH0_A16,ADC0_CH8_A8,ADC0_CH6_A6,ADC0_CH4_A4,ADC0_CH2_A2,ADC0_CH0_A0};
uint8 AI_adc[AI_NUM]={ADC_0,ADC_1,ADC_1,ADC_1,ADC_0,ADC_0,ADC_0,ADC_0,ADC_0};
void get_ai_data(void)
{
   for(uint8 i=0;i<=8;i++)
   {
       ai_data[0][i]=(int16)ADC_Get(AI_adc[i], ch_AI[i], ADC_8BIT);
   }
   ai_data_flag = 1;

}

void AI_process(void)
{
    if(ai_data_flag)
    {
        run_model(model1, ai_data, &temp);
        servo_value = temp >> (inf.quant_bits - inf.frac_bits - 1);
        pwm_servo=servo_mid+((float)servo_value)*1.8/127;
        ai_data_flag = 0;
    }

}





