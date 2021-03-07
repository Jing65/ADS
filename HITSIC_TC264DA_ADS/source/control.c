/*
 * control.c
 *
 *  Created on: 2021��3��7��
 *      Author: HP
 */
#include "control.h"
float pwm_servo=7350;
float servo_mid = 7350;


static float KP_M = 32;
static float KI_M = 17;
static float KP_S_E = 10.5;//��Ŷ������
static float KD_S_E = 0.2;//��Ŷ������
//static float Stop_Min = 2;//�����������ĵ�ŵ���Сֵ
static float pwm_servo_variation=0;
static float err_synthetical_last = 0;
static float pwm_moto = 0;
//static float pwm_moto_goal = 30;//����ٶȣ����ڲ˵����ڣ�
static float pwm_moto_add = 0;//���PID��������
static float Moto_Speed_Goal_Set = 1.5;//�˵������е�Ŀ���ٶ�
static float Moto_Speed_Goal=0;//���ڿ��ƺ�Wifi��λ���е�Ŀ���ٶ�
static float Moto_Speed_Real;//ʵ�ʵ���ٶ�
static float Error_Moto_Now;//�������error
static float Error_Moto_Last;//���ǰһ��error
static float Meter_every_Round = 0.48564;//������ģ�����
static float LIMIT_S = 1.8;//��Ҫ�ģ����Ķ�����ܲ�����
static float LIMIT_M = 80;//��Ҫ�ģ����Ƶ�����ܲ�����
//static float pwm_servo_die = 0.1;


void Moto_Speed(void)//�������
{
    if(If_Start == 0)
    {
        Moto_Speed_Goal = 0;
        If_Start = 0;
    }
    else
    {
        Moto_Speed_Goal = Moto_Speed_Goal_Set;
    }

    //���PID����
    Moto_Speed_Real = 0.0001 * SmartCar_Encoder_Get(GPT12_T2) * Meter_every_Round / 0.005;
    SmartCar_Encoder_Clear(GPT12_T2);//���������
    Error_Moto_Now = (Moto_Speed_Goal - Moto_Speed_Real);
    pwm_moto_add = ((KP_M * (Error_Moto_Now - Error_Moto_Last)) + (KI_M * Error_Moto_Now));
    Error_Moto_Last = Error_Moto_Now;
    pwm_moto = (pwm_moto + pwm_moto_add);

    //����޷�
    if(pwm_moto >= LIMIT_M)
    {
        pwm_moto = LIMIT_M;
    }
    else if(pwm_moto<=-LIMIT_M)
    {
        pwm_moto = -LIMIT_M;
    }

    //������
    if (pwm_moto>0)
    {
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_0, pwm_moto);
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_1, 0);
    }
    else if(pwm_moto<0)
    {
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_0, 0);
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_1, -pwm_moto);
    }
}


void Servo_Elec(void)//��Ŷ������
{
    if(type_of_road==0)//ֱ����45����
    {
        pwm_servo_variation = KP_S_E * err_synthetical_now  + KD_S_E*(err_synthetical_now - err_synthetical_last );
        if(pwm_servo_variation > LIMIT_S)
        {
            pwm_servo_variation = LIMIT_S;
        }
        else if(pwm_servo_variation < -LIMIT_S)
        {
            pwm_servo_variation = -LIMIT_S;
        }
        err_synthetical_last = err_synthetical_now;
        pwm_servo = pwm_servo_variation+servo_mid;
    }
    else if (type_of_road==11)//1Ϊֱ�����1Ϊ�ҹ�
    {
        pwm_servo =servo_mid+LIMIT_S;
    }
    else if (type_of_road==10)//1Ϊֱ�����0Ϊ���
    {
         pwm_servo =servo_mid-LIMIT_S;
    }
    else if (type_of_road==20)//20��
    {
        pwm_servo =servo_mid-0.7*LIMIT_S;
    }
    else if (type_of_road==21)//21�һ�
    {
        pwm_servo =servo_mid+0.7*LIMIT_S;
    }
    SmartCar_Gtm_Pwm_Setduty(&Servo_PIN, pwm_servo);
//    /*********************************������ֵ**********************************/
//    pwm_servo_l=servo_mid;
//    SCFTM_PWM_ChangeHiRes(FTM3, kFTM_Chnl_7 ,50U,pwm_servo_l);
}


void Delay_Start(void)
{
    Delay_ms(STM0, 6000);
    If_Start = 1;
}


void If_Find_Max(void)//�Ƿ�ʼѰ�����ֵ
{
    //Flag_Find_Max==0
    if (Flag_Find_Max==0)
    {
        Flag_Find_Max = 1;
    }
    else if (Flag_Find_Max==1)
    {
        Flag_Find_Max = 0;
    }
}
