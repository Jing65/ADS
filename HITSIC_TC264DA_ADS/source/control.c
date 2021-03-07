/*
 * control.c
 *
 *  Created on: 2021年3月7日
 *      Author: HP
 */
#include "control.h"
float pwm_servo=7350;
float servo_mid = 7350;


static float KP_M = 32;
static float KI_M = 17;
static float KP_S_E = 10.5;//电磁舵机调参
static float KD_S_E = 0.2;//电磁舵机调参
//static float Stop_Min = 2;//出赛到保护的电磁的最小值
static float pwm_servo_variation=0;
static float err_synthetical_last = 0;
static float pwm_moto = 0;
//static float pwm_moto_goal = 30;//电机速度（用于菜单调节）
static float pwm_moto_add = 0;//电机PID控制增量
static float Moto_Speed_Goal_Set = 1.5;//菜单设置中的目标速度
static float Moto_Speed_Goal=0;//用于控制和Wifi上位机中的目标速度
static float Moto_Speed_Real;//实际电机速度
static float Error_Moto_Now;//电机现在error
static float Error_Moto_Last;//电机前一个error
static float Meter_every_Round = 0.48564;//数字需改！！！
static float LIMIT_S = 1.8;//需要改（不改舵机可能不动）
static float LIMIT_M = 80;//需要改（补钙电机可能不动）
//static float pwm_servo_die = 0.1;


void Moto_Speed(void)//电机控制
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

    //电机PID控制
    Moto_Speed_Real = 0.0001 * SmartCar_Encoder_Get(GPT12_T2) * Meter_every_Round / 0.005;
    SmartCar_Encoder_Clear(GPT12_T2);//清除编码器
    Error_Moto_Now = (Moto_Speed_Goal - Moto_Speed_Real);
    pwm_moto_add = ((KP_M * (Error_Moto_Now - Error_Moto_Last)) + (KI_M * Error_Moto_Now));
    Error_Moto_Last = Error_Moto_Now;
    pwm_moto = (pwm_moto + pwm_moto_add);

    //电机限幅
    if(pwm_moto >= LIMIT_M)
    {
        pwm_moto = LIMIT_M;
    }
    else if(pwm_moto<=-LIMIT_M)
    {
        pwm_moto = -LIMIT_M;
    }

    //电机输出
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


void Servo_Elec(void)//电磁舵机控制
{
    if(type_of_road==0)//直道和45度弯
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
    else if (type_of_road==11)//1为直角弯道1为右拐
    {
        pwm_servo =servo_mid+LIMIT_S;
    }
    else if (type_of_road==10)//1为直角弯道0为左拐
    {
         pwm_servo =servo_mid-LIMIT_S;
    }
    else if (type_of_road==20)//20左环
    {
        pwm_servo =servo_mid-0.7*LIMIT_S;
    }
    else if (type_of_road==21)//21右环
    {
        pwm_servo =servo_mid+0.7*LIMIT_S;
    }
    SmartCar_Gtm_Pwm_Setduty(&Servo_PIN, pwm_servo);
//    /*********************************测舵机中值**********************************/
//    pwm_servo_l=servo_mid;
//    SCFTM_PWM_ChangeHiRes(FTM3, kFTM_Chnl_7 ,50U,pwm_servo_l);
}


void Delay_Start(void)
{
    Delay_ms(STM0, 6000);
    If_Start = 1;
}


void If_Find_Max(void)//是否开始寻找最大值
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
