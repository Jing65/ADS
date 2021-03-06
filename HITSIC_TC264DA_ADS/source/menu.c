/*
 * menu.c
 *
 *  Created on: 2021年3月5日
 *      Author: HP
 */
#include "menu.h"

uint8 Item_ID=0;
Car_data CAR[Max_Item_Amount];
void MenuItem_Insert(MenuItem_t* Menu, MenuItem_t* pItem)
{
    if (Menu->Child_list == NULL)
    {
        Menu->Child_list = pItem;
        pItem->list_ID = 1;
        pItem->Pre_item = Menu;
        Menu->Item_data->dataint = 1;
    }
    else
    {
        MenuItem_t* pctrl = Menu->Child_list;
        while(pctrl->Next_item != NULL)
        {
            pctrl = pctrl->Next_item;
        }
        pctrl->Next_item = pItem;
        pItem->Pre_item = pctrl;
        pItem->list_ID   = pctrl->list_ID + 1;
        Menu->Item_data->dataint++;
    }
}

MenuItem_t *ItemCreate(char* Item_name, itemType Item_Type, int32 data_min, int32 data_max)
{
    MenuItem_t* pItem = (MenuItem_t*)malloc(sizeof(MenuItem_t));
    pItem->Item_type = Item_Type;
    strncpy(pItem->Item_name, Item_name, name_size);
    pItem->Item_name[name_size - 1] = '\0';
    if (Item_ID < Max_Item_Amount)
    {
        pItem->ID = Item_ID;
        Item_ID++;
    }
    pItem->data_range[min] = data_min;
    pItem->data_range[max] = data_max;
//    pItem->Item_data = &CAR[Item_ID-1];

    if (Item_Type == intType)
    {
        pItem->Item_data = &CAR[Item_ID-1].dataint;
    }else if (Item_Type == floatType)
    {
        pItem->Item_data = &CAR[Item_ID-1].datafloat;
    }
    //主菜单头节点是0
    pItem->list_ID = 0;
    pItem->Pre_item = NULL;
    pItem->Next_item = NULL;
    pItem->Child_list = NULL;
    return pItem;
}

MenuItem_t *MenuCreate(void)
{
    MenuItem_t* MenuRoot = ItemCreate("Main Menu", listType, 0, 20);
//    MenuItem_Insert(MenuRoot, ItemCreate("SAVE", listType, 0, 20)); //特殊项  ID = 0
    MenuItem_Insert(MenuRoot, ItemCreate("ASens1", floatType, 0, 10));
    MenuItem_Insert(MenuRoot, ItemCreate("ASens2", floatType, 0, 10));
    MenuItem_Insert(MenuRoot, ItemCreate("DSens1", intType, 0, 50));
    MenuItem_Insert(MenuRoot, ItemCreate("DSens2", intType, 0, 50));
    MenuItem_Insert(MenuRoot, ItemCreate("Camera", intType, 0, 50));
    MenuItem_Insert(MenuRoot, ItemCreate("Motor", floatType, 0, 100));

    return MenuRoot;
}

void MenuPrint(MenuItem_t *Menu, MenuItem_t *currItem)  //再加一项当前指针
{
    SmartCar_OLED_Fill(0);
    uint8 x = 0;
    uint8 y = 2;   //菜单项在y = 2开始
    SmartCar_OLED_Printf6x8(0, 0,"%s", Menu->Item_name);
    SmartCar_OLED_P6x8Str(0, 1,"---------------------");
    uint8 start_print_ID;
    MenuItem_t *pctrl = Menu->Child_list;
    start_print_ID = currItem->list_ID <= 6? 1:currItem->list_ID - 5;   //滚动式菜单，
    while (pctrl->list_ID != start_print_ID)        //从哪一项开始打印，需要改，光标移到最后再往上移有问题。
    {
        pctrl = pctrl->Next_item;
    }
    while (pctrl != NULL && y <= 7)
    {
        if (pctrl->Item_type == listType)
            SmartCar_OLED_Printf6x8(6, y,"[%s]", pctrl->Item_name);
        else if (pctrl->Item_type == intType)
        {
            SmartCar_OLED_Printf6x8(6, y,"%s", pctrl->Item_name);
            SmartCar_OLED_Printf6x8(66, y,"%6d", pctrl->Item_data->dataint);
        }
        else if (pctrl->Item_type == floatType)
        {
            SmartCar_OLED_Printf6x8(6, y,"%s", pctrl->Item_name);
            SmartCar_OLED_Printf6x8(66, y,"%6.2f", pctrl->Item_data->datafloat);
        }
        y++;
        pctrl = pctrl->Next_item;
    }
    SmartCar_OLED_Printf6x8(0, currItem->list_ID + 1 >= 7? 7: currItem->list_ID + 1,">");
}

uint8 ButtonRead(void)
{
    uint8 button_operation = 0;
    Delay_ms(STM0, 200);
    if (!GPIO_Read(P22,0))
    {
        button_operation = up;
    }
    if (!GPIO_Read(P22,1))
    {
        button_operation = down;
    }
    if (!GPIO_Read(P22,2))
    {
        button_operation = left;
    }
    if (!GPIO_Read(P22,3))
    {
        button_operation = OK;
    }

    return button_operation;
}


MenuItem_t *ButtonProcess(MenuItem_t *Menu, MenuItem_t* currItem)
{
    uint8 button_order = ButtonRead();
    switch (button_order)
    {
        case up:
            currItem = CursorMove_up(currItem);
            MenuPrint(Menu, currItem);
            break;
        case down:
            currItem = CursorMove_down(currItem);
            MenuPrint(Menu, currItem);
            break;
        case OK:
            if  (currItem->ID == save)
            {
//                memcpy(&buff[0], CAR, sizeof(Car_data) * Max_Item_Amount);
//                FLASH_SectorWrite(0, buff);
//                MenuPrint(Menu, currItem);
            }
            else if (currItem->Item_type == listType)
                currItem = IntoMenu(currItem);
            else
            {
                currItem = DataModify(currItem);
                MenuPrint(Menu, currItem);
            }
            break;
        default:
            break;
    }
    return currItem;
}

MenuItem_t *CursorMove_up(MenuItem_t* currItem)
{
    if (currItem->list_ID > 1)
    {
        currItem = currItem->Pre_item;
    }
    return currItem;
}

MenuItem_t *CursorMove_down(MenuItem_t* currItem)
{
    if (currItem->Next_item != NULL)
    {
        currItem = currItem->Next_item;
    }
    return currItem;
}

MenuItem_t *IntoMenu(MenuItem_t *currItem)
{
    if (currItem->list_ID == 1)     //[Back]
    {
        currItem = currItem->Pre_item;
        MenuPrint(GetRoot(currItem), currItem);
    }
    else
    {
        MenuPrint(currItem, currItem->Child_list);
        currItem = currItem->Child_list;
    }
    return currItem;
}

MenuItem_t *GetRoot(MenuItem_t *currItem)       //获取当前操作的菜单项所属的菜单
{
    MenuItem_t *pctrl = currItem;
    if (pctrl->list_ID == 0)
        return pctrl;
    while(pctrl->list_ID > 1)
    {
        pctrl = pctrl->Pre_item;
    }
    return pctrl->Pre_item;
}

MenuItem_t *DataModify(MenuItem_t *currItem)
{
    int32 max_digit;
    int32 i;
    max_digit = (currItem->Item_type == intType) ? 0 : 2;   //所有浮点型均为两位小数
    int32 data_max = currItem->data_range[max];
    while(data_max != 0)
    {
        data_max /= 10;
        max_digit++;
    }                       //获得最大数位

    int32 data_array[max_digit];
    int32 Item_dataint = (currItem->Item_type == intType) ? currItem->Item_data->dataint : floor(currItem->Item_data->datafloat * 100);
    for(i = 0; i < max_digit; i++)
    {
        data_array[max_digit - 1 - i] = Item_dataint % ((i+1)*10);
        Item_dataint /= 10;
    }
    ItemPrint(currItem, 1, data_array, max_digit);
    int32 pos = 1;        //pos:光标位置
    while(1)
    {
        uint8 flag_return = 0;
        uint8 button_order = ButtonRead();
        switch(button_order)
        {
            case left:          //光标左移
                if(pos > 1)
                {
                    pos--;
                    ItemPrint(currItem, pos, data_array, max_digit);
                }
                break;
//            case right:
//                if(pos < max_digit + 2)
//                {
//                    pos++;
//                    ItemPrint(currItem, pos, data_array, max_digit);
//                }
//                break;
            case up:            //光标指向的数位+1
                if(pos <= max_digit && data_array[pos-1] < 9)
                {
                    data_array[pos-1]++;
                    ItemPrint(currItem, pos, data_array, max_digit);
                }
                break;
            case down:
                if(pos >= 1 && data_array[pos-1] > 0)
                {
                    data_array[pos-1]--;
                    ItemPrint(currItem, pos, data_array, max_digit);
                }
                break;
            case OK:
                if (pos == max_digit + 1)       //[OK]
                {
                    Item_dataint = ArrayToDataint(data_array, max_digit);
                    if (currItem->Item_type == intType)
                        currItem->Item_data->dataint = Item_dataint;
                    else  currItem->Item_data->datafloat = (float)Item_dataint / 100.0;
                    flag_return = 1;
                    break;
                }
                if (pos == max_digit + 2)           //[cancel]，返回上一级菜单
                {
                    flag_return = 1;
                }
                break;
            default:
                break;
        }
        if (flag_return) break;
    }
    return currItem;
}

void ItemPrint(MenuItem_t *currItem, int32 pos, int32 *data_array, int32 length)
{
    SmartCar_OLED_Fill(0);
    int32 max_digit = length;//sizeof(data_array) / sizeof(int32_t);

    SmartCar_OLED_Printf6x8(0, 0,"  %s   [%d,%d]", currItem->Item_name, currItem->data_range[min], currItem->data_range[max]);
    SmartCar_OLED_P6x8Str(0, 1,"---------------------");

    uint8 i;
    for(i = 0; i < (uint8)max_digit; i++)
    {
        SmartCar_OLED_Printf6x8(42 + i*6, 3, "%d", (uint8)data_array[i]);     //显示各个数位
    }
    if (currItem->Item_type == floatType)
    {
        SmartCar_OLED_P6x8Str(42 + i*6, 3, "e-2");
    }

    SmartCar_OLED_P6x8Str(12, 7, "[OK]");
    SmartCar_OLED_P6x8Str(60, 7, "[Cancel]");
    if  (pos == max_digit + 1)
    {
        SmartCar_OLED_P6x8Str(6, 7, ">");
    }
    else if (pos == max_digit + 2)
    {
        SmartCar_OLED_P6x8Str(54, 7, ">");
    }
    else
    {
        SmartCar_OLED_P6x8Str(42 + (pos-1)*6, 4, "^");
    }

}

int32 ArrayToDataint(int32 *data_array, int32 length)
{
    int32 Item_dataint = 0;
    int32 max_digit = length;
    for (uint8 i = 0; i < (uint8)max_digit; i++)
    {
        Item_dataint *= 10;
        Item_dataint += data_array[i];
    }
    return Item_dataint;
}





