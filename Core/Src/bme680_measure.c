/*
 * bme680_measure.c
 *
 *  Created on: Jun 2, 2024
 *      Author: mikol
 */

#include "bme680_measure.h"


struct bme680_dev gas_sensor;
uint16_t meas_period;
struct bme680_field_data data;


int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    /*
	 * The parameter dev_id can be used as a variable to store the I2C address of the device
	 */

	/*
	 * Data on the bus should be like
	 * |------------+---------------------|
	 * | I2C action | Data                |
	 * |------------+---------------------|
	 * | Start      | -                   |
	 * | Write      | (reg_addr)          |
	 * | Stop       | -                   |
	 * | Start      | -                   |
	 * | Read       | (reg_data[0])       |
	 * | Read       | (....)              |
	 * | Read       | (reg_data[len - 1]) |
	 * | Stop       | -                   |
	 * |------------+---------------------|
	 */

    rslt = HAL_I2C_Master_Transmit(&hi2c3, (dev_id << 1), &reg_addr, 1, HAL_MAX_DELAY);
    if(rslt != HAL_OK)
    	return -1;

    rslt = HAL_I2C_Master_Receive(&hi2c3, (dev_id << 1) | 0x01U, reg_data, len, HAL_MAX_DELAY);
    if(rslt != HAL_OK)
    	return -1;
    else
    	return 0;
}

int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Write      | (reg_data[0])       |
     * | Write      | (....)              |
     * | Write      | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */

    uint8_t *data;

    data = (uint8_t *)malloc(len + 1);
    data[0] = reg_addr;
    memcpy(data+1, reg_data, len);

    if(HAL_I2C_Master_Transmit(&hi2c3, (dev_id << 1), data, len+1, HAL_MAX_DELAY) != HAL_OK)
    	rslt = -1;
    else
    	rslt = 0;

    free(data);

    return rslt;
}


void
sensor_init(void)
{
	memset(&gas_sensor, 0, sizeof(gas_sensor));

	gas_sensor.dev_id = BME680_I2C_ADDR_SECONDARY; //Se considera la dirección 2 debido a que SDO queda conectado a VCC por medio de PULL-UP.
	gas_sensor.intf = BME680_I2C_INTF;
	gas_sensor.read = user_i2c_read;
	gas_sensor.write = user_i2c_write;

	/**
	 *  amb_temp can be set to 25 prior to configuring the gas sensor
	 * or by performing a few temperature readings without operating the gas sensor.
	 */
	gas_sensor.amb_temp = 25;

	if(bme680_init(&gas_sensor) != BME680_OK)
	{
		while(1)
			;
	}
	// Select desired oversampling rates
	gas_sensor.tph_sett.os_hum = BME680_OS_2X;
	gas_sensor.tph_sett.os_pres = BME680_OS_4X;
	gas_sensor.tph_sett.os_temp = BME680_OS_8X;

	// Sensor power on
	gas_sensor.power_mode = BME680_FORCED_MODE;

	// Set oversampling settings
	uint8_t required_settings = (BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL);
	int8_t rslt = bme680_set_sensor_settings(required_settings, &gas_sensor);

	// Set sensor mode
	rslt = bme680_set_sensor_mode(&gas_sensor);

	if(rslt != BME680_OK)
		HAL_UART_Transmit_IT(&huart1, (uint8_t *)"BME680 not initialize properly!\r\n", 33);

	bme680_get_profile_dur(&meas_period, &gas_sensor);
}

void
user_delay_ms(uint16_t period)
{
	HAL_Delay(period); // To avoid delay I'll need couple hours so it stays as it is
}


void
bme680_measure(void)
{
	gas_sensor.power_mode = BME680_FORCED_MODE;
	bme680_set_sensor_mode(&gas_sensor);

	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
	user_delay_ms(meas_period);
	bme680_get_sensor_data(&data, &gas_sensor);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

	gas_sensor.power_mode = BME680_SLEEP_MODE;
	bme680_set_sensor_mode(&gas_sensor);
}
















