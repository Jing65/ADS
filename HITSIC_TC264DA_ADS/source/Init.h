/*
 * Init.h
 *
 *  Created on: 2021年3月7日
 *      Author: HP
 */

#ifndef SOURCE_INIT_H_
#define SOURCE_INIT_H_
#include"port_def.h"
#include "SmartCar_Pwm.h"
#include "SmartCar_PIT.h"
#include "SmartCar_ADC.h"
#include"control.h"
#include "SmartCar_PIT.h"
#include "SmartCar_Encoder.h"
#include "SmartCar_Uart.h"
#include "SmartCar_Eru.h"

void elec_init(void);

void PWM_init(void);

void PIT_init(void);

void Encoder_init(void);

void Eru_init(void);//外部中断初始化

void Uart_init(void);

#endif /* SOURCE_INIT_H_ */
