/*
 * bme680_measure.h
 *
 *  Created on: Jun 2, 2024
 *      Author: mikol
 */

#ifndef INC_BME680_MEASURE_H_
#define INC_BME680_MEASURE_H_

#include "bme680.h"
#include "bme680_defs.h"
#include "main.h"
#include "i2c.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"

extern I2C_HandleTypeDef hi2c3;


// my structure
extern struct bme680_dev gas_sensor;
extern uint16_t meas_period;
extern struct bme680_field_data data;


void sensor_init(void);
void bme680_measure(void);
int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

#endif /* INC_BME680_MEASURE_H_ */
