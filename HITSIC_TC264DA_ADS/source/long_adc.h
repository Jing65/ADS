/*
 * long_adc.h
 *
 *  Created on: 2021年3月7日
 *      Author: HP
 */

#ifndef SOURCE_LONG_ADC_H_
#define SOURCE_LONG_ADC_H_

#include "Ifx_Types.h"
#include "common.h"
#include "Vadc/Adc/IfxVadc_Adc.h"
#include "stdio.h"
#include "SmartCar_ADC.h"
#include "control.h"
#include "Ifx_Types.h"
#include "SmartCar_Uart.h"

#define AD_NUM 7
#define AI_NUM 9
#define SampleTimes 40
#define SampleTimes_AI 40
/**********************************************************************************************************************
*  @brief      获取AD转化值
*  @param      base               ADC模块选择
*  @param      channelGroup       ADC模组，0或1，即A或B，按照MCUXpresso上选
*  @param      channel            ADC通道选择
*  @return     uint32_t   ADC采集数值
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
void Get_AI_AD (void);
extern float err_synthetical_now;
extern float AD[(AD_NUM+AI_NUM)];
extern uint8 If_Start;
extern _Bool Flag_Find_Max;
extern int type_of_road;
//控制ad采数数组



#endif /* SOURCE_LONG_ADC_H_ */
