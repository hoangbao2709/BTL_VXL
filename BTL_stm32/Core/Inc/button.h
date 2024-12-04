/*
 * button.h
 *
 *  Created on: Oct 21, 2024
 *      Author: PC
 */
#include "main.h"
#include "global.h"

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#define NORMAL_STATE GPIO_PIN_SET
#define PRESSED_STATE GPIO_PIN_RESET

#define ARRAY_SIZE 4

extern int button_flag[ARRAY_SIZE];
extern int PRESSED_STATE_0;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

void subKeyProcess();
void getKeyInput();

#endif /* INC_BUTTON_H_ */
