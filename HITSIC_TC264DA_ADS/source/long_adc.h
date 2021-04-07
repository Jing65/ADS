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
#include "SmartCar_Flash.h"

#define AD_NUM 7
#define AI_NUM 9
#define SampleTimes 26
#define SampleTimes_AI 26
#define SendDataTime   11
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
void print_AD(void);
void swap(uint16 *a,uint16 *b);
void get_err(void);
void Elec_process(void);
void recognize_road(void);
void Get_AI_AD (void);
void Short_process(void);
void Long_process(void);
void send_ad(void);
//void Save_ADMAX(void);
//void Read_AD(void);
extern float err_synthetical_now;
extern float AD[(AD_NUM+AI_NUM)];
extern float Max[(AD_NUM+AI_NUM)];
extern uint8 If_Start;
extern uint8 collect_max_flag;
extern uint8 type_of_road;
//extern int16 right_threshould;
//extern int16 cancel_right_ad;
//extern int16 _SCFTM;
typedef struct
{
     int16 right_threshould;
     int16 cancel_right_ad;
     int16 _SCFTM;
     int16 cross_misjudge;
     int16 round_midjudge;
     int16 round_misjudge_num;
     int16 round_misjudge_single;
     int16 round_SCFTM;

} Judgment_threshold;

extern Judgment_threshold long_theshold,short_theshold;
//控制ad采数数组



#endif /* SOURCE_LONG_ADC_H_ */
