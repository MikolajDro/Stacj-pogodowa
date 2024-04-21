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


typedef struct{
		uint8_t *BlackImage;
		uint16_t Imagesize;
} IMAGE_T;


IMAGE_T *ImageCreator(void);
int EpaperINIT(IMAGE_T *Image);
void EpaperWakeUp(void);
void Posiotion_test(IMAGE_T *Image);
void Draw_SimpleImage(IMAGE_T *Image);
void DrawNormalTheme(IMAGE_T *Image);


#endif /* INC_FUNCTIONS_TEST_H_ */
