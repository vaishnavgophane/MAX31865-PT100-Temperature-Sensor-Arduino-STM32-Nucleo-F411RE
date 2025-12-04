/*
 * MAX31865.h
 *
 *  Created on: Dec 1, 2025
 *      Author: Vaishnav
 */

#ifndef INC_MAX31865_H_
#define INC_MAX31865_H_

#include "main.h"
#include "stm32f4xx_hal.h"


#define RREF_OHMS      430.0f
#define R0_OHMS        100.0f

/* MAX31865 register addresses */
#define MAX31865_REG_CONFIG        0x00
#define MAX31865_REG_RTD_MSB       0x01
#define MAX31865_REG_RTD_LSB       0x02
#define MAX31865_REG_FAULT_STATUS  0x07

/* Chip select GPIO for MAX31865 - change if you used different pin in CubeMX */
#define MAX31865_CS_GPIO_Port  SPI_CS_GPIO_Port
#define MAX31865_CS_Pin        SPI_CS_Pin



void MAX31865_CS_LOW(void);
void MAX31865_CS_HIGH(void);
void MAX31865_WriteRegister(uint8_t reg, uint8_t value);
uint8_t MAX31865_ReadRegister(uint8_t reg);
uint16_t MAX31865_ReadRTD(void);
float MAX31865_ReadResistance(void);
void MAX31865_Init(void);
float rtd_to_temperature(float rtd_resistance);
uint8_t MAX31865_ReadFault(void);
void MAX31865_ClearFaults(void);
#endif /* INC_MAX31865_H_ */
