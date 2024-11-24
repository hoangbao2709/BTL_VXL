/*
 * fsm_manual.c
 *
 *  Created on: 27 Oct 2024
 *      Author: PC
 */

#include "fsm_manual.h"
#include <stdio.h>
#include <stdlib.h>

int temp, test_1 = 0;

int case2(int mode,int a){
	if(en0 == 1)
		switch(mode){
		case 1:
			LED_RED_1();
			LED_RED_2();
			LED_RED_3();
			LED_RED_4();
			break;
		case 2:
			LED_YELLOW_1();
			LED_YELLOW_2();
			LED_YELLOW_3();
			LED_YELLOW_4();
			break;
		case 3:
			LED_GREEN_1();
			LED_GREEN_2();
			LED_GREEN_3();
			LED_GREEN_4();
			break;
		}
	else{
		off_LED();
	}

	if(button_flag[1] == 1){
	  button_flag[1] = 0;
	  if(a == 99){
		  a = 1;
	  }else
		  a++;
	}
	if(button_flag[3] == 1){
	  button_flag[3] = 0;
	  a--;
	}
	if(a == 0){
	  a = 99;
	}
	if(button_flag[2] == 1){
	  button_flag[2] = 0;
	  switch(mode){
	  case 1:
		  prev_max_Red = a;
		  break;
	  case 2:
		  prev_max_Yellow = a;
		  break;
	  case 3:
		  prev_max_Green = a;
		  break;
	  }
	}
	lcd_goto_XY(0, 0);
	char buffer[20];
	if(mode == 1){
		if(a <= 9){
			sprintf(buffer, "Time Red: %d ", a);
		}else{
			sprintf(buffer, "Time Red: %d", a);
		}
	}

	if(mode == 2){
		if(a <= 9){
			sprintf(buffer, "Time Yellow: %d ", a);
		}else{
			sprintf(buffer, "Time Yellow: %d", a);
		}
	}

	if(mode == 3){
		if(a <= 9){
			sprintf(buffer, "Time Green: %d ", a);
		}else{
			sprintf(buffer, "Time Green: %d", a);
		}
	}
	lcd_send_string(buffer);
	return a;
}


void fsm_manual(){
	if(button_flag[0] == 1){
		lcd_clear_display();
		button_flag[0] = 0;
		mode++;
		test_1 = 1;
		if(mode == 4 || mode == 6) {
			SCH_Delete_Task(2);
			lcd_clear_display();
			if(prev_max_Red == prev_max_Green + prev_max_Yellow){
				max_Red = prev_max_Red;
				max_Green = prev_max_Green;
				max_Yellow = prev_max_Yellow;
			}else if(prev_max_Red > prev_max_Yellow){
				prev_max_Green = prev_max_Red - prev_max_Yellow;
				max_Red = prev_max_Red;
				max_Green = prev_max_Green;
				max_Yellow = prev_max_Yellow;
			}
			Reset();
			mode = 0;
		}
		else if(mode == 1){
			SCH_Add_Task(setup_Led7SEG, 0, 50);
		}
	}
	if(mode != 0 && mode != 6){
		lcd_goto_XY(1, 0);
		char buffer[20];
		sprintf(buffer, "Mode: %d", mode + 1);
		lcd_send_string(buffer);
	}else if(mode == 6){
		lcd_goto_XY(1, 0);
		char buffer[20];
		sprintf(buffer, "Mode manual");
		lcd_send_string(buffer);
	}
	switch(mode){
	case 0:
	  if(timer_flag[0] == 1){
		  case1();
		  setTimer(0, 1000);
	  }
	  break;
	case 1:
		if(test_1 == 1){
			temp = max_Red;
			test_1 = 0;
		}
		temp = case2(mode, temp);
		break;
	case 2:
		if(test_1 == 1){
			temp = max_Yellow;
			test_1 = 0;
		}
		temp = case2(mode, temp);
		break;
	case 3:
		if(test_1 == 1){
			temp = max_Green;
			test_1 = 0;
		}
		temp = case2(mode, temp);
		break;
	}
	if(PRESSED_STATE_0 == 1){
		lcd_clear_display();
		case3();
		mode = 5;
		PRESSED_STATE_0 = 0;
	}
	if(mode == 5){
		if(button_flag[1] == 1){
			lcd_clear_display();
			case3();
			button_flag[1] = 0;
		}
	}
}
