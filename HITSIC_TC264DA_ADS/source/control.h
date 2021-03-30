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
#include "SmartCar_Pwm.h"
#include "port_def.h"
#include "long_adc.h"
//处理程序flag
extern uint8 process_type_ai;
extern float KP_m;
extern float KI_m;
extern float pwm_servo;
extern float servo_mid;
extern float Moto_Speed_Goal_Set;
extern float KP_S_E;//电磁舵机调参
extern float KD_S_E;//电磁舵机调参
extern float LIMIT_SE;
extern uint8 short_control;
extern int16 sigle_k;
//extern float Moto_Speed_Goal_Set;
void Servo_Elec(void);
void Delay_Start(void);
void Moto_Speed(void);//含pid控制的电机pwm输出
//void modechange(void);//oled图像与菜单转换
//void Menu_set(void);
void If_Find_Max(void);
void Servo_Elec_AI(void);


#endif /* SOURCE_CONTROL_H_ */
