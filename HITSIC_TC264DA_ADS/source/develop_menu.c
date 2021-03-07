/*
 * develop_menu.c
 *
 *  Created on: 2021��3��7��
 *      Author: HP
 */
#include "develop_menu.h"

uint16 ItemNum = 0;   //�˵������Ŀ
uint16 ParaNum = 0;   //�����Ͳ˵������Ŀ
uint16 OrderNum = 0;  //ָ���Ͳ˵������Ŀ
uint16 CurItem = 1;   //��ǰ�˵����ID
uint16 CurMenu = MENU;//��ǰ�˵���ID����ʼ��Ϊ1
item Item[ITEM_MAX];  //�˵���ṹ������
cardata CAR[PARA_MAX];


item CreatItem(itemtype type, char* name, int32 min, int32 max)
{
    item Item;
    Item.item_type = type;
    strncpy(Item.item_name, name, NAME_MAX);
    Item.data_max = max;
    Item.data_min = min;
    Item.root_ID = 0;
    Item.child_ID = 0;
    Item.list_ID = 0;
    Item.list_num = 0;
    Item.para_ID = 0;
    Item.order_ID = 0;
    Item.ID = ItemNum++;                          //MENU���ID��0
    if(type == order_item)                        //ָ���Ͳ˵���
    {
        Item.order_ID = ++OrderNum;                 // ָ��˵�������һ
    }
    else
    {
      if(type == paraI_item || type == paraF_item)//�����Ͳ˵���
      {
          Item.para_ID = ++ParaNum;                 //ָ��˵�������һ
          Item.item_data = CAR[ParaNum];            //��С�������ݸ������Ͳ˵���Բ����Ͳ˵������ݽ��г�ʼ����
      }
  }
    return Item;
}

void InsertItem(item* Item, uint16 root, uint16 item_t)
{
    if(Item[root].child_ID == 0)
    {
        Item[root].child_ID = Item[item_t].ID;
    }
    Item[item_t].root_ID = Item[root].ID;
    Item[item_t].list_ID = ++Item[root].list_num;
}

void CreatMenu(void)//**����������һ��������Ҫ����һ����ITEMIDö����***
{
    Item[MENU] = CreatItem(list_item, "MENU", 0, 1);
    Item[MODE] = CreatItem(paraI_item, "Mode", 0, 2);
    Item[POSITION] = CreatItem(list_item, "Position", 0, 1);
    Item[PID_K] = CreatItem(list_item, "Balance", 0, 1);
    Item[SPEED] = CreatItem(list_item, "Speed", 0, 1);
    Item[DIR] = CreatItem(list_item, "Dir", 0, 1);
    Item[HOLD] = CreatItem(order_item, "Hold", 0, 1);//����˵���
    InsertItem(Item, MENU, MODE);
    InsertItem(Item, MENU, POSITION);
    InsertItem(Item, MENU, PID_K);
    InsertItem(Item, MENU, SPEED);
    InsertItem(Item, MENU, DIR);
    InsertItem(Item, MENU, HOLD);
    Item[GYRO_X] = CreatItem(stateF_item, "gyro_x", 0, 1);
    Item[GYRO_Y] = CreatItem(stateF_item, "gyro_y", 0, 1);
    Item[GYRO_Z] = CreatItem(stateF_item, "gyro_z", 0, 1);
    Item[ACC_X] = CreatItem(stateF_item, "acc_x", 0, 1);
    Item[ACC_Y] = CreatItem(stateF_item, "acc_y", 0, 1);
    Item[ACC_Z] = CreatItem(stateF_item, "acc_z", 0, 1);
    InsertItem(Item, POSITION, GYRO_X);
    InsertItem(Item, POSITION, GYRO_Y);
    InsertItem(Item, POSITION, GYRO_Z);
    InsertItem(Item, POSITION, ACC_X);
    InsertItem(Item, POSITION, ACC_Y);
    InsertItem(Item, POSITION, ACC_Z);
    Item[ANGLE_SET] = CreatItem(paraF_item, "angle_set", -90, 90);
    Item[ANGLE_CUR] = CreatItem(stateF_item, "angle_cur", -90, 90);
    Item[ANGLE_KP] = CreatItem(paraF_item, "angle_kp", 0, 20);
    Item[ANGLE_KI] = CreatItem(paraF_item, "angle_ki", 0, 20);
    Item[ANGLE_KD] = CreatItem(paraF_item, "angle_kd", 0, 20);
    Item[ANGLE_OUT] = CreatItem(stateF_item, "angle_out", -500, 500);
    InsertItem(Item, PID_K, ANGLE_SET);
    InsertItem(Item, PID_K, ANGLE_CUR);;
    InsertItem(Item, PID_K, ANGLE_KP);
    InsertItem(Item, PID_K, ANGLE_KI);
    InsertItem(Item, PID_K, ANGLE_KD);
    InsertItem(Item, PID_K, ANGLE_OUT);
    Item[SPEED_SET] = CreatItem(paraF_item, "speed_set", -10000, 10000);
    Item[SPEED_CUR] = CreatItem(stateF_item, "speed_cur", -1000, 1000);
    Item[SPEED_KP] = CreatItem(paraF_item, "speed_kp", 0, 50);
    Item[SPEED_KI] = CreatItem(paraF_item, "speed_ki", 0, 50);
    Item[SPEED_KD] = CreatItem(paraF_item, "speed_kd", 0, 50);
    Item[SPEED_MOTOR] = CreatItem(stateF_item, "speed_mot", -500, 500);
    InsertItem(Item, SPEED, SPEED_SET);
    InsertItem(Item, SPEED, SPEED_CUR);
    InsertItem(Item, SPEED, SPEED_KP);
    InsertItem(Item, SPEED, SPEED_KI);
    InsertItem(Item, SPEED, SPEED_KD);
    InsertItem(Item, SPEED, SPEED_MOTOR);
    Item[DIR_KP] = CreatItem(paraF_item, "dir_kp", -20, 20);
    Item[DIR_KI] = CreatItem(paraF_item, "dir_ki", -20, 20);
    Item[DIR_KD] = CreatItem(paraF_item, "dir_kd", -20, 20);
    Item[DIR_MID] = CreatItem(paraF_item, "dir_kp_mid", -20, 20);
    Item[DIR_OUT] = CreatItem(stateF_item, "dir_out", -500, 500);
    InsertItem(Item, DIR, DIR_KP);
    InsertItem(Item, DIR, DIR_KI);
    InsertItem(Item, DIR, DIR_KD);
    InsertItem(Item, DIR, DIR_MID);
    InsertItem(Item, DIR, DIR_OUT);
}

//void MenuInit(void)
//{
//
//}

//-------------------------------------------------------------------------------------------------------------------
//  @brief       ��ӡ�˵�
//  @param
//  @attention   ���յ�ǰ�˵�ID(CurMenu)�͵�ǰ�˵���ID(CurItem)��ӡ�˵�
//  @return      void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void PrintMenu(void)
{
    uint8 x, y;
    uint16 start_site;
    uint16 print_ID;
    uint16 end_ID;
    start_site = (Item[CurItem].list_ID <= 6) ? 1 : Item[CurItem].list_ID - 5;                                  //�˵��д�ӡ��ʼ���λ��
    print_ID = Item[CurMenu].child_ID + (start_site - 1);                                                                           //��ӡ��ʼ���λ�õ�ID
    end_ID = Item[CurMenu].child_ID + (Item[CurMenu].list_num - 1);                                                         //�ò˵����һ���ID
//    DataUpdate();                                                                                                                                                               //���ȸ���һ������
    SmartCar_OLED_Fill(0);                                                                                                                                                       //����
    SmartCar_OLED_P6x8Str(0, 0, Item[CurMenu].item_name);
    SmartCar_OLED_P6x8Str(0, 1, "----------------------");
    x = 6;
    y = 2;                                                                                                                                                                          //�˵���Ŀ�ʼ��ʾλ��
    while(y <= 7 && print_ID <= end_ID)
 {
     SmartCar_OLED_P6x8Str(x, y, Item[print_ID].item_name);                                                                                  //��ӡ�˵�������
     if(Item[print_ID].item_type != list_item && Item[print_ID].item_type != order_item)                //��ӡ���ݲ˵�������
     {
         if(Item[print_ID].item_type == stateI_item || Item[print_ID].item_type == paraI_item)    //��ӡ��������
             SmartCar_OLED_Printf6x8(66, y,"%d",Item[print_ID].item_data.intData);
         else
             SmartCar_OLED_Printf6x8(66, y,"%.4f",Item[print_ID].item_data.floatData);
     }
     print_ID++;
     y++;
 }
    SmartCar_OLED_P6x8Str(0, (Item[CurItem].list_ID + 1 <= 7) ? Item[CurItem].list_ID + 1 : 7, ">");  //��ӡλ��ָʾ��
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief         ���´�ӡ�˵�ʱ�����ݽ��и���
//  @param
//  @attention     �˺������ڴ�ӡ�˵�PrintMenu()������ڴ�ӡ�˵�ǰ����һ�²˵��������Լ����޸ĵĲ����Ͳ˵������ݴ���С�����ݺ�ʵ�ʲ�������
//  @return
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void DataUpdate(void)
{

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief        ��ⰴ��
//  @param
//  @attention    ͬʱ�������Ҽ���okʹ�ã�Ŀǰok�����ڱ������ݣ�ÿ��150ms���һ�ΰ���
//  @return       key
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
key GetKey(void)
{
    key key_oper = undo;
    if(!GPIO_Read(P22,0)||!GPIO_Read(P22,1)||!GPIO_Read(P22,2)||!GPIO_Read(P22,3))
    {
        Delay_ms(STM0,150);
        if(!GPIO_Read(P22,0))
        {
            key_oper = up_o;
        }
        if(!GPIO_Read(P22,1))
        {
            key_oper = down_o;
        }
        if(!GPIO_Read(P22,2))
        {
            key_oper = left_o;
        }
        if(!GPIO_Read(P22,3))
        {
            key_oper = right_o;
        }
    }
    return key_oper;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief        �˵�����İ�������
//  @param
//  @attention    ���¼����������ƶ���겢�޸ĵ�ǰ�˵���ID ��CurItem�������������һ���˵����Ҽ������Ӳ˵����޸ĵ�ǰ�˵�ID��CurMenu)�͵�ǰ�˵���ID��CurItem��
//                ****�ڵ�ǰ�˵��CurItem���ǲ������Ͳ˵���ʱ���Ҽ���������������޸Ľ���****
//                ****�ڵ�ǰ�˵��CurItem����Hold�˵���ʱ���Ҽ���������****
//  @return       void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void KeyOperation(key Key)
{
    switch (Key)
    {
        case up_o:
            if(Item[CurItem].list_ID > 1)
            {
                CurItem--;
            }
            break;
        case down_o:
            if(Item[CurItem].list_ID < Item[CurMenu].list_num)
            {
                CurItem++;
            }
            break;
        case left_o:
            if(CurMenu != MENU)
            {
                CurMenu = Item[CurMenu].root_ID;
            CurItem = Item[CurItem].root_ID;
            }
          break;
        case right_o:
            if(Item[CurItem].item_type == list_item)
            {
                CurMenu = CurItem;
            CurItem = Item[CurItem].child_ID;
            }
            else if(CurItem == HOLD)
            {
                GPIO_Set(P20,9, 0);
                Delay_ms(STM0, 150);
                GPIO_Set(P20,9, 1);
//                iap_erase_page(0x01);
//                iap_write_bytes(0x01, &CAR[0], PARA_MAX * sizeof(cardata));                            //�����޸ĵĲ���
            }
            else if(Item[CurItem].item_type == paraI_item || Item[CurItem].item_type == paraF_item)//�����޸Ĳ�������
            {
                if(Item[CurItem].item_type == paraI_item)
                ModifyintData();
                if(Item[CurItem].item_type == paraF_item)
                ModifyfloatData();
            }
            break;
        default:
            break;
    }
}

void ModifyintData(void);
{

}





