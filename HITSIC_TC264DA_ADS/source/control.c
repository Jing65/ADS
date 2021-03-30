/*
 * control.c
 *
 *  Created on: 2021年3月7日
 *      Author: HP
 */
#include "control.h"
float pwm_servo=7.35;
float servo_mid = 7.15;
float KP_m = 32;
float KI_m = 17;
float KP_S_E = 15.5;//电磁舵机调参
float KD_S_E = 8.2;//电磁舵机调参
//static float Stop_Min = 2;//出赛到保护的电磁的最小值
static float pwm_servo_variation=0;
static float err_synthetical_last = 0;
static float pwm_moto = 0;
//static float pwm_moto_goal = 30;//电机速度（用于菜单调节）
static float pwm_moto_add = 0;//电机PID控制增量
static float Moto_Speed_Goal=0;//用于控制和Wifi上位机中的目标速度
float Moto_Speed_Goal_Set=2.5;
static float Moto_Speed_Real;//实际电机速度
static float Error_Moto_Now;//电机现在error
static float Error_Moto_Last;//电机前一个error
float Meter_every_Round = 0.48564;//数字需改！！！
float LIMIT_SE = 1.8;//需要改（不改舵机可能不动）
static float LIMIT_MO = 80;//需要改（补钙电机可能不动）
//static float pwm_servo_die = 0.1;
uint8 short_control = 0;
int16 sigle_k=0.4;


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
//    Moto_Speed_Goal = Moto_Speed_Goal_Set;
    //电机PID控制
    Moto_Speed_Real = 0.0001 * (float)(-SmartCar_Encoder_Get(GPT12_T2)) * Meter_every_Round / 0.005;
    SmartCar_Encoder_Clear(GPT12_T2);//清除编码器
    Error_Moto_Now = (Moto_Speed_Goal - Moto_Speed_Real);
    pwm_moto_add = ((KP_m * (Error_Moto_Now - Error_Moto_Last)) + (KI_m * Error_Moto_Now));
    Error_Moto_Last = Error_Moto_Now;
    pwm_moto = (pwm_moto + pwm_moto_add);

    //电机限幅
    if(pwm_moto >= LIMIT_MO)
    {
        pwm_moto = LIMIT_MO;
    }
    else if(pwm_moto<=-LIMIT_MO)
    {
        pwm_moto = -LIMIT_MO;
    }

    //电机输出
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

/**************************************************开环测电机*************************************************/
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
void Servo_Elec(void)//电磁舵机控制
{
    if(type_of_road==0)//直道和45度弯
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
    else if (type_of_road==11)//1为直角弯道1为右拐
    {
        pwm_servo =servo_mid+LIMIT_SE;
    }
    else if (type_of_road==10)//1为直角弯道0为左拐
    {
         pwm_servo =servo_mid-LIMIT_SE;
    }
    else if (type_of_road==20)//20左环
    {
        pwm_servo =servo_mid-(float)sigle_k*LIMIT_SE/10;
    }
    else if (type_of_road==21)//21右环
    {
        pwm_servo =servo_mid+(float)sigle_k*LIMIT_SE/10;
    }
    SmartCar_Gtm_Pwm_Setduty(&Servo_PIN, (uint32)(pwm_servo*100));
//    /*********************************测舵机中值**********************************/
//    uint32 pwm_servo_l=(uint32)(servo_mid*100);
//    SmartCar_Gtm_Pwm_Setduty(&Servo_PIN, pwm_servo_l);
}


void Delay_Start(void)
{
    Delay_ms(STM0, 500);
    If_Start = 1;
}


//void If_Find_Max(void)//是否开始寻找最大值
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
