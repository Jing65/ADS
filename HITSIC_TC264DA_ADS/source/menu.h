/*
 * menu.h
 *
 *  Created on: 2021��3��5��
 *      Author: HP
 */

#ifndef SOURCE_MENU_H_
#define SOURCE_MENU_H_



#include "Ifx_Types.h"
#include "common.h"
#include "SmartCar_GPIO.h"
#include "SmartCar_Systick.h"
#include "stdlib.h"

#define name_size 16u
#define Max_Item_Amount 100      //�˵����������

typedef union Car_data                //���պ�Ľ��������������ݴ洢
{
    int     dataint;
    float   datafloat;
}Car_data;

extern uint8 Item_ID;
extern Car_data CAR[Max_Item_Amount];

typedef enum item_type          //�˵�������
{
    intType = 1,                //������
    floatType,                  //������
    listType,                   //�˵��ͣ���һ���Ӳ˵���ͷ�ڵ㣩
}itemType;



typedef enum range
{
    min = 0,
    max,
}range;

typedef enum button_order
{
    up = 1,
    down,
    left,
    right,
    OK,
}button_order;

typedef enum special_id
{
    save = 0,
}special_id;


typedef struct MenuItem_t
{
    itemType    Item_type;            //�˵�������
    uint8    list_ID;                 //�˵�������ֱ���˵��е����
    uint8    ID;                      //�˵��������
    int32     data_range[2];          //����������
    Car_data   *Item_data;              //����ָ�룬ָ��ṹ��
    struct MenuItem_t *Pre_item;               //ǰһ���ڵ�ָ��
    struct MenuItem_t *Next_item;              //��һ���ڵ�ָ��
    struct MenuItem_t *Child_list;             //�Ӳ˵���һ���ָ�루ֻ�в˵���������listTypeʱ�ŷǿգ�
    char        Item_name[name_size];   //�˵������ƣ���Ļ��ʾ
}MenuItem_t;

void MenuItem_Insert(MenuItem_t* Menu, MenuItem_t* pItem);

MenuItem_t *ItemCreate(char* Item_name, itemType Item_Type, int32 data_min, int32 data_max);

/**
 * @brief : �����˵���
 */
MenuItem_t *MenuCreate(void);

void MenuPrint(MenuItem_t *Menu, MenuItem_t *currItem);


uint8 ButtonRead(void);

MenuItem_t *ButtonProcess(MenuItem_t *Menu, MenuItem_t* currItem);

MenuItem_t *CursorMove_up(MenuItem_t* currItem);

MenuItem_t *CursorMove_down(MenuItem_t* currItem);

MenuItem_t *IntoMenu(MenuItem_t *currItem);

//��ȡ��ǰ�����Ĳ˵��������Ĳ˵�
MenuItem_t *GetRoot(MenuItem_t *currItem);

MenuItem_t *DataModify(MenuItem_t *currItem);

void ItemPrint(MenuItem_t *currItem, int32 pos, int32 *data_array, int32 length);

int32 ArrayToDataint(int32 *data_array, int32 length);



#endif /* SOURCE_MENU_H_ */
