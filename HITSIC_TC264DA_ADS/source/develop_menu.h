/*
 * develop_menu.h
 *
 *  Created on: 2021��3��7��
 *      Author: HP
 */

#ifndef SOURCE_DEVELOP_MENU_H_
#define SOURCE_DEVELOP_MENU_H_

#include "common.h"
#include "SmartCar_Oled.h"
#include "SmartCar_Systick.h"
#include "SmartCar_GPIO.h"


#define NAME_MAX 20//�˵������ֳ��ȵļ���
#define ITEM_MAX 40//�˵�����Ŀ�ļ���
#define PARA_MAX 30//�����Ͳ˵�����Ŀ�ļ���


extern uint16 ItemNum;
extern uint16 ParaNum;
extern uint16 OrderNum;
extern uint16 CurItem;
extern uint16 CurMenu;


enum ITEMID//ע���IDһһ��Ӧ***˳���ܸ��***
{
    MENU = 0,
    MODE = 1, POSITION, PID_K, SPEED, DIR, HOLD,
    GYRO_X, GYRO_Y, GYRO_Z, ACC_X, ACC_Y, ACC_Z,
    ANGLE_SET, ANGLE_CUR, ANGLE_KP, ANGLE_KI, ANGLE_KD, ANGLE_OUT,
    SPEED_SET, SPEED_CUR, SPEED_KP, SPEED_KI, SPEED_KD, SPEED_MOTOR,
    DIR_KP, DIR_KI, DIR_KD, DIR_MID, DIR_OUT,
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

struct CARDATA   //�洢���ݵı������洢�˵��������Լ�С������
{
    int16 intData;
    float floatData;
};
typedef struct CARDATA cardata;

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

extern item Item[ITEM_MAX];
extern cardata CAR[PARA_MAX];


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







#endif /* SOURCE_DEVELOP_MENU_H_ */
