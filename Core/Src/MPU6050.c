/*
 * MPU6050.c
 *
 *  Created on: Nov 30, 2023
 *      Author: arthur
 */


#include "MPU6050.h"
#define I2C_TIMEOUT 1000
#define ONBOARD_LED_PORT GPIOC
#define ONBOARD_LED_PIN GPIO_PIN_13

uint8_t ACCEL_RANGE = 0;


//void mpu_config_accel(MPU6050 *mpu, ){
//	uint8_t tx_buffer[]
//}

uint8_t get_accel_range(){
	switch (ACCEL_RANGE){
	case 0:
		return 2;
		break;
	case 1:
		return 4;
		break;
	case 2:
		return 8;
		break;
	case 3:
		return 16;
		break;
	}
	return 0;
}

void mpu_wakeup(MPU6050 *mpu){
    uint8_t tx_buffer[2];
    tx_buffer[0] = MPU6050_RA_PWR_MGMT_1;
    tx_buffer[1] = 0; // Clear the SLEEP bit to wake up
    mpu_send_cmd(mpu, tx_buffer, 2, NULL, 0);
    HAL_Delay(100);
}

void mpu_set_accel(MPU6050 *mpu, uint8_t accel_range){
	ACCEL_RANGE = accel_range;
	uint8_t tx_buffer[2];
	tx_buffer[0] = MPU6050_RA_ACCEL_CONFIG;
	tx_buffer[1] = 0;
	tx_buffer[1] |= ACCEL_RANGE << 5;
	mpu_send_cmd(mpu, tx_buffer, 2, NULL, 0);
}

void mpu_get_accelX(MPU6050 *mpu, float *accelX) {
	uint8_t tx_buffer[2];
	uint8_t rx_buffer[2];
	tx_buffer[0] = MPU6050_RA_ACCEL_XOUT_H;
	tx_buffer[1] = MPU6050_RA_ACCEL_XOUT_L;
	mpu_send_cmd(mpu, tx_buffer, 2, rx_buffer, 2);
    int16_t accelX_raw = (int16_t)((rx_buffer[1] << 8) | rx_buffer[0]);
    float scaleFactor = get_accel_range() / 32767.0;
    *accelX = accelX_raw * scaleFactor;
}

void mpu_get_accelY(MPU6050 *mpu, float *accelY) {
	uint8_t tx_buffer[2];
	uint8_t rx_buffer[2];
	tx_buffer[0] = MPU6050_RA_ACCEL_YOUT_H;
	tx_buffer[1] = MPU6050_RA_ACCEL_YOUT_L;
	mpu_send_cmd(mpu, tx_buffer, 2, rx_buffer, 2);
    int16_t accelY_raw = (int16_t)((rx_buffer[1] << 8) | rx_buffer[0]);
    float scaleFactor = get_accel_range() / 32767.0;
    *accelY = accelY_raw * scaleFactor;
}

void mpu_get_accelZ(MPU6050 *mpu, float *accelZ) {
	uint8_t tx_buffer[2];
	uint8_t rx_buffer[2];
	tx_buffer[0] = MPU6050_RA_ACCEL_ZOUT_H;
	tx_buffer[1] = MPU6050_RA_ACCEL_ZOUT_L;
	mpu_send_cmd(mpu, tx_buffer, 2, rx_buffer, 2);
    int16_t accelZ_raw = (int16_t)((rx_buffer[1] << 8) | rx_buffer[0]);
    float scaleFactor = get_accel_range() / 32767.0;
    *accelZ = accelZ_raw * scaleFactor;
}

void mpu_get_accel(MPU6050 *mpu, float *accel) {
	mpu_get_accelX(mpu, accel + 0);
	mpu_get_accelY(mpu, accel + 1);
	mpu_get_accelZ(mpu, accel + 2);
}

void mpu_send_cmd(MPU6050 *mpu, uint8_t *tx_buffer, uint8_t tx_len, uint8_t* rx_buffer, uint8_t rx_len) {
	HAL_StatusTypeDef status;
	if ((status = HAL_I2C_Master_Transmit(mpu->i2c, mpu->i2c_address << 1, tx_buffer, tx_len, I2C_TIMEOUT)) != HAL_OK) {
		if (status == HAL_ERROR){
			for (int i = 0; i < 4 ; i ++){
			HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 0);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 1);
			HAL_Delay(1000);
			}

		}
		if (status == HAL_BUSY){
			for (int i = 0; i < 4 ; i ++){
			HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 0);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 1);
			HAL_Delay(500);
			}
		}
		if (status == HAL_TIMEOUT){
			for (int i = 0; i < 4 ; i ++){
			HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 0);
			HAL_Delay(500);
			HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 1);
			HAL_Delay(1000);
			}
		}
	}

	if (rx_buffer != NULL && rx_len > 0) {
		if ((status = HAL_I2C_Master_Receive(mpu->i2c, (mpu->i2c_address << 1) | 1, rx_buffer, rx_len, I2C_TIMEOUT)) != HAL_OK) {
			if (status == HAL_ERROR){
				for (int i = 0; i < 4 ; i ++){
				HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 0);
				HAL_Delay(1000);
				HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 1);
				HAL_Delay(1000);
				}
			}
			if (status == HAL_BUSY){
				for (int i = 0; i < 4 ; i ++){
				HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 0);
				HAL_Delay(1000);
				HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 1);
				HAL_Delay(500);
				}
			}
			if (status == HAL_TIMEOUT){
				for (int i = 0; i < 4 ; i ++){
				HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 0);
				HAL_Delay(500);
				HAL_GPIO_WritePin(ONBOARD_LED_PORT, ONBOARD_LED_PIN, 1);
				HAL_Delay(1000);
				}
			}
		}
	}

    // Add a delay if needed
    HAL_Delay(100);
}
