/*
 * develop_menu.h
 *
 *  Created on: 2021��3��7��
 *      Author: HP
 */

#ifndef SOURCE_DEVELOP_MENU_H_
#define SOURCE_DEVELOP_MENU_H_

#include "common.h"
#include "SmartCar_Flash.h"
#include "control.h"
#include "SmartCar_Oled.h"
#include "SmartCar_Systick.h"
#include "SmartCar_GPIO.h"
#include "long_adc.h"

#define NAME_MAX 20//�˵������ֳ��ȵļ���
#define ITEM_MAX 80//�˵�����Ŀ�ļ���
#define PARA_MAX 60//�����Ͳ˵�����Ŀ�ļ���

extern uint16 ItemNum;
extern uint16 ParaNum;
extern uint16 OrderNum;
extern uint16 CurItem;
extern uint16 CurMenu;


//���ڲ��԰汾���������Ե���ò����ã�
enum ITEMID//ע���IDһһ��Ӧ***˳���ܸ��***
{
    MENU = 0,
    MODE = 1, AD_, STEER, MOTOR, UNUSE, HOLD,THRESHOLD,
    AD_1, AD_2, AD_3, AD_4, AD_5, AD_6,AD_7, AD_8, AD_9, AD_10, AD_11, AD_12,AD_13, AD_14, AD_15, AD_16,
    Moto_Goal, KP_S, KD_S, LIMIT_S, MID_SERVO, SERVO_1,SERVO_2,
    KP_M, KI_M, MOTOR_1, MOTOR_2, MOTOR_3, MOTOR_4,
    U_1, U_2, U_3, U_4, U_5,
    thre_1,thre_2,thre_3,thre_4,thre_5,thre_6,thre_7,thre_8,thre_9,thre_10
};

enum KEY   //��������������
{
    up_o = 1,  //4.0
    down_o,    //4.1
    left_o,    //4.2
    right_o,   //4.3
    ok_o,      //ͬʱ����4.2��4.3
    undo,    //����
};
typedef enum KEY key;

enum ITEMTYPE    //�˵��������
{
    list_item = 1, //�˵����ͣ������Ӳ˵��
    paraI_item,    //���Ͳ������ͣ���Ϊ�趨�������޸ģ���:angle_kp
    stateI_item,   //����״̬�����ͣ�������ʾ������������������޸ģ��磺angle_cur
    paraF_item,    //�����Ͳ�������
    stateF_item,   //������״̬������
    order_item,    //ָ�����ͣ��磺Hold��������
    read_only,
};
typedef enum ITEMTYPE itemtype;

union CARDATA   //�洢���ݵı������洢�˵��������Լ�С������
{
    int16 intData;
    float floatData;
};
typedef union CARDATA cardata;

struct ITEM
{
    uint16 ID;             //�������в˵����е����
    uint16 root_ID;        //�����˵������в˵����е����
    uint16 child_ID;       //����ǲ˵��Ͳ˵��child_ID��ʾ���ĵ�һ���˵����ID
    uint16 list_ID;        //���������˵��е����
    uint16 para_ID;        //����ǲ����Ͳ˵�����в�����ţ�����Ϊ0
    uint16 order_ID;       //�����ָ���Ͳ˵������ָ����ţ�����Ϊ0
    uint16 list_num;       //����ǲ˵��Ͳ˵��list_num��ʾ�����еĲ˵�����Ŀ
    itemtype item_type;    //�˵�������,ö���ͱ���
    char item_name[NAME_MAX];  //�˵��������
    cardata item_data;     //�˵�������ݣ������͵Ŀ��޸ģ�״̬�͵Ĳ����޸�
    int16 data_max;        //�˵������ݵ����ֵ
    int16 data_min;        //�˵������ݵ���Сֵ
};
typedef struct ITEM item;


item CreatItem(itemtype type, char* name, int16 min, int16 max);
void InsertItem(item* Item, uint16 root, uint16 item_t);
void CreatMenu(void);
void MenuInit(void);
void DataUpdate(void);
void PrintMenu(void);
key GetKey(void);
void KeyOperation(key Key);
void ModifyintData(void);
void ModifyfloatData(void);
void PrintintData(int mul,int now_);
void PrintfloatData(float mul,float now_);
void Save_data(void);
void Read_flash(void);
void key_start(void);
void Save_ADMAX(void);
void Read_AD(void);



#endif /* SOURCE_DEVELOP_MENU_H_ */
