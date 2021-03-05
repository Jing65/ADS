/*
 * menu.c
 *
 *  Created on: 2021年2月27日
 *      Author: HP
 */
#include "menu_last.h"


int current_line=3;
int Arrow_line=3;
float K_P=0;
float K_D=0;
uint32 buffer[1024]={0};

void Menu_control(void)
{
    if((!GPIO_Read(P22,0))||(!GPIO_Read(P22,1))||(!GPIO_Read(P22,2)))
    {
        Delay_ms(STM0, 80);
        if(!GPIO_Read(P22,0))
        {
            Delay_ms(STM0, 50);
            SmartCar_OLED_P6x8Str(0,Arrow_line,"  ");
            Arrow_line--;
            current_line--;
            if(Arrow_line<3){SmartCar_OLED_Fill(0);Arrow_line=7;}
            if(current_line<3){current_line=12;}
        }
        else if(!GPIO_Read(P22,1))
        {
            Delay_ms(STM0, 50);
            SmartCar_OLED_P6x8Str(0,Arrow_line,"  ");
            Arrow_line++;
            current_line++;
            if(Arrow_line>7)
            {
                SmartCar_OLED_Fill(0);
                Arrow_line=3;
            }
            if(current_line>12){current_line=3;}
        }
        else if(!GPIO_Read(P22,2))
        {
            Delay_ms(STM0, 50);
            if(current_line==3)
            {
                SmartCar_OLED_Fill(0);
                MenuA_1();//二级菜单函数
                SmartCar_OLED_Fill(0);
            }
            if(current_line==4)
            {
                SmartCar_OLED_Fill(0);
                MenuB_1();//二级菜单函数
                SmartCar_OLED_Fill(0);
            }
        }

    }
    if(current_line>=3&&current_line<=7)
    {OSD_Main_1 ();}

    else if(current_line>7&&current_line<13)
    {OSD_Main_2 ();}
}
void OSD_Main_1 (void)
{

    SmartCar_OLED_P6x8Str(40,0,"Main Menu 1");
    SmartCar_OLED_P6x8Str(16,2,"------------");
    SmartCar_OLED_P6x8Str(16,3,"pid");
    SmartCar_OLED_P6x8Str(16,4,"read_only");
    SmartCar_OLED_P6x8Str(16,5,"steer");
    SmartCar_OLED_P6x8Str(16,6,"motor");
    SmartCar_OLED_P6x8Str(16,7,"AD");
    SmartCar_OLED_P6x8Str(0,Arrow_line,"->");
}

void OSD_Main_2 (void)
{
    SmartCar_OLED_P6x8Str(40,0,"Main Menu 2");
    SmartCar_OLED_P6x8Str(16,2,"------------");
    SmartCar_OLED_P6x8Str(16,3,"pid");
    SmartCar_OLED_P6x8Str(16,4,"read_only");
    SmartCar_OLED_P6x8Str(16,5,"steer");
    SmartCar_OLED_P6x8Str(16,6,"motor");
    SmartCar_OLED_P6x8Str(16,7,"AD");
    SmartCar_OLED_P6x8Str(0,Arrow_line,"->");
}
void MenuA_1(void)
{
    int current_1_line=3;
    int Arrow_1_line=3;//用来标记行数
    while(1)
    {
               if((!GPIO_Read(P22,0))||(!GPIO_Read(P22,1))||(!GPIO_Read(P22,2))||(!GPIO_Read(P22,3)))
                    {
                        Delay_ms(STM0, 50);
                        if(!GPIO_Read(P22,0))
                        {
                           Delay_ms(STM0, 50);
                           SmartCar_OLED_P8x16Str(0,Arrow_1_line,"  ");
                           Arrow_1_line--;
                           current_1_line--;
                           if(Arrow_1_line<3){SmartCar_OLED_Fill(0);Arrow_1_line=7;}
                           if(current_1_line<3){current_1_line=12;}

                        }else if (!GPIO_Read(P22,1))
                        {
                             Delay_ms(STM0, 50);
                             SmartCar_OLED_P8x16Str(0,Arrow_1_line,"  ");
                             Arrow_1_line++;
                             current_1_line++;
                             if(Arrow_1_line>7){SmartCar_OLED_Fill(0);Arrow_1_line=3;}
                              if(current_1_line>12){current_1_line=3;}
                        }else if(!GPIO_Read(P22,3))
                        {

                            if(current_1_line==3)
                           {
                               SmartCar_OLED_Fill(0);
                               MenuA_1_1();//进入调参函数界面
                               SmartCar_OLED_Fill(0);

                           }

                        }else if(!GPIO_Read(P22,2))
                        {
                           Delay_ms(STM0, 100);
                           break;
                        }
                    };
                    if(current_1_line>=3&&current_1_line<=7)
                    {
                               SmartCar_OLED_P8x16Str(40,0,"pid");
                               SmartCar_OLED_P8x16Str(16,3,"K_P");
                               SmartCar_OLED_P8x16Str(16,5,"K_I");
                               SmartCar_OLED_P8x16Str(0,Arrow_1_line,"->");
                    }
                    else if(current_1_line>7&&current_1_line<13)
                            {

                               SmartCar_OLED_P8x16Str(40,0,"pid");
                               SmartCar_OLED_P8x16Str(16,3,"K_P");
                               SmartCar_OLED_P8x16Str(16,5,"K_I");
                               SmartCar_OLED_P8x16Str(0,Arrow_1_line,"->");

                            }

      };
}
void Read_flash(void)
{
    K_P=Page_Read(0,0,float);
    K_D=Page_Read(0,1,float);

}
void MenuB_1(void)
{

}
void MenuA_1_1(void)
{
    float w=10;
    float a= K_P ;
    float b= K_P ;//用于最后判断是否一致决定是否存储
    SmartCar_OLED_P6x8Str(40,0,"K_P");
    SmartCar_OLED_Printf6x8(20,2,"%.2f",a);
    SmartCar_OLED_Printf6x8(20,1,"%.2e",a);
   while(1)
   {
       if((!GPIO_Read(P22,0))||(!GPIO_Read(P22,1))||(!GPIO_Read(P22,2))||(!GPIO_Read(P22,3)))
       {

                                 Delay_ms(STM0, 50);
                                 if(!GPIO_Read(P22,0))
                                  {
                                     Delay_ms(STM0, 100);
                                     if (a<90) {a=a+w;}
                                     SmartCar_OLED_P6x8Str(20,4,"     ");
                                  }else if (!GPIO_Read(P22,1))
                                  {
                                      Delay_ms(STM0, 100);
                                      if (a>-90) {a=a-w;}
                                      SmartCar_OLED_P6x8Str(20,4,"      ");
                                  }else if(!GPIO_Read(P22,2))
                                  {
                                      Delay_ms(STM0, 100);
                                      K_P= a;
                                      break;
                                  }else if(!GPIO_Read(P22,3))
                                  {
                                      Delay_ms(STM0, 100);
                                      SmartCar_OLED_P6x8Str(20,6,"    ");
                                      if(w>0.01){w=w/10;}
                                      else{w=10;}
                                  };

                                 SmartCar_OLED_Printf6x8(20,5,"%.2f",a);
                                 SmartCar_OLED_Printf6x8(20,4,"%.2e",a);
                                 SmartCar_OLED_Printf6x8(20,6,"%.2f",w);

       };
   };
   if(b==a){return;}
   else
   {
       K_P=a;
       Sector_Erase(0);
       buffer[0]=float_conversion_uint32(a);
       buffer[1]=float_conversion_uint32(K_D);
       Sector_Program(0,buffer);
       Delay_ms(STM0, 80);
   }

}
