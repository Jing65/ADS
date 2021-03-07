/*
 * control.h
 *
 *  Created on: 2021年3月7日
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
void Moto_Speed(void);//含pid控制的电机pwm输出
//void modechange(void);//oled图像与菜单转换
//void Menu_set(void);
void If_Find_Max(void);


#endif /* SOURCE_CONTROL_H_ */
