/*
 * control.c
 *
 *  Created on: 2021��3��7��
 *      Author: HP
 */
#include "control.h"
float pwm_servo=7.35;
float servo_mid = 7.15;
float KP_m = 32;
float KI_m = 17;
float KP_S_E = 15.5;//��Ŷ������
float KD_S_E = 8.2;//��Ŷ������
//static float Stop_Min = 2;//�����������ĵ�ŵ���Сֵ
static float pwm_servo_variation=0;
static float err_synthetical_last = 0;
static float pwm_moto = 0;
//static float pwm_moto_goal = 30;//����ٶȣ����ڲ˵����ڣ�
static float pwm_moto_add = 0;//���PID��������
static float Moto_Speed_Goal=0;//���ڿ��ƺ�Wifi��λ���е�Ŀ���ٶ�
float Moto_Speed_Goal_Set=2.5;
static float Moto_Speed_Real;//ʵ�ʵ���ٶ�
static float Error_Moto_Now;//�������error
static float Error_Moto_Last;//���ǰһ��error
float Meter_every_Round = 0.48564;//������ģ�����
float LIMIT_SE = 1.8;//��Ҫ�ģ����Ķ�����ܲ�����
static float LIMIT_MO = 80;//��Ҫ�ģ����Ƶ�����ܲ�����
//static float pwm_servo_die = 0.1;
uint8 short_control = 0;
int16 sigle_k=0.4;


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
//    Moto_Speed_Goal = Moto_Speed_Goal_Set;
    //���PID����
    Moto_Speed_Real = 0.0001 * (float)(-SmartCar_Encoder_Get(GPT12_T2)) * Meter_every_Round / 0.005;
    SmartCar_Encoder_Clear(GPT12_T2);//���������
    Error_Moto_Now = (Moto_Speed_Goal - Moto_Speed_Real);
    pwm_moto_add = ((KP_m * (Error_Moto_Now - Error_Moto_Last)) + (KI_m * Error_Moto_Now));
    Error_Moto_Last = Error_Moto_Now;
    pwm_moto = (pwm_moto + pwm_moto_add);

    //����޷�
    if(pwm_moto >= LIMIT_MO)
    {
        pwm_moto = LIMIT_MO;
    }
    else if(pwm_moto<=-LIMIT_MO)
    {
        pwm_moto = -LIMIT_MO;
    }

    //������
    if (pwm_moto>0)
    {
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_0,0);
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_1,(uint32)(pwm_moto*100));
    }
    else if(pwm_moto<0)
    {
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_0,(uint32)(-pwm_moto*100));
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_1,0);
    }

/**************************************************��������*************************************************/
//    static uint32 flag=0;
//    flag++;
//        if(flag<=1000)
//        {
//            pwm_moto = 4000;
//        }
//        if(flag > 1000 && flag < 2000)
//        {
//            pwm_moto = 0;
//        }
//        if(flag == 2000)
//        {
//            pwm_moto = 4000;
//            flag = 0;
//        }
//
//         SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_0, pwm_moto);
//         SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_1, 0);

}

void Servo_Elec_AI(void)
{
    SmartCar_Gtm_Pwm_Setduty(&Servo_PIN, (uint32)(pwm_servo*100));
}
void Servo_Elec(void)//��Ŷ������
{
    if(type_of_road==0)//ֱ����45����
    {
        pwm_servo_variation = KP_S_E * err_synthetical_now  + KD_S_E*(err_synthetical_now - err_synthetical_last );
        if(pwm_servo_variation > LIMIT_SE)
        {
            pwm_servo_variation = LIMIT_SE;
        }
        else if(pwm_servo_variation < -LIMIT_SE)
        {
            pwm_servo_variation = -LIMIT_SE;
        }
        err_synthetical_last = err_synthetical_now;
        pwm_servo = pwm_servo_variation+servo_mid;
    }
    else if (type_of_road==11)//1Ϊֱ�����1Ϊ�ҹ�
    {
        pwm_servo =servo_mid+LIMIT_SE;
    }
    else if (type_of_road==10)//1Ϊֱ�����0Ϊ���
    {
         pwm_servo =servo_mid-LIMIT_SE;
    }
    else if (type_of_road==20)//20��
    {
        pwm_servo =servo_mid-(float)sigle_k*LIMIT_SE/10;
    }
    else if (type_of_road==21)//21�һ�
    {
        pwm_servo =servo_mid+(float)sigle_k*LIMIT_SE/10;
    }
    SmartCar_Gtm_Pwm_Setduty(&Servo_PIN, (uint32)(pwm_servo*100));
//    /*********************************������ֵ**********************************/
//    uint32 pwm_servo_l=(uint32)(servo_mid*100);
//    SmartCar_Gtm_Pwm_Setduty(&Servo_PIN, pwm_servo_l);
}


void Delay_Start(void)
{
    Delay_ms(STM0, 500);
    If_Start = 1;
}


//void If_Find_Max(void)//�Ƿ�ʼѰ�����ֵ
//{
//    //Flag_Find_Max==0
//    if (Flag_Find_Max==0)
//    {
//        Flag_Find_Max = 1;
//    }
//    else if (Flag_Find_Max==1)
//    {
//        Flag_Find_Max = 0;
//    }
//}
