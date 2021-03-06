/*
 * control.h
 *
 *  Created on: 2021年2月27日
 *      Author: SYX
 */

#ifndef SOURCE_CONTROL_H_
#define SOURCE_CONTROL_H_



#define Motor_PIN_0     IfxGtm_ATOM0_0_TOUT53_P21_2_OUT       //定义电机引脚
#define Motor_PIN_1     IfxGtm_ATOM0_1_TOUT54_P21_3_OUT       //定义电机引脚
#define Servo_PIN       IfxGtm_ATOM0_1_TOUT31_P33_9_OUT       //定义舵机引脚

extern float pwm_servo;
extern float servo_mid;
void Servo_Elec(void);
void Delay_Start(void);
void Moto_Speed(void);//含pid控制的电机pwm输出
//void modechange(void);//oled图像与菜单转换
//void Menu_set(void);
void If_Find_Max(void);

#endif /* SOURCE_CONTROL_H_ */
