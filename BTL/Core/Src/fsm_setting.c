/*
 * fsm_setting.c
 *
 *  Created on: 27 Oct 2024
 *      Author: PC
 */

#include "fsm_setting.h"

int mode = 0, en0 = 1, en1 = 0, en2 = 1, en3 = 0;

void setup_Led7SEG(){
	if(timer_flag[2] == 1){
		switch(en0){
		case 0:
			en0 = 1;
			break;
		case 1:
			en0 = 0;
			break;
		}
		setTimer(2, 500);
	}
}

void fsm_setting(){
	turn_Led7SEG();
}

