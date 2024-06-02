/*
 * user_gui.h
 *
 *  Created on: Jun 1, 2024
 *      Author: mikol
 */

#ifndef INC_USER_GUI_H_
#define INC_USER_GUI_H_


#include "gpio.h"
#include "main.h"
#include "../epaper/epaper_functions.h"
#include <string.h>
#include <stdlib.h>


typedef enum{
	LOW_POWER	 	= -1,
	MEASURE 		= 0,
	REFRESH 		= 1,
} EPAPER_MACHINE_STATE;

extern EPAPER_MACHINE_STATE epaper_state;


void epaper_state_machine(void);

#endif /* INC_USER_GUI_H_ */
