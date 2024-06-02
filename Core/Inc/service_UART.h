/*
 * uartdma.h
 *
 *  Created on: May 15, 2024
 *      Author: mikol
 */

#ifndef INC_UARTDMA_H_
#define INC_UARTDMA_H_

#include <string.h>

#include "main.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "user_gui.h"

// defines
#define BUFFER_SIZE			128
#define NOT_ALPHANUMERIC 	0
#define ALPHANUMERIC 		1
#define MAX_SERVICE_TIME 	1000 * 1 * 1 // 5 minutes (1000ms * 60 seconds * 5 minutes)


// struct defines
typedef enum{
	SERVICE_OFF 			= -1,
	SERVICE_ON				= 0,
	SERVICE_LOW_POWER_MODE	= 1,
}service_state_t;



// Variabels defines
extern char ReceiveBuffer[BUFFER_SIZE];
extern service_state_t	service_state;
extern RTC_AlarmTypeDef sAlarm;

// External defines
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern TIM_HandleTypeDef htim17;


// function defines
void service_init(void);
void rtc_init(void);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void disable_timer(TIM_TypeDef *TIMx); // used in initialization
void wakeup_timer(TIM_TypeDef *TIMx);
void timer_restart_counter_value(TIM_TypeDef *TIMx);
void service(void);



#endif /* INC_UARTDMA_H_ */
