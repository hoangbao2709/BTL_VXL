/*
 * fsm_manual.h
 *
 *  Created on: 27 Oct 2024
 *      Author: PC
 */

#ifndef INC_FSM_MANUAL_H_
#define INC_FSM_MANUAL_H_

#include "button.h"
#include "software_timer.h"
#include "led_traffic.h"
#include "fsm_automatic.h"
#include "fsm_setting.h"
#include "global.h"
#include "i2c-lcd.h"

void fsm_manual();
int case2(int ,int);
int case3();
int case4();
#endif /* INC_FSM_MANUAL_H_ */
