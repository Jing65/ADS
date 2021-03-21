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
static item Item[ITEM_MAX];  //�˵���ṹ������
static uint32 CAR_BUFFER[PARA_MAX];//flash�����������ݵ�����

void key_start(void)
{
    if(!GPIO_Read(P20,10))
    {
        key get_key=GetKey();
        KeyOperation(get_key);
        if(get_key!=undo)
        PrintMenu();
        if(!GPIO_Read(P20,12))
        PrintMenu();
    }
    if(If_Start == 0)
    {
           if(!GPIO_Read(P20,0))
          {
             Delay_ms(STM1,3000);
             If_Start = 1;
          }
    }
    if(If_Start == 1)
    {
           if(!GPIO_Read(P20,8))
          {
             Delay_ms(STM1,200);
             If_Start = 0;
          }
    }
}


item CreatItem(itemtype type, char* name, int16 min, int16 max)
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
    Item.ID = (ItemNum++);                          //MENU���ID��0
    if(type == order_item)                        //ָ���Ͳ˵���
    {
        Item.order_ID = (++OrderNum);                 // ָ��˵�������һ
    }
    else
    {
      if(type == paraI_item || type == paraF_item)//�����Ͳ˵���
      {
          cardata car_init;//ָ��˵�������һ
          Item.para_ID = (ParaNum++);                               //��С�������ݸ������Ͳ˵���Բ����Ͳ˵������ݽ��г�ʼ����
          Item.item_data=car_init;
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
    Item[item_t].list_ID = (++Item[root].list_num);
}





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
    uint16 print_ID=0;
    uint16 end_ID;
    start_site = (Item[CurItem].list_ID <= 6) ? 1 : Item[CurItem].list_ID - 5;                                  //�˵��д�ӡ��ʼ���λ��
    print_ID = Item[CurMenu].child_ID + (start_site - 1);                                                                           //��ӡ��ʼ���λ�õ�ID
    end_ID = Item[CurMenu].child_ID + (Item[CurMenu].list_num - 1);                                                         //�ò˵����һ���ID
    DataUpdate();                                                                                                                                                               //���ȸ���һ������
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
void CreatMenu(void)//**����������һ��������Ҫ����һ����ITEMIDö����***
{
    Item[MENU] = CreatItem(list_item, "MENU", 0, 1);
    Item[MODE] = CreatItem(paraI_item, "EXAMPLE", 0, 1);
    Item[AD_] = CreatItem(list_item, "AD_read", 0, 1);
    Item[STEER] = CreatItem(list_item, "steer&speed", 0, 1);
    Item[MOTOR] = CreatItem(list_item, "MOTOR", 0, 1);
    Item[UNUSE] = CreatItem(list_item, "flag", 0, 1);
    Item[HOLD] = CreatItem(order_item, "Hold", 0, 1);//����˵���
    InsertItem(Item, MENU, MODE);
    InsertItem(Item, MENU, AD_);
    InsertItem(Item, MENU, STEER);
    InsertItem(Item, MENU, MOTOR);
    InsertItem(Item, MENU, UNUSE);
    InsertItem(Item, MENU, HOLD);
//    Item[AD_1] = CreatItem(stateF_item, "AD_1", 0, 5000);
//    Item[AD_2] = CreatItem(stateF_item, "AD_2", 0, 5000);
//    Item[AD_3] = CreatItem(stateF_item, "AD_3", 0, 5000);
//    Item[AD_4] = CreatItem(stateF_item, "AD_4", 0, 5000);
//    Item[AD_5] = CreatItem(stateF_item, "AD_5", 0, 5000);
//    Item[AD_6] = CreatItem(stateF_item, "AD_6", 0, 5000);
//    Item[AD_7] = CreatItem(stateF_item, "AD_7", 0, 5000);
//    Item[AD_8] = CreatItem(stateF_item, "AD_8", 0, 5000);
//    Item[AD_9] = CreatItem(stateF_item, "AD_9", 0, 5000);
//    Item[AD_10] = CreatItem(stateF_item, "AD_10", 0, 5000);
//    Item[AD_11] = CreatItem(stateF_item, "AD_11", 0, 5000);
//    Item[AD_12] = CreatItem(stateF_item, "AD_12", 0, 5000);
//    Item[AD_13] = CreatItem(stateF_item, "AD_13", 0, 5000);
//    Item[AD_14] = CreatItem(stateF_item, "AD_14", 0, 5000);
//    Item[AD_15] = CreatItem(stateF_item, "AD_15", 0, 5000);
//    Item[AD_16] = CreatItem(stateF_item, "AD_16", 0, 5000);
    Item[AD_1] = CreatItem(stateF_item, "AD_1", 0, 400);
    Item[AD_2] = CreatItem(stateF_item, "AD_2", 0, 400);
    Item[AD_3] = CreatItem(stateF_item, "AD_3", 0, 400);
    Item[AD_4] = CreatItem(stateF_item, "AD_4", 0, 400);
    Item[AD_5] = CreatItem(stateF_item, "AD_5", 0, 400);
    Item[AD_6] = CreatItem(stateF_item, "AD_6", 0, 400);
    Item[AD_7] = CreatItem(stateF_item, "AD_7", 0, 400);
    Item[AD_8] = CreatItem(stateF_item, "AI_left2", 0, 400);
    Item[AD_9] = CreatItem(stateF_item, "AI_left1", 0, 400);
    Item[AD_10] = CreatItem(stateF_item, "AI_MID", 0, 400);
    Item[AD_11] = CreatItem(stateF_item, "AI_LM", 0, 400);
    Item[AD_12] = CreatItem(stateF_item, "AI_MID", 0, 400);
    Item[AD_13] = CreatItem(stateF_item, "AI_RM", 0, 400);
    Item[AD_14] = CreatItem(stateF_item, "AI_Rshu", 0, 400);
    Item[AD_15] = CreatItem(stateF_item, "AI_right1", 0, 400);
    Item[AD_16] = CreatItem(stateF_item, "AI_rihrt2", 0, 400);
    InsertItem(Item, AD_, AD_1);
    InsertItem(Item, AD_, AD_2);
    InsertItem(Item, AD_, AD_3);
    InsertItem(Item, AD_, AD_4);
    InsertItem(Item, AD_, AD_5);
    InsertItem(Item, AD_, AD_6);
    InsertItem(Item, AD_, AD_7);
    InsertItem(Item, AD_, AD_8);
    InsertItem(Item, AD_, AD_9);
    InsertItem(Item, AD_, AD_10);
    InsertItem(Item, AD_, AD_11);
    InsertItem(Item, AD_, AD_12);
    InsertItem(Item, AD_, AD_13);
    InsertItem(Item, AD_, AD_14);
    InsertItem(Item, AD_, AD_15);
    InsertItem(Item, AD_, AD_16);
    Item[Moto_Goal] = CreatItem(paraF_item, "Moto_Goal", 0, 4);
    Item[KP_S] = CreatItem(paraF_item, "KP_S", 0,90);
    Item[KD_S] = CreatItem(paraF_item, "KD_S", 0,90);
    Item[LIMIT_S] = CreatItem(paraF_item, "LIMIT_S", 0, 3);
    Item[MID_SERVO] = CreatItem(paraF_item, "mid_servo", 6, 8);
    Item[SERVO_1] = CreatItem(stateF_item, "RXXX", -500, 500);
    Item[SERVO_2] = CreatItem(stateF_item, "RXXX", -500, 500);
    InsertItem(Item, STEER, Moto_Goal);
    InsertItem(Item, STEER, KP_S);;
    InsertItem(Item, STEER, KD_S);
    InsertItem(Item, STEER, LIMIT_S);
    InsertItem(Item, STEER, MID_SERVO);
    InsertItem(Item, STEER, SERVO_1);
    InsertItem(Item, STEER, SERVO_2);
    Item[KP_M] = CreatItem(paraF_item, "KP_M", 0, 1000);
    Item[KI_M] = CreatItem(paraF_item, "KI_M", 0, 1000);
    Item[MOTOR_1] = CreatItem(paraI_item, "XXXX", -10000, 10000);
    Item[MOTOR_2] = CreatItem(paraF_item, "XXXX", 0, 50);
    Item[MOTOR_3] = CreatItem(stateI_item, "RXXX", -30000, 30000);
    Item[MOTOR_4] = CreatItem(stateF_item, "RXXX", -500, 500);
    InsertItem(Item, MOTOR, KP_M);
    InsertItem(Item, MOTOR, KI_M);
    InsertItem(Item, MOTOR, MOTOR_1);
    InsertItem(Item, MOTOR, MOTOR_2);
    InsertItem(Item, MOTOR, MOTOR_3);
    InsertItem(Item, MOTOR, MOTOR_4);
    Item[U_1] = CreatItem(paraI_item, "collect_max", 0, 1);
    Item[U_2] = CreatItem(paraI_item, "process_ai", 0, 1);
    Item[U_3] = CreatItem(paraF_item, "XXXX", -20, 20);
    Item[U_4] = CreatItem(paraF_item, "XXXX", -20, 20);
    Item[U_5] = CreatItem(stateF_item, "RXXX", -500, 500);
    InsertItem(Item, UNUSE, U_1);
    InsertItem(Item, UNUSE, U_2);
    InsertItem(Item, UNUSE, U_3);
    InsertItem(Item, UNUSE, U_4);
    InsertItem(Item, UNUSE, U_5);
}

void MenuInit(void)
{
    Item[Moto_Goal].item_data.floatData=Moto_Speed_Goal_Set;
    Item[MID_SERVO].item_data.floatData=servo_mid;
    Item[KP_S].item_data.floatData=KP_S_E;
    Item[KD_S].item_data.floatData=KD_S_E;
    Item[KP_M].item_data.floatData=KP_m;
    Item[KI_M].item_data.floatData=KI_m;
    Item[LIMIT_S].item_data.floatData=LIMIT_SE;
    Item[U_1].item_data.intData=collect_max_flag;
    Item[U_2].item_data.intData=process_type_ai;
}

void Read_flash(void)
{
    Moto_Speed_Goal_Set=Page_Read(0,0,float);
    KP_S_E=Page_Read(0,1,float);
    KD_S_E=Page_Read(0,2,float);
    LIMIT_SE=Page_Read(0,3,float);
    servo_mid=Page_Read(0,4,float);
    KP_m=Page_Read(0,5,float);
    KI_m=Page_Read(0,6,float);
    collect_max_flag=Page_Read(0,7,uint8);
    process_type_ai=Page_Read(0,8,uint8);
}

void DataUpdate(void)
{
    for(uint8 i=0;i<16;i++)
    {
      Item[(i+AD_1)].item_data.floatData =AD[i];
    }
     Moto_Speed_Goal_Set=Item[Moto_Goal].item_data.floatData;
     KP_m=Item[KP_M].item_data.floatData;
     KI_m=Item[KI_M].item_data.floatData;
     KP_S_E=Item[KP_S].item_data.floatData;
     KD_S_E=Item[KD_S].item_data.floatData;
     LIMIT_SE=Item[LIMIT_S].item_data.floatData;
     servo_mid=Item[MID_SERVO].item_data.floatData;
     collect_max_flag = (uint8)Item[U_1].item_data.intData;
     process_type_ai  = (uint8)Item[U_2].item_data.intData;
}

void Save_data(void)
{
    CAR_BUFFER[1]=float_conversion_uint32(Moto_Speed_Goal_Set);
    CAR_BUFFER[2]=float_conversion_uint32(KP_S_E);
    CAR_BUFFER[3]=float_conversion_uint32(KD_S_E);
    CAR_BUFFER[4]=float_conversion_uint32(LIMIT_SE);
    CAR_BUFFER[5]=float_conversion_uint32(servo_mid);
    CAR_BUFFER[6]=float_conversion_uint32(KP_m);
    CAR_BUFFER[7]=float_conversion_uint32(KI_m);
    CAR_BUFFER[8]=collect_max_flag ;
    CAR_BUFFER[9]=process_type_ai;
    Sector_Erase(0);
    for(uint8 i=0;i<PARA_MAX-1;i++)
    {
        Page_Program(0,i,&CAR_BUFFER[i+1]);
    }

}

void Save_ADMAX(void)
{
    Sector_Erase(1);
    uint32 Save_max[(AD_NUM+AI_NUM)];
    for(uint8 i=0;i<(AD_NUM+AI_NUM);i++)
    {
        Save_max[i]=float_conversion_uint32(Max[i]);
    }
    for(uint8 i=0;i<(AD_NUM+AI_NUM);i++)
    {
        Page_Program(1,i,&Save_max[i]);
    }
}

void Read_AD(void)
{
    for(uint8 i=0;i<(AD_NUM+AI_NUM);i++)
    {
        Max[i]=Page_Read(1,i,float);
    }

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
    if(!GPIO_Read(P21,4)||!GPIO_Read(P20,3)||!GPIO_Read(P21,2)||!GPIO_Read(P21,3)||!GPIO_Read(P21,5))
    {
        Delay_ms(STM0,100);
        if(!GPIO_Read(P21,4))
        {
            key_oper = ok_o;
        }
        else if(!GPIO_Read(P20,3))
        {
            key_oper = up_o;
        }
        else if(!GPIO_Read(P21,2))
        {
            key_oper = down_o;
        }
        else if(!GPIO_Read(P21,3))
        {
            key_oper = left_o;
        }
        else if(!GPIO_Read(P21,5))
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
        case ok_o:
            if(Item[CurItem].item_type == list_item)
            {
                CurMenu = CurItem;
            CurItem = Item[CurItem].child_ID;
            }
            else if(CurItem == HOLD)
            {
                Save_data();
                Save_ADMAX();
//                Sector_Erase(1);
                GPIO_Set(P02,8, 1);
                Delay_ms(STM0, 100);
                GPIO_Set(P02,8, 0);
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

void ModifyintData(void)
{
    int16 multiply=1000;
    int16 now_num=Item[CurItem].item_data.intData;
    int16 max=Item[CurItem].data_max;
    int16 min=Item[CurItem].data_min;
    while(TRUE)                                                    //���������µİ�������
        {
           key g_key=GetKey();
           switch (g_key)
           {
               case up_o:
                   now_num = now_num + multiply;
                   if( now_num>max)
                       now_num=max;
                   break;
               case down_o:
                   now_num = now_num - multiply;
                   if( now_num<min)
                       now_num=min;
                   break;
               case right_o:
                    multiply= multiply/10;
                    if(multiply<1)
                       multiply=1000;
                   break;
               case left_o:
                   Item[CurItem].item_data.intData = now_num;
                   break;
               case undo:
                   break;
               default:
                   break;
           }
          if(g_key!=undo)
          PrintintData(multiply,now_num);
          if (g_key==left_o)
          break;
        }

}

void ModifyfloatData(void)
{
    float multiply=10;
    float now_num=Item[CurItem].item_data.floatData;
    float max=(float)Item[CurItem].data_max;
    float min=(float)Item[CurItem].data_min;
    while(TRUE)                                                    //���������µİ�������
        {
           key g_key=GetKey();
           switch (g_key)
           {
               case up_o:
                   now_num = now_num + multiply;
                   if( now_num>max)
                       now_num=max;
                   break;
               case down_o:
                   now_num = now_num - multiply;
                   if( now_num<min)
                       now_num=min;
                   break;
               case right_o:
                    multiply= multiply/10;
                    if(multiply<0.001)
                       multiply=10;
                   break;
               case left_o:
                   Item[CurItem].item_data.floatData = now_num;
                   break;
               case undo:
                   break;
               default:
                   break;
           }
          if(g_key!=undo)
          PrintfloatData(multiply,now_num);
          if (g_key==left_o)
          break;

        }

}


void PrintintData(int mul,int now_)
{
    SmartCar_OLED_Fill(0);                                            //����
    SmartCar_OLED_P6x8Str(0, 0, Item[CurItem].item_name);
    SmartCar_OLED_P6x8Str(0, 1, "----------------------");
    SmartCar_OLED_Printf6x8(0, 3,"%d",now_);
    SmartCar_OLED_Printf6x8(10, 5,"%d", mul);
}

void PrintfloatData(float mul,float now_)
{
    SmartCar_OLED_Fill(0);                                            //����
    SmartCar_OLED_P6x8Str(0, 0, Item[CurItem].item_name);
    SmartCar_OLED_P6x8Str(0, 1, "----------------------");
    SmartCar_OLED_Printf6x8(0, 3,"%.4f",now_);
    SmartCar_OLED_Printf6x8(10, 5,"%.4f", mul);
}


