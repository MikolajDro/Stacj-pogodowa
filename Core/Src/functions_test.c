/*
 * functions_test.c
 *
 *  Created on: Apr 19, 2024
 *      Author: mikol
 */
#include "functions_test.h"
#include "simple.h"


IMAGE_T *ImageCreator(void)
{
	static IMAGE_T NewImage;

	NewImage.Imagesize = ((EPD_2IN13B_V4_WIDTH % 8 == 0)? (EPD_2IN13B_V4_WIDTH / 8 ):
			(EPD_2IN13B_V4_WIDTH / 8 + 1)) * EPD_2IN13B_V4_HEIGHT;

	if((NewImage.BlackImage = (uint8_t *)malloc(NewImage.Imagesize)) == NULL)
	        return NULL;

    Paint_NewImage(NewImage.BlackImage, EPD_2IN13B_V4_WIDTH, EPD_2IN13B_V4_HEIGHT, 0, WHITE);

    Paint_SelectImage(NewImage.BlackImage);
    Paint_Clear(WHITE);

	return &NewImage;
}

int EpaperINIT(IMAGE_T *Image)
{
	EPD_2IN13B_V4_Init();
	if(Image == NULL || Image->BlackImage == NULL || Image->Imagesize == 0)
	{
		return -1;
	}

    Paint_SelectImage(Image->BlackImage);
    Paint_Clear(WHITE);
    EPD_2IN13B_V4_Display(Image->BlackImage);
    return 0;
}

void EpaperWakeUp(void)
{
	EPD_2IN13B_V4_Init();
}


void Posiotion_test(IMAGE_T *Image)
{
	if(Image == NULL || Image->BlackImage == NULL || Image->Imagesize == 0)
	{
		return;
	}
	Paint_SelectImage(Image->BlackImage);
	Paint_Clear(WHITE);
	Paint_DrawPoint(100, 100, BLACK, DOT_PIXEL_5X5, DOT_STYLE_DFT);

	EPD_2IN13B_V4_Display(Image->BlackImage);
}

void Draw_SimpleImage(IMAGE_T *Image)
{
	if(Image == NULL || Image->BlackImage == NULL || Image->Imagesize == 0)
	{
		return;
	}

	Paint_SelectImage(Image->BlackImage);
	Paint_Clear(WHITE);
	Paint_DrawBitMap_Paste(gSimpleSimplest122X250, 0, 0, 122, 250, BLACK);

	EPD_2IN13B_V4_Display(Image->BlackImage);
}

void DrawNormalTheme(IMAGE_T *Image)
{
	EPD_2IN13B_V4_Clear();
	Paint_SelectImage(Image->BlackImage);
	Paint_Clear(WHITE);

	Paint_DrawString_EN(10, 0, "KN KOIoT", &Font12, WHITE, BLACK);

	uint16_t tempFlag, hum, humFlag, bat, batFlag, air, airFlag, pres, presFlag; //prototype values
	uint8_t xShift;
	float temp = 10.4122;
	hum  = 50;
	air  = 129;
	pres = 1012;
	bat = 100; // Battery 1-100

	const char sTemp[] 	= "Temperature";
	const char sHum[]	= "Humidity";
	const char sAir[]	= "Air quality";
	const char sPres[]	= "Pressure";


	if(pres > 900 && pres < 980)
		presFlag = 0;
	else if(pres >= 980 && pres <= 1025)
		presFlag = 1;
	else if(pres > 1025 && pres <= 1080)
		presFlag = 2;
	else
		return;

	if(temp >= 0 && temp < 10)
		tempFlag = 0;
	else if(temp >= 10 && temp < 20)
		tempFlag = 1;
	else if(temp >= 20 && temp < 30)
		tempFlag = 2;
	else if(temp >= 30 && temp < 40)
		tempFlag = 3;
	else
		return;


	if(hum >= 0 && hum < 45)
		humFlag = 0;
	else if(hum >= 45 && hum <= 65)
		humFlag = 1;
	else if(hum >= 66 && hum <= 100)
		humFlag = 2;
	else
		return;


	if(air >= 0 && air <= 25)
		airFlag = 0;
	else if (air > 25 && air <= 50)
		airFlag = 1;
	else if (air > 50 && air <= 100)
		airFlag = 2;
	else if (air > 100 && air <= 210)
		airFlag = 3;
	else if (air > 210 && air <= 500)
		airFlag = 4;
	else
		return;


	if(bat >= 80 && bat <= 100)
		batFlag = 0;
	else if(bat >= 40 && bat < 80)
		batFlag = 1;
	else if(bat >= 0 && bat < 40)
		batFlag = 2;
	else
		return;


	//Pressure level image selector
	switch (presFlag){
		case 0:
			Paint_DrawBitMap_Paste(gNormalPressureLow60X60, 62, 10, 60, 60, BLACK);
			Paint_DrawString_EN(12, 48, "(Low)", &Font12, WHITE, BLACK);
			break;
		case 1:
			Paint_DrawBitMap_Paste(gNormalPressureGood60X60, 62, 10, 60, 60, BLACK);
			Paint_DrawString_EN(9, 48, "(Good)", &Font12, WHITE, BLACK);
			break;
		case 2:
			Paint_DrawBitMap_Paste(gNormalPressureHigh60X60, 62, 10, 60, 60, BLACK);
			Paint_DrawString_EN(9, 48, "(High)", &Font12, WHITE, BLACK);
			break;
		default:
			break;
	}
	Paint_DrawString_EN(10, 18, sPres, &Font8, WHITE, BLACK);
	Paint_DrawNumDecimals(0, 30, pres, &Font20, 0, BLACK, WHITE);


	//AIQ level image selector
	switch (airFlag){
		case 0:
			Paint_DrawBitMap_Paste(gNormalAirExcellent60X60 , 62, 70, 60, 60, BLACK);
			Paint_DrawString_EN(0, 116, "(Excellent)", &Font8, WHITE, BLACK);
			break;
		case 1:
			Paint_DrawBitMap_Paste(gNormalAirGood60X60 , 62, 70, 60, 60, BLACK);
			Paint_DrawString_EN(16,116, "(Good)", &Font8, WHITE, BLACK);
			break;
		case 2:
			Paint_DrawBitMap_Paste(gNormalAirBad60X60 , 62, 70, 60, 60, BLACK);
			Paint_DrawString_EN(2, 116, "(Moderate)", &Font8, WHITE, BLACK);
			break;
		case 3:
			Paint_DrawBitMap_Paste(gNormalAirVeryBad60X60 , 62, 70, 60, 60, BLACK);
			Paint_DrawString_EN(0, 116, "(Unhealthy*)", &Font8, WHITE, BLACK);
			break;
		case 4:
			Paint_DrawBitMap_Paste(gNormalAirVeryBad60X60 , 62, 70, 60, 60, BLACK);
			Paint_DrawString_EN(0, 116, "(Unhealthy)", &Font8, WHITE, BLACK);
			break;
		default:
			break;
	}
	Paint_DrawString_EN(0, 80, sAir, &Font8, WHITE, BLACK);
	if (air >= 0 && air <= 10)
		xShift = 21;
	else if (air < 100)
		xShift = 13;
	else
		xShift = 4;

	Paint_DrawNumDecimals(xShift, 92, air, &Font24, 0, BLACK, WHITE);


	//Temperature level image selector
	switch (tempFlag){
		case 0:
			Paint_DrawBitMap_Paste(gNormalTempTerriblyHot60X60, 62, 130, 60, 60, BLACK);
			break;
		case 1:
			Paint_DrawBitMap_Paste(gNormalTempVeryHot60X60, 62, 130, 60, 60, BLACK);
			break;
		case 2:
			Paint_DrawBitMap_Paste(gNormalTempGood60X60, 62, 130, 60, 60, BLACK);
			break;
		case 3:
			Paint_DrawBitMap_Paste(gNormalTempHot60X60, 62, 130, 60, 60, BLACK);
			break;
		default:
			break;
	}
	Paint_DrawString_EN(0, 150, sTemp, &Font8, WHITE, BLACK);
	xShift = temp >= 10. || temp <= 10. ? 45 : 34;
	Paint_DrawNumDecimals(0, 160, temp, &Font16, 1, BLACK, WHITE);
	Paint_DrawCircle(xShift, 163, 1, BLACK, 1, DRAW_FILL_EMPTY);
	Paint_DrawString_EN(xShift, 160, "C", &Font16, WHITE, BLACK);


	//Humidity level image selector
	switch (humFlag){
		case 0:
			Paint_DrawBitMap_Paste(gNormalHumLow60X60, 62, 190, 60, 60, BLACK);
			break;
		case 1:
			Paint_DrawBitMap_Paste(gNormalHumGood60X60, 62, 190, 60, 60, BLACK);
			break;
		case 2:
			Paint_DrawBitMap_Paste(gNormalHumHigh60X60, 62, 190, 60, 60, BLACK);
			break;
		default:
			break;
	}
	Paint_DrawString_EN(5, 210, sHum, &Font8, WHITE, BLACK);
	Paint_DrawNumDecimals(0, 220, hum, &Font16, 1, BLACK, WHITE);
	xShift = hum > 10 ? 43 : 32;
	Paint_DrawString_EN(xShift, 220, "%", &Font16, WHITE, BLACK);


	//Battery status
	Paint_DrawBitMap_Paste(gBattery45X10, 77, 0, 45, 10, BLACK);
	switch (batFlag){
		case 0:
			Paint_DrawRectangle(106, 3, 117, 8, BLACK, DRAW_FILL_FULL, DRAW_FILL_FULL);
		case 1:
			Paint_DrawRectangle(93, 3, 104, 8, BLACK, DRAW_FILL_FULL, DRAW_FILL_FULL);
		case 2:
			Paint_DrawRectangle(80, 3, 91, 8, BLACK, DRAW_FILL_FULL, DRAW_FILL_FULL);
		default:
			break;
		}


	EPD_2IN13B_V4_Display(Image->BlackImage);
}
