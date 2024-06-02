/*
 * functions_test.h
 *
 *  Created on: Apr 19, 2024
 *      Author: mikol
 */

#ifndef INC_FUNCTIONS_TEST_H_
#define INC_FUNCTIONS_TEST_H_

#include "main.h"
#include "EPD_Test.h"
#include "EPD_2in13b_V4.h"
#include "GUI_Paint.h"
#include <time.h>
#include "back.h"
#include "Debug.h"
#include "images_and_theme.h"
#include "../Inc/usart.h"
#include "../Inc/user_gui.h"



#define STEMP 		"Temperature"
#define SPRES 		"Pressure"
#define SAIR 		"Air quality"
#define SHUM 		"Humidity"


#define SIMPLE_MODE 	1
#define NORMAL_MODE 	2
#define ADVANCED_MODE	3


typedef struct{
		uint8_t *BlackImage;
		uint16_t Imagesize;
} IMAGE_T;

typedef struct{
	float 		temp;
	float 		hum;
	float 		pres;
	int16_t		air;
	int8_t		bat;
	int8_t 		tempFlag;
	int8_t 		humFlag;
	int8_t 		presFlag;
	int8_t 		airFlag;
	int8_t		batFlag;
	uint8_t 	xShift;

}STATION_DATA_T;

extern IMAGE_T *Image;
extern STATION_DATA_T StationData;
extern uint8_t epaper_mode;


IMAGE_T *ImageCreator(void);
int EpaperINIT(void);
void EpaperWakeUp(void);
void Posiotion_test(IMAGE_T *Image);
void Draw_SimpleImage(IMAGE_T *Image);
int DrawNormalTemperature(IMAGE_T *Image, STATION_DATA_T *station_data);
int DrawNormalPressure(IMAGE_T *Image, STATION_DATA_T *station_data);
int DrawNormalHumidity(IMAGE_T *Image, STATION_DATA_T *station_data);
int DrawNormalAirQuality(IMAGE_T *Image, STATION_DATA_T *station_data);
void DrawNormalTheme(IMAGE_T *Image, STATION_DATA_T *station_data);
void EpaperUpdate();


#endif /* INC_FUNCTIONS_TEST_H_ */
