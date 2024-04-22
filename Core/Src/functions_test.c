/*
 * functions_test.c
 *
 *  Created on: Apr 19, 2024
 *      Author: mikol
 */
#include "functions_test.h"
#include "simple.h"
#include "Debug.h"


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
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

	EPD_2IN13B_V4_Init();
	if(Image == NULL || Image->BlackImage == NULL || Image->Imagesize == 0)
	{
		return -1;
	}

    Paint_SelectImage(Image->BlackImage);
    Paint_Clear(WHITE);
	Paint_DrawBitMap_Paste(gInitImage122X250, 0, 0, 122, 250, BLACK);
    EPD_2IN13B_V4_Display(Image->BlackImage);
    return 0;
}

void EpaperWakeUp(void)
{
	EPD_2IN13B_V4_Init();
}


int DrawNormalTemperature(IMAGE_T *Image, STATION_DATA_T *station_data)
{
	if (BME680OFF)
		station_data->temp = 21.37;

	if(Image == NULL || station_data == NULL)
	{
		return -1;
	}

	//Set appropriate flag or end
	if(station_data->temp >= 0. && station_data->temp < 10.)
		station_data->tempFlag = 0;
	else if(station_data->temp >= 10. && station_data->temp < 20.)
		station_data->tempFlag = 1;
	else if(station_data->temp >= 20. && station_data->temp < 30.)
		station_data->tempFlag = 2;
	else if(station_data->temp >= 30. && station_data->temp < 40.)
		station_data->tempFlag = 3;
	else
		return -1;


	//Temperature level image selector
	switch (station_data->tempFlag){
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
	Paint_DrawString_EN(0, 150, STEMP, &Font8, WHITE, BLACK);
	station_data->xShift = station_data->temp >= 10. || station_data->temp <= 10. ? 45 : 34;
	Paint_DrawNumDecimals(0, 160, station_data->temp, &Font16, 1, BLACK, WHITE);
	Paint_DrawCircle(station_data->xShift, 163, 1, BLACK, 1, DRAW_FILL_EMPTY);
	Paint_DrawString_EN(station_data->xShift, 160, "C", &Font16, WHITE, BLACK);

	return 0;
}

int DrawNormalPressure(IMAGE_T *Image, STATION_DATA_T *station_data)
{
	if (BME680OFF)
		station_data->pres = 1002.123;

	if(Image == NULL || station_data == NULL)
	{
		return -1;
	}

	//Set appropriate flag or end
	if(station_data->pres > 900. && station_data->pres < 980.)
		station_data->presFlag = 0;
	else if(station_data->pres >= 980. && station_data->pres <= 1025.)
		station_data->presFlag = 1;
	else if(station_data->pres > 1025. && station_data->pres <= 1080.)
		station_data->presFlag = 2;
	else
		return -1;


	//Pressure level image selector
	switch (station_data->presFlag){
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
	Paint_DrawString_EN(10, 18, SPRES, &Font8, WHITE, BLACK);
	Paint_DrawNumDecimals(0, 30, station_data->pres, &Font20, 0, BLACK, WHITE);

	return 0;
}

int DrawNormalHumidity(IMAGE_T *Image, STATION_DATA_T *station_data)
{
	if (BME680OFF)
		station_data->hum = 68;\

	if(Image == NULL || station_data == NULL)
	{
		return -1;
	}

	//Set humidity flag
	if(station_data->hum >= 0. && station_data->hum < 45.)
		station_data->humFlag = 0;
	else if(station_data->hum >= 45. && station_data->hum <= 65.)
		station_data->humFlag = 1;
	else if(station_data->hum >= 66. && station_data->hum <= 100.)
		station_data->humFlag = 2;
	else
		return -1;


	//Humidity level image selector
	switch (station_data->humFlag){
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
	Paint_DrawString_EN(5, 210, SHUM, &Font8, WHITE, BLACK);
	Paint_DrawNumDecimals(0, 220, station_data->hum, &Font16, 1, BLACK, WHITE);
	station_data->xShift = station_data->hum > 10. ? 43 : 32;
	Paint_DrawString_EN(station_data->xShift, 220, "%", &Font16, WHITE, BLACK);

	return 0;
}

int DrawNormalAirQuality(IMAGE_T *Image, STATION_DATA_T *station_data)
{
	if (BME680OFF)
		station_data->air = 53;

	if(Image == NULL || station_data == NULL)
	{
		return -1;
	}

	//Set air flag
	if(station_data->air >= 0 && station_data->air <= 25)
		station_data->airFlag = 0;
	else if (station_data->air > 25 && station_data->air <= 50)
		station_data->airFlag = 1;
	else if (station_data->air > 50 && station_data->air <= 100)
		station_data->airFlag = 2;
	else if (station_data->air > 100 && station_data->air <= 210)
		station_data->airFlag = 3;
	else if (station_data->air > 210 && station_data->air <= 500)
		station_data->airFlag = 4;
	else
		return -1;


	//AIQ level image selector
	switch (station_data->airFlag){
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
	Paint_DrawString_EN(0, 80, SAIR, &Font8, WHITE, BLACK);
	if (station_data->air >= 0 && station_data->air <= 10)
		station_data->xShift = 21;
	else if (station_data->air < 100)
		station_data->xShift = 13;
	else
		station_data->xShift = 4;

	Paint_DrawNumDecimals(station_data->xShift, 92, station_data->air, &Font24, 0, BLACK, WHITE);

	return 0;
}

void DrawTopBar(IMAGE_T *Image, STATION_DATA_T *station_data)
{
	if (BME680OFF)
		station_data->bat = 53;

	Paint_DrawString_EN(10, 0, "KN KOIoT", &Font12, WHITE, BLACK);

	if(station_data->bat >= 80 && station_data->bat <= 100)
		station_data->batFlag = 0;
	else if(station_data->bat >= 40 && station_data->bat < 80)
		station_data->batFlag = 1;
	else if(station_data->bat >= 0 && station_data->bat < 40)
		station_data->batFlag = 2;
	else
		return;


	//Battery status
	Paint_DrawBitMap_Paste(gBattery45X10, 77, 0, 45, 10, BLACK);
	switch (station_data->batFlag){
		case 0:
			Paint_DrawRectangle(106, 3, 117, 8, BLACK, DRAW_FILL_FULL, DRAW_FILL_FULL);
		case 1:
			Paint_DrawRectangle(93, 3, 104, 8, BLACK, DRAW_FILL_FULL, DRAW_FILL_FULL);
		case 2:
			Paint_DrawRectangle(80, 3, 91, 8, BLACK, DRAW_FILL_FULL, DRAW_FILL_FULL);
		default:
			break;
	}
}

void Draw_Simple(IMAGE_T *Image, STATION_DATA_T *station_data)
{
	if(Image == NULL || station_data == NULL)
	{
		return;
	}
	Paint_DrawBitMap_Paste(gSimpleSimplest122X250, 0, 0, 122, 250, BLACK);

	if (BME680OFF)
	{
			station_data->temp = 5.412;
			station_data->air = 123;
			station_data->pres = 1012;
			station_data->hum = 92.221;
	}

	//Air
	Paint_DrawNumDecimals(0, 130, station_data->air, &Font20, 0, BLACK, WHITE);
	if (station_data->air >= 0 && station_data->air <= 10)
		station_data->xShift = 14;
	else if (station_data->air < 100)
		station_data->xShift = 28;
	else
		station_data->xShift = 42;
	Paint_DrawString_EN(station_data->xShift, 130, "IAQ", &Font20, WHITE, BLACK);

	//Temperature
	station_data->xShift = station_data->temp >= 10. || station_data->temp <= 10. ? 48 : 38;
	Paint_DrawNumDecimals(0, 150, station_data->temp, &Font20, 1, BLACK, WHITE);
	Paint_DrawCircle(station_data->xShift - 2, 153, 1, BLACK, 2, DRAW_FILL_EMPTY);
	Paint_DrawString_EN(station_data->xShift, 150, "C", &Font20, WHITE, BLACK);

	//Humidity
	Paint_DrawNumDecimals(0, 170, station_data->hum, &Font20, 1, BLACK, WHITE);
	station_data->xShift = station_data->hum > 10. ? 56 : 42;
	Paint_DrawString_EN(station_data->xShift - 3, 170, "%", &Font20, WHITE, BLACK);

	//Pressure
	Paint_DrawNumDecimals(0, 190, station_data->pres, &Font20, 0, BLACK, WHITE);
	station_data->xShift = station_data->pres >= 1000. ? 56 : 44;
	Paint_DrawString_EN(station_data->xShift, 190, "hPa", &Font20, WHITE, BLACK);


}

void DrawNormalTheme(IMAGE_T *Image, STATION_DATA_T *station_data)
{
	DrawTopBar(Image, station_data);
	DrawNormalPressure(Image, station_data);
	DrawNormalAirQuality(Image, station_data);
	DrawNormalTemperature(Image, station_data);
	DrawNormalHumidity(Image, station_data);
}

void DrawSimpleTheme(IMAGE_T *Image, STATION_DATA_T *station_data)
{
	Draw_Simple(Image, station_data);
	DrawTopBar(Image, station_data);
}





void EpaperUpdate(IMAGE_T *Image, STATION_DATA_T *station_data, uint8_t epaper_mode)
{
	EpaperWakeUp();
	EPD_2IN13B_V4_Clear();
	Paint_SelectImage(Image->BlackImage);
	Paint_Clear(WHITE);

	switch(epaper_mode)
	{
		case SIMPLE_MODE:
			DrawSimpleTheme(Image, station_data);
			break;
		case NORMAL_MODE:
			DrawNormalTheme(Image, station_data);
			break;
		case ADVANCED_MODE:
//			void DrawAdvancedTheme(Image, station_data);
			break;
		default:
			break;
	}

	EPD_2IN13B_V4_Display(Image->BlackImage);
	EPD_2IN13B_V4_Sleep();
}
