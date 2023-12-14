/*
 * BNO08x.c
 *
 *  Created on: Dec 12, 2023
 *      Author: arthur
 */

#include "BNO08x.h"

int bn_init(void){
	sh2_initialize();
}

void bno_reset(BNO08x *bno){
	HAL_GPIO_WritePin(bno->reset_port, bno->reset_pin, GPIO_PIN_RESET);
	HAL_Delay(20);
	HAL_GPIO_WritePin(bno->reset_port, bno->reset_pin, GPIO_PIN_SET);
	HAL_Delay(500);
}
