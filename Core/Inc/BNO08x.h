/*
 * BNO08x.h
 *
 *  Created on: Dec 12, 2023
 *      Author: arthur
 */

#ifndef INC_BNO08X_H_
#define INC_BNO08X_H_

#include "sh2.h"
#include "sh2_SensorValue.h"
#include "sh2_err.h"
#include "stm32f4xx_hal.h"

typedef struct {
	SPI_HandleTypeDef *spi;
	GPIO_TypeDef *reset_port;
	uint16_t *reset_pin;
	GPIO_TypeDef *int_port;
	uint16_t *int_pin;
	GPIO_TypeDef *cs_port;
	uint16_t *cs_pin;
} BNO08x;

#define BNO_RESET_GPIO_Port GPIOA
#define BNO_RESET_Pin GPIO_PIN_8
#define BNO_CS_GPIO_Port GPIOA
#define BNO_CS_Pin GPIO_PIN_9
#define BNO_INT_GPIO_Port GPIOA
#define BNO_INT_Pin GPIO_PIN_10

void bno_reset(BNO08x *bno);

#endif /* INC_BNO08X_H_ */
