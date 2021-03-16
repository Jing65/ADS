/*
 * long_adc.h
 *
 *  Created on: 2021��3��7��
 *      Author: HP
 */

#ifndef SOURCE_LONG_ADC_H_
#define SOURCE_LONG_ADC_H_

#include "Ifx_Types.h"


#define AD_NUM 7
#define AI_NUM 9
#define SampleTimes 40
#define SampleTimes_AI 40
/**********************************************************************************************************************
*  @brief      ��ȡADת��ֵ
*  @param      base               ADCģ��ѡ��
*  @param      channelGroup       ADCģ�飬0��1����A��B������MCUXpresso��ѡ
*  @param      channel            ADCͨ��ѡ��
*  @return     uint32_t   ADC�ɼ���ֵ
*  @since      v1.1
*  Sample usage:          SCADC_GetSample(ADC0,0,12);
**********************************************************************************************************************/

void LV_Sample(void);
void LV_Get_Val(void);
void print_AD(void);
void swap(uint32 *a,uint32 *b);
void get_err(void);
void get_Wifi_AD(void);
void Elec_process(void);
void recognize_road(void);
extern float err_synthetical_now;
extern float AD[(AD_NUM+AI_NUM)];
extern uint8 If_Start;
extern float Max[(AD_NUM+AI_NUM)];//���ڴ洢��е����ֵ����һ��׼��
extern _Bool Flag_Find_Max;
extern float Wifi_Data[10];//��λ�����ص�����
extern int type_of_road;


#endif /* SOURCE_LONG_ADC_H_ */
