/*
 * develop_menu.h
 *
 *  Created on: 2021年3月7日
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

#define NAME_MAX 20//菜单项名字长度的极限
#define ITEM_MAX 80//菜单项数目的极限
#define PARA_MAX 60//参量型菜单项数目的极限

extern uint16 ItemNum;
extern uint16 ParaNum;
extern uint16 OrderNum;
extern uint16 CurItem;
extern uint16 CurMenu;


//用于测试版本（单纯测试电机好不好用）
enum ITEMID//注意和ID一一对应***顺序不能搞错***
{
    MENU = 0,
    MODE = 1, AD_, STEER, MOTOR, UNUSE, HOLD,THRESHOLD,
    AD_1, AD_2, AD_3, AD_4, AD_5, AD_6,AD_7, AD_8, AD_9, AD_10, AD_11, AD_12,AD_13, AD_14, AD_15, AD_16,
    Moto_Goal, KP_S, KD_S, LIMIT_S, MID_SERVO, SERVO_1,SERVO_2,
    KP_M, KI_M, MOTOR_1, MOTOR_2, MOTOR_3, MOTOR_4,
    U_1, U_2, U_3, U_4, U_5,
    thre_1,thre_2,thre_3,thre_4,thre_5,thre_6,thre_7,thre_8,thre_9,thre_10
};

enum KEY   //按键操作的类型
{
    up_o = 1,  //4.0
    down_o,    //4.1
    left_o,    //4.2
    right_o,   //4.3
    ok_o,      //同时按下4.2和4.3
    undo,    //不按
};
typedef enum KEY key;

enum ITEMTYPE    //菜单项的类型
{
    list_item = 1, //菜单类型（含有子菜单项）
    paraI_item,    //整型参量类型（人为设定，可以修改，如:angle_kp
    stateI_item,   //整型状态量类型（传感器示数，或计算结果，不可修改，如：angle_cur
    paraF_item,    //浮点型参量类型
    stateF_item,   //浮点型状态量类型
    order_item,    //指令类型，如：Hold保存数据
    read_only,
};
typedef enum ITEMTYPE itemtype;

union CARDATA   //存储数据的变量，存储菜单项数据以及小车数据
{
    int16 intData;
    float floatData;
};
typedef union CARDATA cardata;

struct ITEM
{
    uint16 ID;             //处于所有菜单项中的序号
    uint16 root_ID;        //所处菜单在所有菜单项中的序号
    uint16 child_ID;       //如果是菜单型菜单项，child_ID表示它的第一个菜单项的ID
    uint16 list_ID;        //处于所处菜单中的序号
    uint16 para_ID;        //如果是参量型菜单项，则有参量序号，否则为0
    uint16 order_ID;       //如果是指令型菜单项，则有指令序号，否则为0
    uint16 list_num;       //如果是菜单型菜单项，list_num表示它含有的菜单项数目
    itemtype item_type;    //菜单项类型,枚举型变量
    char item_name[NAME_MAX];  //菜单项的名字
    cardata item_data;     //菜单项的数据，参量型的可修改，状态型的不可修改
    int16 data_max;        //菜单项数据的最大值
    int16 data_min;        //菜单项数据的最小值
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
