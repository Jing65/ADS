/*
 * sc_adc.h
 *
 *  Created on: 2021��2��27��
 *      Author: SYX
 */

#ifndef SOURCE_SC_ADC_H_
#define SOURCE_SC_ADC_H_

#include "Ifx_Types.h"

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
void Elec_process_fun(void);
void recognize_road(void);
extern float err_synthetical_now;
extern float LV[8];
extern float AD[8];
extern int If_Start;
extern float Max[8];//���ڴ洢��е����ֵ����һ��׼��
extern _Bool Flag_Find_Max;
extern float Wifi_Data[10];//��λ�����ص�����
extern int type_of_road;


#endif /* SOURCE_SC_ADC_H_ */
