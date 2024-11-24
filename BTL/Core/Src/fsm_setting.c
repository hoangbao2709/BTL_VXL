/*
 * fsm_setting.c
 *
 *  Created on: 27 Oct 2024
 *      Author: PC
 */

#include "fsm_setting.h"

int mode = 0, en0 = 1, en1 = 0, en2 = 1, en3 = 0;

void setup_Led7SEG(){
	switch(en0){
	case 0:
		en0 = 1;
		break;
	case 1:
		en0 = 0;
		break;
	}
}


