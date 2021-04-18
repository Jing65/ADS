/*
 * control.c
 *
 *  Created on: 2021��3��7��
 *      Author: HP
 */
#include "control.h"
float pwm_servo=7.15;
float servo_mid = 7.15;
float round_servo = 0.7;
float KP_m = 32;
float KI_m = 17;
float KP_S_E = 15.5;//��Ŷ������
float KD_S_E = 8.2;//��Ŷ������
float KP_S_S = 15.5;//��Ŷ������
float KD_S_S = 8.2;//��Ŷ������
//static float Stop_Min = 2;//�����������ĵ�ŵ���Сֵ
static float pwm_servo_variation=0;
static float err_synthetical_last = 0;
float pwm_moto = 0;
//static float pwm_moto_goal = 30;//����ٶȣ����ڲ˵����ڣ�
static float pwm_moto_add = 0;//���PID��������
static float Moto_Speed_Goal=0;//���ڿ��ƺ�Wifi��λ���е�Ŀ���ٶ�
float Moto_Speed_Goal_Set=1.5;
float Moto_Speed_inround=1.1;
float lower_speed=1.0;
float Moto_Speed_Goal_low=0.9;
static float Moto_Speed_Real;//ʵ�ʵ���ٶ�
static float Error_Moto_Now;//�������error
static float Error_Moto_Last;//���ǰһ��error
float Meter_every_Round = 0.48564;//������ģ�����
float LIMIT_SE = 1.8;//��Ҫ�ģ����Ķ�����ܲ�����
static float LIMIT_MO = 70;//��Ҫ�ģ����Ƶ�����ܲ�����
//static float pwm_servo_die = 0.1;
uint8 short_control = 0;
int16 sigle_k=4;
int16 acc_encoder=0;
void sendddd_data(void)
{
    float buffer[2]={0};
    buffer[0]=Moto_Speed_Goal;
    buffer[1]=Moto_Speed_Real;
    SmartCar_VarUpload(buffer,2);
}
int16 servo_flag=0;

void stop (void)
{
    if(servo_flag>2000&&!GPIO_Read(P02,5))
    {
       If_Start=0;
       servo_flag=0;
    }

}
void Moto_long(void)
{
    if(If_Start == 0)
    {
        Moto_Speed_Goal = 0;
        If_Start = 0;
    } else if(If_Start == 1)
    {
        Moto_Speed_Goal = Moto_Speed_Goal_Set;
    }
    int16 ecoder_num=-SmartCar_Encoder_Get(GPT12_T2);
    SmartCar_Encoder_Clear(GPT12_T2);
    Moto_Speed_Real = 0.0001 * ((float)ecoder_num) * Meter_every_Round / 0.005;
    //���������
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
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_1,0);
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_0,(uint32)(pwm_moto*100));
    }
    else if(pwm_moto<0)
    {
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_1,(uint32)(-pwm_moto*100));
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_0,0);
    }
    acc_encoder += ecoder_num;
    if(acc_encoder>32600)
    {
        acc_encoder=0;
    }
}
void Moto_Speed(void)//�������
{
//    static uint32 flag=0;
//    flag++;
////    if(If_Start == 0)
////    {
////        Moto_Speed_Goal = 0;
////        If_Start = 0;
////    }
////    else
////    {
//                if(flag<=1000)
//                {
//                    Moto_Speed_Goal= 1.2;
//                }
//                if(flag > 1000 && flag < 2000)
//                {
//                    Moto_Speed_Goal = 0;
//                }
//                if(flag == 2000)
//                {
//                    Moto_Speed_Goal = 1.2;
//                    flag = 0;
//                }
////    }
//        int16 ecoder_num=-SmartCar_Encoder_Get(GPT12_T2);
//        SmartCar_Encoder_Clear(GPT12_T2);
//        Moto_Speed_Real = 0.0001 * ((float)ecoder_num) * Meter_every_Round / 0.005;
//        //���������
//        Error_Moto_Now = (Moto_Speed_Goal - Moto_Speed_Real);
//        pwm_moto_add = ((KP_m * (Error_Moto_Now - Error_Moto_Last)) + (KI_m * Error_Moto_Now));
//        Error_Moto_Last = Error_Moto_Now;
//        pwm_moto = (pwm_moto + pwm_moto_add);
//
//        //����޷�
//        if(pwm_moto >= LIMIT_MO)
//        {
//            pwm_moto = LIMIT_MO;
//        }
//        else if(pwm_moto<=-LIMIT_MO)
//        {
//            pwm_moto = -LIMIT_MO;
//        }
//
//        //������
//        if (pwm_moto>0)
//        {
//            SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_1,0);
//            SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_0,(uint32)(pwm_moto*100));
//        }
//        else if(pwm_moto<0)
//        {
//            SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_1,(uint32)(-pwm_moto*100));
//            SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_0,0);
//        }



    if(If_Start == 0)
    {
        Moto_Speed_Goal = 0;
        If_Start = 0;
    }
    else if(type_of_road==0&&AD[11]>100&&AD[11]<140&&If_Start ==1&&in_the_round==0)
//    else if(type_of_road!=10&&type_of_road!=11&&If_Start ==1)
    {
        servo_flag++;
        Moto_Speed_Goal = Moto_Speed_Goal_Set;
//        if(AD[]+AD[]<high_thsou)
//        {
//            Moto_Speed_Goal = Moto_Speed_Goal_Set;
//        }
//        else
//        {
//            Moto_Speed_Goal = Moto_Speed_Goal_low;
//        }
    }
    else if(If_Start ==1&&in_the_round==1)
    {
        Moto_Speed_Goal = Moto_Speed_inround;
    }
    else if(If_Start ==1)
    {
        servo_flag++;
        Moto_Speed_Goal=lower_speed;
    }


//    Moto_Speed_Goal = Moto_Speed_Goal_Set;
    //���PID����
    int16 ecoder_num=-SmartCar_Encoder_Get(GPT12_T2);
    SmartCar_Encoder_Clear(GPT12_T2);
    Moto_Speed_Real = 0.0001 * ((float)ecoder_num) * Meter_every_Round / 0.005;
    //���������
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
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_1,0);
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_0,(uint32)(pwm_moto*100));
    }
    else if(pwm_moto<0)
    {
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_1,(uint32)(-pwm_moto*100));
        SmartCar_Gtm_Pwm_Setduty(&Motor_PIN_0,0);
    }
    acc_encoder += ecoder_num;
    if(acc_encoder>32600)
    {
        acc_encoder=0;
    }
    if(servo_flag>3000)
    {
        servo_flag=3000;
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
    if(pwm_servo>servo_mid+LIMIT_SE)
    {
        pwm_servo=servo_mid+LIMIT_SE;
    }

    if(pwm_servo<servo_mid-LIMIT_SE)
    {
        pwm_servo=servo_mid-LIMIT_SE;
    }
    SmartCar_Gtm_Pwm_Setduty(&Servo_PIN, (uint32)(pwm_servo*100));
}
void Short_Servo_Elec(void)
{
    if(type_of_road==0)//ֱ����45����
    {
        pwm_servo_variation = KP_S_S * err_synthetical_now  + KD_S_S*(err_synthetical_now - err_synthetical_last );
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
    if(type_of_road==30)
    {
        if(into_the_ring==0)
        {
            pwm_servo =servo_mid;
        }
        if(into_the_ring!=0&&left_round!=0&&right_round==0)
        {
            pwm_servo =servo_mid-round_servo;
        }
        if(into_the_ring!=0&&left_round==0&&right_round!=0)
        {
            pwm_servo =servo_mid+round_servo;
        }

    }
    if(type_of_road==31)
    {
        if(out_the_ring==0)
        {
            pwm_servo =servo_mid;
        }

        if(out_the_ring!=0&&left_round!=0&&right_round==0)
        {
            pwm_servo =servo_mid-round_servo;
        }

        if(out_the_ring!=0&&left_round==0&&right_round!=0)
        {
            pwm_servo =servo_mid+round_servo;
        }

    }
    if (type_of_road==11)//1Ϊֱ�����1Ϊ�ҹ�
        {
            pwm_servo =servo_mid+LIMIT_SE;
        }
    if (type_of_road==10)//1Ϊֱ�����0Ϊ���
        {
             pwm_servo =servo_mid-LIMIT_SE;
        }
    if(servo_flag>100)
    {
        SmartCar_Gtm_Pwm_Setduty(&Servo_PIN, (uint32)(pwm_servo*100));
    }
    else
    {
        SmartCar_Gtm_Pwm_Setduty(&Servo_PIN, (uint32)(servo_mid*100));
    }

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

float angle_init(float acc_,float angle_cur,float gyro_,float updatetime_ms)
{
    if(acc_ > 9.8)
    acc_ = 9.8;
    if(acc_ < -9.8)
    acc_ = -9.8;//�޷�
    float angle_acc=0;
    float angle_gyro=0;
   angle_acc=asin(-acc_/9.8)*57.3;
//   angle_gyro = angle_cur + gyro_ * ((float)updatetime_ms) * (0.001f);
//   angle_cur += (gyro_ + (-angle_cur + angle_acc) * 1.25) * ((float)updatetime_ms) * (0.001f);
   return angle_acc;
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
