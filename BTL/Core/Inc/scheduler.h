/*
 * scheduler.h
 *
 *  Created on: Nov 13, 2024
 *      Author: hoang
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

unsigned char ERROR_SCH_TOO_MANY_TASKS;
unsigned char ERROR_SCH_WAITING_FOR_SLAVE_TO_ACK;
unsigned char ERROR_SCH_WAITING_FOR_START_COMMAND_FROM_MASTER;
unsigned char ERROR_SCH_ONE_OR_MORE_SLAVES_DID_NOT_START;
unsigned char ERROR_SCH_LOST_SLAVE;
unsigned char ERROR_SCH_CAN_BUS_ERROR;
unsigned char ERROR_I2C_WRITE_BYTE_AT24C64;
unsigned char RETURN_NORMAL;
unsigned char ERROR_SCH_CANNOT_DELETE_TASK;
unsigned char RETURN_ERROR;

typedef struct{
	void(*pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
}sTasks;

#define SCH_MAX_TASKS 40

void SCH_Init(void);

void SCH_Add_Task(void (*pFunction)(), uint32_t DELAY,uint32_t PERIOD);

void SCH_Update(void);

void SCH_Dispath_Tasks(void);

uint8_t SCH_Delete_Task(uint32_t);

void SCH_Report_Status(void);

#endif /* INC_SCHEDULER_H_ */
