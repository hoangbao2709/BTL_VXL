/*
 * button.c
 *
 *  Created on: Oct 21, 2024
 *      Author: PC
 */
#include "button.h"


#define ARRAY_SIZE 4

int button_flag[ARRAY_SIZE] = {0 ,0 ,0, 0};
int PRESSED_STATE_0 = 0;

int KeyReg0[ARRAY_SIZE] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
int KeyReg1[ARRAY_SIZE] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
int KeyReg2[ARRAY_SIZE] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};

int KeyReg3[ARRAY_SIZE] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
int TimerForKeyPress[ARRAY_SIZE] = {2000, 2000, 2000, 2000};

void subKeyProcess(int i){
	for(int j = 0; j < ARRAY_SIZE; j++){
		button_flag[j] = 0;
	}
	char buffer[20];
	sprintf(buffer, "%d", i + 1);
	HAL_UART_Transmit(&huart2, buffer, sizeof(buffer), 1000);
	button_flag[i] = 1;
}

void subKeyProcess0(){
	PRESSED_STATE_0 = 1;
}

void getKeyInput(){
	KeyReg0[0] = KeyReg1[0];
	KeyReg1[0] = KeyReg2[0];
	KeyReg2[0] = HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin);

	KeyReg0[1] = KeyReg1[1];
	KeyReg1[1] = KeyReg2[1];
	KeyReg2[1] = HAL_GPIO_ReadPin(Button2_GPIO_Port, Button2_Pin);

	KeyReg0[2] = KeyReg1[2];
	KeyReg1[2] = KeyReg2[2];
	KeyReg2[2] = HAL_GPIO_ReadPin(Button3_GPIO_Port, Button3_Pin);

	KeyReg0[3] = KeyReg1[3];
	KeyReg1[3] = KeyReg2[3];
	KeyReg2[3] = HAL_GPIO_ReadPin(Button4_GPIO_Port, Button4_Pin);

	for(int i = 0; i < ARRAY_SIZE; i++){
		if((KeyReg0[i] == KeyReg1[i]) && (KeyReg1[i] == KeyReg2[i])){
			if(KeyReg3[i] != KeyReg2[i]){
				KeyReg3[i] = KeyReg2[i];
				if(KeyReg2[i] == PRESSED_STATE){
					subKeyProcess(i);
					TimerForKeyPress[i] = 2000;
				}
			}else{
				TimerForKeyPress[i]--;
				if(TimerForKeyPress[i] == 0){
					if(i == 0 && KeyReg2[0] == PRESSED_STATE){
						subKeyProcess0();
					}
					else if(i != 0 && KeyReg2[i] == PRESSED_STATE){
						subKeyProcess(i);
					}
					TimerForKeyPress[i] = 2000;
				}
			}
		}
	}
}
