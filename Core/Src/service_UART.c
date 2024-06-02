/*
 * uartdma.c
 *
 *  Created on: May 15, 2024
 *      Author: mikol
 */
#include "service_UART.h"


char 	ReceiveBuffer[BUFFER_SIZE];
char	TransmitBuffer[BUFFER_SIZE];
service_state_t 	service_state = SERVICE_LOW_POWER_MODE;
RTC_AlarmTypeDef sAlarm = {0};


void
service_init(void)
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)ReceiveBuffer, BUFFER_SIZE);
}

void
rtc_reset_time(void)
{
	RTC_TimeTypeDef sTime = {0};
	sTime.Hours = 0;
	sTime.Minutes = 0;
	sTime.Seconds = 0;
	sTime.SubSeconds = 0;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}

void
rtc_reset_alarm(void)
{
	HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
}


void
rtc_init(void)
{
	RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};

	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		HAL_UART_Transmit_IT(&huart1, (uint8_t *)"RTC initialization error\r\n", 26);
	}


	sTime.Hours = 0;
	sTime.Minutes = 0;
	sTime.Seconds = 0;
	sTime.SubSeconds = 0;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		HAL_UART_Transmit_IT(&huart1, (uint8_t *)"RTC initialization error\r\n", 26);
	}
	sDate.WeekDay = RTC_WEEKDAY_MONDAY;
	sDate.Month = RTC_MONTH_JANUARY;
	sDate.Date = 1;
	sDate.Year = 0;
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		HAL_UART_Transmit_IT(&huart1, (uint8_t *)"RTC initialization error\r\n", 26);
	}

	sAlarm.AlarmTime.Hours = 0;
	sAlarm.AlarmTime.Minutes = 0;
	sAlarm.AlarmTime.Seconds = 10;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_HOURS | RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_MINUTES;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 1;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		HAL_UART_Transmit_IT(&huart1, (uint8_t *)"RTC initialization error\r\n", 26);
	}
}

// Cleans buffer
void
buffer_cleaner(char* buffer, uint16_t size)
{
	if(buffer == NULL)
		return;
	for(uint16_t i = 0; i < size; i++)
		buffer[i]=0;
}

/// UART RX interrupt callback
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
	if(huart->Instance == USART1)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t *)ReceiveBuffer, BUFFER_SIZE);
		service_state = SERVICE_ON;
		wakeup_timer(htim17.Instance);
	}
}


void
HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	epaper_state = MEASURE;
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
	rtc_reset_alarm();
	rtc_reset_time();

}


int
is_alphanumeric(uint8_t sign)
{
	if(sign < 48 || (sign > 57 && sign < 65) || sign > 122)
	{
		return NOT_ALPHANUMERIC;
	}
	return ALPHANUMERIC;
}



/**
 * @fn void service_read_buffer(uint8_t*, const uint16_t)
 * @param buffer: pointer to buffer
 * @param size: size of buffer
 *
 * AT command finder
 */
void
service_read_buffer(char *buffer, const uint16_t size)
{
	buffer_cleaner(TransmitBuffer, size);
	// AT commands
	if(buffer[0] == 'A' && buffer[1] == 'T')
	{
		if(buffer[2] == 0)
		{
			HAL_UART_Transmit_IT(&huart1, (uint8_t *)"AT OK\r\n", 7);
		}
		if(buffer[2] == '+')
		{
			if(strncmp((char *)&buffer[3], "VERSION?", strlen("VERSION?")) == 0)
			{
				HAL_UART_Transmit_IT(&huart1, (uint8_t *)"Version v1.0\r\n", 14);
			}
			else if(strncmp((char *)&buffer[3], "BAT?", strlen("BAT?")) == 0)
			{
				// TODO: check battery level
				uint8_t battery_level = 12;
				uint8_t battery_level_buffer[4];
				itoa((int )battery_level, (char *)battery_level_buffer, 10);
				strcpy(TransmitBuffer, "Battery level:");
				strcat(TransmitBuffer, (char *)battery_level_buffer);
				strcat(TransmitBuffer, "%\r\n");

				HAL_UART_Transmit_IT(&huart1, (uint8_t *)TransmitBuffer,
						strlen(TransmitBuffer));

			}
			else if(strncmp((char *)&buffer[3], "VALUES?", strlen("VALUES?")) == 0)
			{
				//TODO: change value with measured values
				uint8_t value = 12;
				char	value_buffor[5] = {};

				strcpy(TransmitBuffer, "Temp:");
				strcat(TransmitBuffer, itoa((int )value, value_buffor, 10));
				strcat(TransmitBuffer, "*C Hum:");
				strcat(TransmitBuffer, itoa((int )value, value_buffor, 10));
				strcat(TransmitBuffer, "% Press:");
				strcat(TransmitBuffer, itoa((int )value, value_buffor, 10));
				strcat(TransmitBuffer, "hPa AQI:");
				strcat(TransmitBuffer, itoa((int )value, value_buffor, 10));
				strcat(TransmitBuffer, "\r\n");

				HAL_UART_Transmit_IT(&huart1, (uint8_t *)TransmitBuffer,
										strlen(TransmitBuffer));

			}
			else if(strncmp((char *)&buffer[3], "MEASURE", strlen("MEASURE")) == 0)
			{
				// TODO: new measurement
				HAL_UART_Transmit_IT(&huart1, (uint8_t *)"Measuring\r\n", 10);
			}
			else if(strncmp((char *)&buffer[3], "INTERVAL=", strlen("INTERVAL=")) == 0)
			{
				int time=0;
				time = atoi((char *)&buffer[12]);
				if(time < 1 || time > 86399){
					HAL_UART_Transmit_IT(&huart1,
							(uint8_t *)"Invalid interval time. (86400s > interval > 10s)\r\n", 36);
				}
				uint8_t seconds, minutes, hours;
				char tmp[3];


				hours = time / 3600;
				time %= 3600;
				minutes = time / 60;
				seconds = time % 60;

				if(hours > 0){
					sAlarm.AlarmMask &= ~RTC_ALARMMASK_HOURS;
				}
				else{
					sAlarm.AlarmMask |= RTC_ALARMMASK_HOURS;
				}
				if(minutes > 0){
					sAlarm.AlarmMask &= ~RTC_ALARMMASK_MINUTES;
				}
				else{
					sAlarm.AlarmMask |= RTC_ALARMMASK_MINUTES;
				}

				sAlarm.AlarmTime.Hours = hours;
				sAlarm.AlarmTime.Minutes = minutes;
				sAlarm.AlarmTime.Seconds = seconds;

				rtc_reset_alarm();
				rtc_reset_time();

				strcpy(TransmitBuffer, "Measuring interval set to: ");
				strcat(TransmitBuffer, itoa((int )hours, tmp, 16));
				strcat(TransmitBuffer, "hours ");
				strcat(TransmitBuffer, itoa((int )minutes, tmp, 16));
				strcat(TransmitBuffer, "minutes ");
				strcat(TransmitBuffer, itoa((int )seconds, tmp, 16));
				strcat(TransmitBuffer, "seconds\r\n");
				HAL_UART_Transmit_IT(&huart1, (uint8_t *)TransmitBuffer,
						strlen(TransmitBuffer));
			}
			else if(strncmp((char *)&buffer[3], "SLEEP", strlen("SlEEP")) == 0)
			{
				HAL_UART_Transmit_IT(&huart1, (uint8_t *)"Enter low power mode\r\n", 10);
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
				service_state = SERVICE_LOW_POWER_MODE;
				disable_timer(htim17.Instance);
				timer_restart_counter_value(htim17.Instance);
			}
			else if(strncmp((char *)&buffer[3], "REFRESH", strlen("REFRESH")) == 0)
			{
				HAL_UART_Transmit_IT(&huart1, (uint8_t *)"Start refreshing e-paper\r\n", 10);
				epaper_state = REFRESH;
			}
			else
			{
				HAL_UART_Transmit_IT(&huart1, (uint8_t *)"AT command not found\r\n", 22);
			}
		}
	}

	buffer_cleaner(buffer, size);
	service_state = SERVICE_OFF;
}

// Testing purpose variables

void
wakeup_timer(TIM_TypeDef *TIMx)
{
	TIMx->CR1 |= TIM_CR1_CEN; // wakes up timer number x
}

void
disable_timer(TIM_TypeDef *TIMx)
{
	TIMx->CR1 &= ~TIM_CR1_CEN; // wakes up timer number x
}

uint32_t
timer_get_counter_value(TIM_TypeDef *TIMx) {
    return TIMx->CNT; // returns CNT counter value
}

void
timer_restart_counter_value(TIM_TypeDef *TIMx) {
    TIMx->CNT = 0; // CNT counter value set to 0
}

/**@fn void service(void)
 *
 * Main service function with
 * State machine in it
 */
void
service(void)
{
	switch(service_state)
	{
		case SERVICE_ON:
			HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
			service_read_buffer(ReceiveBuffer, BUFFER_SIZE);
			timer_restart_counter_value(htim17.Instance);
			break;
		case SERVICE_OFF:
			if(timer_get_counter_value(htim17.Instance) > MAX_SERVICE_TIME)
			{
				HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
				service_state = SERVICE_LOW_POWER_MODE;
				disable_timer(htim17.Instance);
				timer_restart_counter_value(htim17.Instance);
			}
			break;
		case SERVICE_LOW_POWER_MODE:
			break;
		default:
			break;
	}
}
