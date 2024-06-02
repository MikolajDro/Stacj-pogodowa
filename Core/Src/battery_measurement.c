/*
 * battery_measurement.c
 *
 *  Created on: Jun 2, 2024
 *      Author: mikol
 */

#include "battery_measurement.h"


uint32_t
measure_battery_level(void)
{
	MX_ADC1_Init();
	uint32_t adcValue = 0;

    // Uruchomienie ADC
    HAL_ADC_Start(&hadc1);

    // Czekaj na zakończenie konwersji
    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK) {
        // Pobierz wynik konwersji
        adcValue = HAL_ADC_GetValue(&hadc1);
    }

    // Zatrzymanie ADC
    HAL_ADC_Stop(&hadc1);

    // Usypianie ADC (wyłączanie zegara dla ADC)
    __HAL_RCC_ADC_FORCE_RESET();
    __HAL_RCC_ADC_RELEASE_RESET();


    ///TODO: make return battery level as 0-100
    /// now it is returning 0-4095
    /// add voltage divider
    return adcValue;
}
