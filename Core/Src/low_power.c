/*
 * low_power.c
 *
 *  Created on: Jun 1, 2024
 *      Author: mikol
 */

#include "low_power.h"


void
low_power_mode(void)
{
	if(service_state == SERVICE_LOW_POWER_MODE && epaper_state == LOW_POWER)
	{
		HAL_SuspendTick();
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
		HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
		HAL_ResumeTick();
	}
}

