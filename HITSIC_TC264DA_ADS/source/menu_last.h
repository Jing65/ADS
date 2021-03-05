/*
 * menu.h
 *
 *  Created on: 2021Äê2ÔÂ27ÈÕ
 *      Author: HP
 */

#ifndef SOURCE_MENU_LAST_H_
#define SOURCE_MENU_LAST_H_
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include <ifxCpu_Irq.h>
#include "IfxScuWdt.h"
#include "common.h"
#include "SmartCar_Oled.h"
#include "SmartCar_GPIO.h"
#include "SmartCar_Flash.h"
#include "SmartCar_Systick.h"





void Read_flash(void);
void Menu_control(void);
void OSD_Main_1 (void);
void OSD_Main_2 (void);
void MenuA_1(void);
void MenuB_1(void);
void MenuA_1_1(void);


#endif /* SOURCE_MENU_LAST_H_ */
