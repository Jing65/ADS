/*
 * control.h
 *
 *  Created on: 2021��2��27��
 *      Author: SYX
 */

#ifndef SOURCE_CONTROL_H_
#define SOURCE_CONTROL_H_



#define Motor_PIN_0     IfxGtm_ATOM0_0_TOUT53_P21_2_OUT       //����������
#define Motor_PIN_1     IfxGtm_ATOM0_1_TOUT54_P21_3_OUT       //����������
#define Servo_PIN       IfxGtm_ATOM0_1_TOUT31_P33_9_OUT       //����������

extern float pwm_servo;
extern float servo_mid;
void Servo_Elec(void);
void Delay_Start(void);
void Moto_Speed(void);//��pid���Ƶĵ��pwm���
//void modechange(void);//oledͼ����˵�ת��
//void Menu_set(void);
void If_Find_Max(void);

#endif /* SOURCE_CONTROL_H_ */
