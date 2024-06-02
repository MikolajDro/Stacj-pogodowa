/*
 * user_gui.c
 *
 *  Created on: Jun 1, 2024
 *      Author: mikol
 */

#include "user_gui.h"



EPAPER_MACHINE_STATE epaper_state = LOW_POWER;



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == B1_Pin)
    {
    	while(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
		{
			;
		}
    	epaper_mode = SIMPLE_MODE;
    	epaper_state = REFRESH;
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
    }
    else if (GPIO_Pin == B2_Pin)
    {
    	while(HAL_GPIO_ReadPin(B2_GPIO_Port, B2_Pin) == GPIO_PIN_RESET)
		{
			;
		}
    	epaper_mode = NORMAL_MODE;
    	epaper_state = REFRESH;
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    }
    else if (GPIO_Pin == B3_Pin)
    {
    	while(HAL_GPIO_ReadPin(B3_GPIO_Port, B3_Pin) == GPIO_PIN_RESET)
		{
			;
		}

    	epaper_state = MEASURE;
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
    }
}


void
epaper_state_machine(void)
{
	switch(epaper_state)
	{
		case LOW_POWER:
			break;
		case MEASURE:
			/// TODO: make measurement with bme680
			EpaperUpdate();
			epaper_state = LOW_POWER;
			break;
		case REFRESH:
			EpaperUpdate();
			epaper_state = LOW_POWER;
			break;
		default:
			break;
	}
}


