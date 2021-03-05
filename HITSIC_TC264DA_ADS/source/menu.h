/*
 * menu.h
 *
 *  Created on: 2021年3月5日
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
#define Max_Item_Amount 100      //菜单项的最大个数

typedef union Car_data                //验收后改进：共用体型数据存储
{
    int     dataint;
    float   datafloat;
}Car_data;

extern uint8 Item_ID;
extern Car_data CAR[Max_Item_Amount];

typedef enum item_type          //菜单项类型
{
    intType = 1,                //整数型
    floatType,                  //浮点型
    listType,                   //菜单型（是一个子菜单的头节点）
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
    itemType    Item_type;            //菜单项类型
    uint8    list_ID;                 //菜单项在其直属菜单中的序号
    uint8    ID;                      //菜单项总序号
    int32     data_range[2];          //数据上下限
    Car_data   *Item_data;              //数据指针，指向结构体
    struct MenuItem_t *Pre_item;               //前一个节点指针
    struct MenuItem_t *Next_item;              //后一个节点指针
    struct MenuItem_t *Child_list;             //子菜单第一项的指针（只有菜单项类型是listType时才非空）
    char        Item_name[name_size];   //菜单项名称，屏幕显示
}MenuItem_t;

void MenuItem_Insert(MenuItem_t* Menu, MenuItem_t* pItem);

MenuItem_t *ItemCreate(char* Item_name, itemType Item_Type, int32 data_min, int32 data_max);

/**
 * @brief : 建立菜单树
 */
MenuItem_t *MenuCreate(void);

void MenuPrint(MenuItem_t *Menu, MenuItem_t *currItem);


uint8 ButtonRead(void);

MenuItem_t *ButtonProcess(MenuItem_t *Menu, MenuItem_t* currItem);

MenuItem_t *CursorMove_up(MenuItem_t* currItem);

MenuItem_t *CursorMove_down(MenuItem_t* currItem);

MenuItem_t *IntoMenu(MenuItem_t *currItem);

//获取当前操作的菜单项所属的菜单
MenuItem_t *GetRoot(MenuItem_t *currItem);

MenuItem_t *DataModify(MenuItem_t *currItem);

void ItemPrint(MenuItem_t *currItem, int32 pos, int32 *data_array, int32 length);

int32 ArrayToDataint(int32 *data_array, int32 length);



#endif /* SOURCE_MENU_H_ */
