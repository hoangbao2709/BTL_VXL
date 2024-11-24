/*
 * trafic_1.c
 *
 *  Created on: Sep 17, 2024
 *      Author: PC
 */

#include "led_traffic.h"


void LED_RED_1(void) {
    HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_SET);
}

void LED_GREEN_1(void) {
    HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
}

void LED_YELLOW_1(void){
    HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_SET);
}

void LED_RED_2(void) {
    HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_SET);
}

void LED_GREEN_2(void) {
    HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
}

void LED_YELLOW_2(void){
    HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_SET);
}

void LED_RED_3(void) {
    HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_SET);
}

void LED_GREEN_3(void) {
    HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
}

void LED_YELLOW_3(void){
    HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_SET);
}

void LED_RED_4(void) {
    HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(h_GPIO_Port, h_Pin, GPIO_PIN_SET);
}

void LED_GREEN_4(void) {
    HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(h_GPIO_Port, h_Pin, GPIO_PIN_RESET);
}

void LED_YELLOW_4(void){
    HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(h_GPIO_Port, h_Pin, GPIO_PIN_SET);
}


void off_LED(void){
	HAL_GPIO_WritePin(a_GPIO_Port, a_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(b_GPIO_Port, b_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(c_GPIO_Port, c_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(d_GPIO_Port, d_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(e_GPIO_Port, e_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(f_GPIO_Port, f_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(g_GPIO_Port, g_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(h_GPIO_Port, h_Pin, GPIO_PIN_RESET);
}
