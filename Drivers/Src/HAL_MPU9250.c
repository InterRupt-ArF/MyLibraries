#include "HAL_MPU9250.h"

////////////////////////////////////////////////////////////////////////////////
MPU9250_Result MPU9250_Init(SPI_HandleTypeDef* SPIx , MPU9250TypeDef* datastruct, GPIO_TypeDef *port, uint16_t pin)
{
	uint8_t WHO_AM_I = (uint8_t)MPU9250_WHO_AM_I;
	uint8_t temp;
	uint8_t transmit_buffer[2]={0,0};
	
	/* Check who am I */
	MPU9250_ReadData(SPIx, &temp, WHO_AM_I, 1, port, pin);
	if(temp != MPU9250_I_AM)
	{
		//go to error function
	}
	
	/* Reset all the Registers */
	transmit_buffer[0] = (uint8_t)MPU9250_PWR_MGMT_1;
	transmit_buffer[1] = 0x80;
	MPU9250_WriteData(SPIx, transmit_buffer[0], transmit_buffer[1], port, pin);
	
	HAL_Delay(25);
	
	/* Configure User Control Register */
	transmit_buffer[0] = (uint8_t)MPU9250_USER_CTRL;
	transmit_buffer[1] = 0x10;
	MPU9250_WriteData(SPIx, 0x6A, 0x10, port, pin);
	HAL_Delay(5);

	/* Configure Gyro Config Register */
	transmit_buffer[0] = (uint8_t)MPU9250_CONFIG;
	transmit_buffer[1] = datastruct->Gyro_DLPF;
  MPU9250_WriteData(SPIx, transmit_buffer[0], transmit_buffer[1], port, pin);
	HAL_Delay(5);
	
	/* Configure Gyro_Config Register */
	transmit_buffer[0] = (uint8_t)MPU9250_GYRO_CONFIG;
	transmit_buffer[1] = (datastruct->Gyro_Range) << 3;
  MPU9250_WriteData(SPIx, transmit_buffer[0], transmit_buffer[1], port, pin);
	HAL_Delay(5);
	
	/* Configure Gyro Config Register */
	transmit_buffer[0] = (uint8_t)MPU9250_ACCEL_CONFIG_2;
	transmit_buffer[1] = datastruct->Accel_DLPF;
	MPU9250_WriteData(SPIx, transmit_buffer[0], transmit_buffer[1], port, pin);
	HAL_Delay(5);

	/* Configure Accel_Config Register */
	transmit_buffer[0] = (uint8_t)MPU9250_ACCEL_CONFIG;
	transmit_buffer[1] = (datastruct->Accel_Range) << 3;	
	MPU9250_WriteData(SPIx, transmit_buffer[0], transmit_buffer[1], port, pin);
	HAL_Delay(5);
	
	/* Return OK */
	return MPU9250_RESULT_OK;
}
////////////////////////////////////////////////////////////////////////////////
MPU9250_Result MPU9250_WriteData(SPI_HandleTypeDef* SPIx, uint8_t addr, uint8_t data, GPIO_TypeDef *port, uint16_t pin)
{
	uint8_t buffer[2] = {addr, data};
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(SPIx, buffer, 2, 10);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
	
	/* Return OK */
	return MPU9250_RESULT_OK;
}
////////////////////////////////////////////////////////////////////////////////
MPU9250_Result MPU9250_ReadData(SPI_HandleTypeDef* SPIx, uint8_t* buffer, uint8_t addr, uint8_t num, GPIO_TypeDef *port, uint16_t pin)
{
	uint8_t reg = addr | 0x80;
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(SPIx, &reg, 1, 10);
	HAL_SPI_Receive(SPIx, buffer, num, 10);
	HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
	
	/* Return OK */
	return MPU9250_RESULT_OK;
}
////////////////////////////////////////////////////////////////////////////////
