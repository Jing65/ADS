/*
 * control.h
 *
 *  Created on: 2021��3��7��
 *      Author: HP
 */

#ifndef SOURCE_CONTROL_H_
#define SOURCE_CONTROL_H_
#include "SmartCar_Pwm.h"
#include "IfxGtm_PinMap.h"
#include "IfxStm.h"
#include "IFXSTM_CFG.h"
#include "common.h"
#include "SmartCar_Encoder.h"
#include "SmartCar_Systick.h"
#include "port_def.h"
#include "long_adc.h"


extern float pwm_servo;
extern float servo_mid;
void Servo_Elec(void);
void Delay_Start(void);
void Moto_Speed(void);//��pid���Ƶĵ��pwm���
//void modechange(void);//oledͼ����˵�ת��
//void Menu_set(void);
void If_Find_Max(void);


#endif /* SOURCE_CONTROL_H_ */
