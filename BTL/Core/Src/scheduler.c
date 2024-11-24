/*
 * scheduler.c
 *
 *  Created on: Nov 13, 2024
 *      Author: hoang
 */


#include "scheduler.h"

sTasks SCH_Tasks_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;
unsigned char Error_code_G = 0;

void SCH_Init(void){
	current_index_task = 0;
}

void SCH_Add_Task(void(*pFuntion)(), uint32_t DELAY, uint32_t PERIOD){
	if(current_index_task < SCH_MAX_TASKS){
		SCH_Tasks_G[current_index_task].pTask = pFuntion;
		SCH_Tasks_G[current_index_task].Delay = DELAY;
		SCH_Tasks_G[current_index_task].Period = PERIOD;
		SCH_Tasks_G[current_index_task].RunMe = 0;

		SCH_Tasks_G[current_index_task].TaskID = current_index_task;
		current_index_task++;
	}
}

void SCH_Update(void){
	for(int i = 0; i < current_index_task;i++){
		if(SCH_Tasks_G[i].Delay > 0){
			SCH_Tasks_G[i].Delay--;
		}else{
			SCH_Tasks_G[i].Delay = SCH_Tasks_G[i].Period;
			SCH_Tasks_G[i].RunMe += 1;
		}
	}
}

void SCH_Dispath_Tasks(void){
	for(int i = 0; i < current_index_task; i++){
		if(SCH_Tasks_G[i].RunMe > 0){
			SCH_Tasks_G[i].RunMe--;
			(*SCH_Tasks_G[i].pTask)();
			if( SCH_Tasks_G[i].Period == 0){
				SCH_Delete_Task(i) ;
			}
		}
	}

	SCH_Report_Status();
}

uint8_t SCH_Delete_Task(uint32_t Task_ID){
	unsigned char Return_code;
	if( SCH_Tasks_G[Task_ID].pTask == 0) {
		Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
		Return_code = RETURN_ERROR;
	} else {
		Return_code = RETURN_NORMAL;
	}
	SCH_Tasks_G[Task_ID].pTask = 0x0000 ;
	SCH_Tasks_G[Task_ID].Delay = 0;
	SCH_Tasks_G[Task_ID].Period = 0;
	SCH_Tasks_G[Task_ID].RunMe = 0;
	current_index_task--;
	return Return_code ; // return status
}

void SCH_Report_Status( void ) {
	#ifdef SCH_REPORT_ERRORS
	if( Error_code_G != Last_error_code_G ) {
		Error_port = 255 − Error_code_G ;
		Last_error_code_G = Error_code_G ;
		if ( Error_code_G != 0) {
			Error_tick_count_G = 60000;
		} else {
			Error_tick_count_G = 0;
		}
	} else {
		if ( Error_tick_count_G != 0) {
			if (−−Error_tick_count_G == 0) {
				Error_code_G = 0;
			}
		}
	}
	#endif
}
