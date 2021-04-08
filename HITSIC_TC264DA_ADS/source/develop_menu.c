/*
 * develop_menu.c
 *
 *  Created on: 2021年3月7日
 *      Author: HP
 */
#include "develop_menu.h"

uint16 ItemNum = 0;   //菜单项的数目
uint16 ParaNum = 0;   //参量型菜单项的数目
uint16 OrderNum = 0;  //指令型菜单项的数目
uint16 CurItem = 1;   //当前菜单项的ID
uint16 CurMenu = MENU;//当前菜单的ID，初始化为1
static item Item[ITEM_MAX];  //菜单项结构体数组
static uint32 CAR_BUFFER[PARA_MAX];//flash中用来存数据的数组

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
//    if(If_Start == 1)
//    {
//           if(!GPIO_Read(P20,8))
//          {
//             Delay_ms(STM1,200);
//             If_Start = 0;
//          }
//    }
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
    Item.ID = (ItemNum++);                          //MENU项的ID是0
    if(type == order_item)                        //指令型菜单项
    {
        Item.order_ID = (++OrderNum);                 // 指令菜单项数加一
    }
    else
    {
      if(type == paraI_item || type == paraF_item)//参量型菜单项
      {
          cardata car_init;//指令菜单项数加一
          Item.para_ID = (ParaNum++);                               //将小车的数据给参量型菜单项（对参量型菜单项数据进行初始化）
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
//  @brief       打印菜单
//  @param
//  @attention   依照当前菜单ID(CurMenu)和当前菜单项ID(CurItem)打印菜单
//  @return      void
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void PrintMenu(void)
{
    uint8 x, y;
    uint16 start_site;
    uint16 print_ID=0;
    uint16 end_ID;
    start_site = (Item[CurItem].list_ID <= 6) ? 1 : Item[CurItem].list_ID - 5;                                  //菜单中打印开始项的位置
    print_ID = Item[CurMenu].child_ID + (start_site - 1);                                                                           //打印开始项的位置的ID
    end_ID = Item[CurMenu].child_ID + (Item[CurMenu].list_num - 1);                                                         //该菜单最后一项的ID
    DataUpdate();                                                                                                                                                               //首先更新一下数据
    SmartCar_OLED_Fill(0);                                                                                                                                                       //清屏
    SmartCar_OLED_P6x8Str(0, 0, Item[CurMenu].item_name);
    SmartCar_OLED_P6x8Str(0, 1, "----------------------");
    x = 6;
    y = 2;                                                                                                                                                                          //菜单项的开始显示位置
    while(y <= 7 && print_ID <= end_ID)
 {
     SmartCar_OLED_P6x8Str(x, y, Item[print_ID].item_name);                                                                                  //打印菜单项名字
     if(Item[print_ID].item_type != list_item && Item[print_ID].item_type != order_item)                //打印数据菜单项数据
     {
         if(Item[print_ID].item_type == stateI_item || Item[print_ID].item_type == paraI_item)    //打印整型数据
             SmartCar_OLED_Printf6x8(66, y,"%d",Item[print_ID].item_data.intData);
         else
             SmartCar_OLED_Printf6x8(66, y,"%.4f",Item[print_ID].item_data.floatData);
     }
     print_ID++;
     y++;
 }
    SmartCar_OLED_P6x8Str(0, (Item[CurItem].list_ID + 1 <= 7) ? Item[CurItem].list_ID + 1 : 7, ">");  //打印位置指示符
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief         重新打印菜单时对数据进行更新
//  @param
//  @attention     此函数放在打印菜单PrintMenu()函数里，在打印菜单前更新一下菜单项数据以及把修改的参量型菜单项数据传给小车数据和实际参量数据
//  @return
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void CreatMenu(void)//**创建并插入一个新项需要增加一个新ITEMID枚举项***
{
    Item[MENU] = CreatItem(list_item, "MENU", 0, 1);
    Item[MODE] = CreatItem(paraI_item, "EXAMPLE", 0, 1);
    Item[AD_] = CreatItem(list_item, "AD_read", 0, 1);
    Item[STEER] = CreatItem(list_item, "steer&speed", 0, 1);
    Item[MOTOR] = CreatItem(list_item, "MOTOR", 0, 1);
    Item[UNUSE] = CreatItem(list_item, "flag", 0, 1);
    Item[HOLD] = CreatItem(order_item, "Hold", 0, 1);//保存菜单项
    Item[THRESHOLD] = CreatItem(list_item, "threshold", 0, 1);
    InsertItem(Item, MENU, MODE);
    InsertItem(Item, MENU, AD_);
    InsertItem(Item, MENU, STEER);
    InsertItem(Item, MENU, MOTOR);
    InsertItem(Item, MENU, UNUSE);
    InsertItem(Item, MENU, HOLD);
    InsertItem(Item, MENU, THRESHOLD);
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
    Item[SERVO_1] = CreatItem(stateI_item, "acc_en", -32700, 32700);
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
    Item[MOTOR_1] = CreatItem(paraF_item, "low_speed", 0,3);
    Item[MOTOR_2] = CreatItem(paraF_item, "XXXX", 0, 50);
    Item[MOTOR_3] = CreatItem(stateI_item, "RXXX", -30000, 30000);
    Item[MOTOR_4] = CreatItem(stateF_item, "RXXX", -500, 500);
    InsertItem(Item, MOTOR, KP_M);
    InsertItem(Item, MOTOR, KI_M);
    InsertItem(Item, MOTOR, MOTOR_1);
    InsertItem(Item, MOTOR, MOTOR_2);
    InsertItem(Item, MOTOR, MOTOR_3);
    InsertItem(Item, MOTOR, MOTOR_4);
    Item[U_1] = CreatItem(paraI_item, "collect_m", 0, 1);
    Item[U_2] = CreatItem(paraI_item, "process_ai", 0, 1);
    Item[U_3] = CreatItem(paraI_item, "short_con", 0, 1);
    Item[U_4] = CreatItem(paraI_item, "nnom_pro", 0, 1);
    Item[U_5] = CreatItem(stateF_item, "RXXX", -500, 500);
    InsertItem(Item, UNUSE, U_1);
    InsertItem(Item, UNUSE, U_2);
    InsertItem(Item, UNUSE, U_3);
    InsertItem(Item, UNUSE, U_4);
    InsertItem(Item, UNUSE, U_5);
    Item[thre_1] = CreatItem(paraI_item, "right_th", 0, 2000);
    Item[thre_2] = CreatItem(paraI_item, "cancel_r", 0, 2000);
    Item[thre_3] = CreatItem(paraI_item, "R_ecoder", 0, 30000);
    Item[thre_4] = CreatItem(paraI_item, "cr_mis", 0, 300);
    Item[thre_5] = CreatItem(paraI_item, "round_m", 0, 300);
    Item[thre_6] = CreatItem(paraI_item, "S_right", 0, 300);
    Item[thre_7] = CreatItem(paraI_item, "S_cross", 0, 300);
    Item[thre_8] = CreatItem(paraI_item, "S_Ronum", 0, 300);
    Item[thre_9] = CreatItem(paraI_item, "S_Rsigle", 0, 300);
    Item[thre_10] = CreatItem(paraI_item, "SRcancel", 0, 300);
    Item[thre_11] = CreatItem(paraI_item, "S_ecoder", 0, 30000);
    Item[thre_12] = CreatItem(paraI_item, "S_midr", 0, 300);
    Item[thre_13] = CreatItem(paraI_item, "S_intor", 0, 300);
    Item[thre_14] = CreatItem(paraI_item, "S_Reco", 0, 31000);
    Item[thre_15] = CreatItem(paraI_item, "cancel_i", 0, 300);
    Item[thre_16] = CreatItem(paraF_item, "servo_r", 0, 1.8);
    Item[thre_17] = CreatItem(paraF_item, "KP_S_S", 0, 100);
    Item[thre_18] = CreatItem(paraF_item, "KD_S_S", 0, 100);
    Item[thre_19] = CreatItem(paraI_item, "out_roun", 0, 100);
    InsertItem(Item,THRESHOLD,thre_1);
    InsertItem(Item,THRESHOLD,thre_2);
    InsertItem(Item,THRESHOLD,thre_3);
    InsertItem(Item,THRESHOLD,thre_4);
    InsertItem(Item,THRESHOLD,thre_5);
    InsertItem(Item,THRESHOLD,thre_6);
    InsertItem(Item,THRESHOLD,thre_7);
    InsertItem(Item,THRESHOLD,thre_8);
    InsertItem(Item,THRESHOLD,thre_9);
    InsertItem(Item,THRESHOLD,thre_10);
    InsertItem(Item,THRESHOLD,thre_11);
    InsertItem(Item,THRESHOLD,thre_12);
    InsertItem(Item,THRESHOLD,thre_13);
    InsertItem(Item,THRESHOLD,thre_14);
    InsertItem(Item,THRESHOLD,thre_15);
    InsertItem(Item,THRESHOLD,thre_16);
    InsertItem(Item,THRESHOLD,thre_17);
    InsertItem(Item,THRESHOLD,thre_18);
    InsertItem(Item,THRESHOLD,thre_19);
}

void MenuInit(void)
{
    Item[Moto_Goal].item_data.floatData=Moto_Speed_Goal_Set;
    Item[MID_SERVO].item_data.floatData=servo_mid;
    Item[KP_S].item_data.floatData=KP_S_E;
    Item[KD_S].item_data.floatData=KD_S_E;
    Item[KP_M].item_data.floatData=KP_m;
    Item[KI_M].item_data.floatData=KI_m;
    Item[MOTOR_1].item_data.floatData=lower_speed;
    Item[LIMIT_S].item_data.floatData=LIMIT_SE;
    Item[SERVO_1].item_data.intData=acc_encoder;
    Item[U_1].item_data.intData=collect_max_flag;
    Item[U_2].item_data.intData=process_type_ai;
    Item[U_3].item_data.intData=short_control;
    Item[U_4].item_data.intData=nnom_pro;
    Item[thre_1].item_data.intData=long_theshold.right_threshould;
    Item[thre_2].item_data.intData=long_theshold.cancel_right_ad;
    Item[thre_3].item_data.intData=long_theshold._SCFTM;
    Item[thre_4].item_data.intData=long_theshold.cross_misjudge;
    Item[thre_5].item_data.intData=long_theshold.round_midjudge;
    Item[thre_6].item_data.intData=short_theshold.right_threshould;
    Item[thre_7].item_data.intData=short_theshold.cross_misjudge;
    Item[thre_8].item_data.intData=short_theshold.round_misjudge_num;
    Item[thre_9].item_data.intData=short_theshold.round_misjudge_single;
    Item[thre_10].item_data.intData=short_theshold.cancel_right_ad;
    Item[thre_11].item_data.intData=short_theshold._SCFTM;
    Item[thre_12].item_data.intData=short_theshold.round_midjudge;
    Item[thre_13].item_data.intData=short_theshold.into_round;
    Item[thre_14].item_data.intData=short_theshold.round_SCFTM;
    Item[thre_15].item_data.intData=short_theshold.cancel_ir;
    Item[thre_16].item_data.floatData =round_servo;
    Item[thre_17].item_data.floatData =KP_S_S;
    Item[thre_18].item_data.floatData =KD_S_S;
    Item[thre_19].item_data.intData =short_theshold.out_flagjudge;
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
    short_control=Page_Read(0,9,uint8);
    long_theshold.right_threshould=Page_Read(0,10,int16);
    long_theshold.cancel_right_ad=Page_Read(0,11,int16);
    long_theshold._SCFTM=Page_Read(0,12,int16);
    long_theshold.cross_misjudge=Page_Read(0,13,int16);
    long_theshold.round_midjudge=Page_Read(0,14,int16);
    nnom_pro=Page_Read(0,15,uint8);
    short_theshold.right_threshould=Page_Read(0,16,int16);
    short_theshold.cross_misjudge=Page_Read(0,17,int16);
    short_theshold.round_misjudge_num=Page_Read(0,18,int16);
    short_theshold.round_misjudge_single=Page_Read(0,19,int16);
    short_theshold.cancel_right_ad=Page_Read(0,20,int16);
    short_theshold._SCFTM=Page_Read(0,21,int16);
    short_theshold.round_midjudge=Page_Read(0,23,int16);
    short_theshold.into_round=Page_Read(0,24,int16);
    short_theshold.round_SCFTM=Page_Read(0,25,int16);
    short_theshold.cancel_ir=Page_Read(0,26,int16);
    round_servo=Page_Read(0,27,float);
    KP_S_S=Page_Read(0,28,float);
    KD_S_S=Page_Read(0,29,float);
    short_theshold.out_flagjudge=Page_Read(0,30,int16);
//暂时去掉    lower_speed=Page_Read(0,22,float);
}

void DataUpdate(void)
{
    for(uint8 i=0;i<16;i++)
    {
      Item[(i+AD_1)].item_data.floatData =AD[i];
    }
     Item[SERVO_1].item_data.intData=acc_encoder;
     Moto_Speed_Goal_Set=Item[Moto_Goal].item_data.floatData;
     KP_m=Item[KP_M].item_data.floatData;
     KI_m=Item[KI_M].item_data.floatData;
     KP_S_E=Item[KP_S].item_data.floatData;
     KD_S_E=Item[KD_S].item_data.floatData;
     LIMIT_SE=Item[LIMIT_S].item_data.floatData;
     servo_mid=Item[MID_SERVO].item_data.floatData;
     collect_max_flag = (uint8)Item[U_1].item_data.intData;
     process_type_ai  = (uint8)Item[U_2].item_data.intData;
     short_control  = (uint8)Item[U_3].item_data.intData;
     nnom_pro = (uint8)Item[U_4].item_data.intData;
     long_theshold.right_threshould=Item[thre_1].item_data.intData;
     long_theshold.cancel_right_ad=Item[thre_2].item_data.intData;
     long_theshold._SCFTM=Item[thre_3].item_data.intData;
     long_theshold.cross_misjudge=Item[thre_4].item_data.intData;
     long_theshold.round_midjudge=Item[thre_5].item_data.intData;
     short_theshold.right_threshould=Item[thre_6].item_data.intData;
     short_theshold.cross_misjudge=Item[thre_7].item_data.intData;
     short_theshold.round_misjudge_num=Item[thre_8].item_data.intData;
     short_theshold.round_misjudge_single=Item[thre_9].item_data.intData;
     short_theshold.cancel_right_ad=Item[thre_10].item_data.intData;
     short_theshold._SCFTM=Item[thre_11].item_data.intData;
     lower_speed=Item[MOTOR_1].item_data.floatData;
     short_theshold.round_midjudge=Item[thre_12].item_data.intData;
     short_theshold.into_round=Item[thre_13].item_data.intData;
     short_theshold.round_SCFTM=Item[thre_14].item_data.intData;
     short_theshold.cancel_ir=Item[thre_15].item_data.intData;
     round_servo=Item[thre_16].item_data.floatData;
     KP_S_S=Item[thre_17].item_data.floatData;
     KD_S_S=Item[thre_18].item_data.floatData;
     short_theshold.out_flagjudge=Item[thre_19].item_data.intData;
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
    CAR_BUFFER[10]=short_control;
    CAR_BUFFER[11]=long_theshold.right_threshould;
    CAR_BUFFER[12]=long_theshold.cancel_right_ad;
    CAR_BUFFER[13]=long_theshold._SCFTM;
    CAR_BUFFER[14]=long_theshold.cross_misjudge;
    CAR_BUFFER[15]=long_theshold.round_midjudge;
    CAR_BUFFER[16]=nnom_pro;
    CAR_BUFFER[17]=short_theshold.right_threshould;
    CAR_BUFFER[18]=short_theshold.cross_misjudge;
    CAR_BUFFER[19]=short_theshold.round_misjudge_num;
    CAR_BUFFER[20]=short_theshold.round_misjudge_single;
    CAR_BUFFER[21]=short_theshold.cancel_right_ad;
    CAR_BUFFER[22]=short_theshold._SCFTM;
    CAR_BUFFER[23]=float_conversion_uint32(lower_speed);
    CAR_BUFFER[24]=short_theshold.round_midjudge;
    CAR_BUFFER[25]=short_theshold.into_round;
    CAR_BUFFER[26]=short_theshold.round_SCFTM;
    CAR_BUFFER[27]=short_theshold.cancel_ir;
    CAR_BUFFER[28]=float_conversion_uint32(round_servo);
    CAR_BUFFER[29]=float_conversion_uint32(KP_S_S);
    CAR_BUFFER[30]=float_conversion_uint32(KD_S_S);
    CAR_BUFFER[31]=short_theshold.out_flagjudge;
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
//  @brief        检测按键
//  @param
//  @attention    同时按下左右键作ok使用，目前ok仅用于保存数据，每隔150ms检测一次按键
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
//  @brief        菜单界面的按键操作
//  @param
//  @attention    上下键进行上下移动光标并修改当前菜单项ID （CurItem），左键返回上一级菜单、右键进入子菜单并修改当前菜单ID（CurMenu)和当前菜单项ID（CurItem）
//                ****在当前菜单项（CurItem）是参量类型菜单项时，右键进入参量项数据修改界面****
//                ****在当前菜单项（CurItem）是Hold菜单项时，右键保存数据****
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
//                iap_write_bytes(0x01, &CAR[0], PARA_MAX * sizeof(cardata));                            //保存修改的参量
            }
            else if(Item[CurItem].item_type == paraI_item || Item[CurItem].item_type == paraF_item)//进入修改参量界面
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
    while(TRUE)                                                    //参量界面下的按键操作
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
    while(TRUE)                                                    //参量界面下的按键操作
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
    SmartCar_OLED_Fill(0);                                            //清屏
    SmartCar_OLED_P6x8Str(0, 0, Item[CurItem].item_name);
    SmartCar_OLED_P6x8Str(0, 1, "----------------------");
    SmartCar_OLED_Printf6x8(0, 3,"%d",now_);
    SmartCar_OLED_Printf6x8(10, 5,"%d", mul);
}

void PrintfloatData(float mul,float now_)
{
    SmartCar_OLED_Fill(0);                                            //清屏
    SmartCar_OLED_P6x8Str(0, 0, Item[CurItem].item_name);
    SmartCar_OLED_P6x8Str(0, 1, "----------------------");
    SmartCar_OLED_Printf6x8(0, 3,"%.4f",now_);
    SmartCar_OLED_Printf6x8(10, 5,"%.4f", mul);
}


