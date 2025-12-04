/*
 * MAX31865.c
 *
 *  Created on: Dec 1, 2025
 *      Author: Vaishnav
 */

#include <main.h>
#include<math.h>
#include "MAX31865.h"
#include "stm32f4xx_hal.h"



extern SPI_HandleTypeDef hspi1;

extern UART_HandleTypeDef huart2;


/******************************************************************************************************************************/
												/* Hardware Helper Functions */
void MAX31865_CS_LOW(void)
{
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_RESET); // Starts Communication
}
void MAX31865_CS_HIGH(void)
{
    HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);   // Stops Communication
}
/******************************************************************************************************************************/


/******************************************************************************************************************************/
											/* Write single register (MSB=1 for write) */
void MAX31865_WriteRegister(uint8_t reg, uint8_t value)	// reg = register address inside MAX31865
{														// value = data byte you want to write into that register
    uint8_t tx[2];										// 2 bytes buffer
    tx[0] = reg | 0x80; 								// OR-ing with 0x80 gives 1 i.e. MSB = 1 -> Write Operation
    tx[1] = value;										// Value = reg value

    MAX31865_CS_LOW();									// SPI Start
    HAL_SPI_Transmit(&hspi1, tx, 2, HAL_MAX_DELAY);		// Tramsmitted 2 bytes via SPI
    MAX31865_CS_HIGH();									// SPI stop
}
/******************************************************************************************************************************/


/******************************************************************************************************************************/
											/* Read single register (MSB=0 for read) */
/*This function reads one byte from a specific register of the MAX31865 RTD converter using SPI */
uint8_t MAX31865_ReadRegister(uint8_t reg)
{
    uint8_t tx = reg & 0x7F;	//ensures MSB = 0 → this becomes a read command.
    							// 0111 1111
    							//   7    f   = 7F; keeps bit 0 to Read Operation
    uint8_t rx = 0;

    MAX31865_CS_LOW();			// SPI Start to receive
    HAL_SPI_Transmit(&hspi1, &tx, 1, HAL_MAX_DELAY); //send 1 byte register address
    HAL_SPI_Receive(&hspi1, &rx, 1, HAL_MAX_DELAY);	// receives 1 byte in response
    MAX31865_CS_HIGH();			// SPI Stops

    return rx;					// Caller receives register contents e.g. fault status
}
/******************************************************************************************************************************/


/******************************************************************************************************************************/
													/* Read 15-bit RTD ADC value */
/*This function reads the raw 15-bit RTD ADC value from the MAX31865 by reading two registers:
* RTD MSB register
* RTD LSB register
*/
uint16_t MAX31865_ReadRTD(void)
{
	//MAX31865_WriteRegister(0x00, 0xC3);
	//HAL_Delay(100);
	uint8_t msb = MAX31865_ReadRegister(MAX31865_REG_RTD_MSB); // read MSB bytes (15 - 8)
    uint8_t lsb = MAX31865_ReadRegister(MAX31865_REG_RTD_LSB); // read LSB bytes (7 - 1)
    														   // Bit 0 = 1; fault flag.

    uint16_t raw = ((uint16_t)msb << 8) | lsb >> 1;				   // Combines MSB & LSB values
    														// shifting MSB so LSB can have space
    //raw >>= 1; 												// removes fault bit (D0)
    return raw;												// Returns raw value i.e. later used to calculate temperature */
   // uint16_t MAX31865_ReadRTD(void)
//    {
//        uint8_t tx = 0x01;   // MSB address
//        uint8_t buffer[2];
//
//        MAX31865_CS_LOW();
//        HAL_SPI_Transmit(&hspi1, &tx, 1, HAL_MAX_DELAY);
//        HAL_SPI_Receive(&hspi1, buffer, 2, HAL_MAX_DELAY);
//        MAX31865_CS_HIGH();
//
//        uint16_t rtd = ((uint16_t)buffer[0] << 8) | buffer[1];
//        rtd >>= 1;  // remove fault bit
//
//        return rtd;
//    }

}
/******************************************************************************************************************************/


/******************************************************************************************************************************/
										/* Convert RTD ADC counts to resistance (Ohms) */
float MAX31865_ReadResistance(void)
{
	/* This function converts the raw 15-bit RTD ADC value from the MAX31865 into
	   the actual RTD sensor resistance (Ohms)
	   ADC value -> Actual RTD resistance */

    uint16_t rtd = MAX31865_ReadRTD();
    float resistance = ((float)rtd * RREF_OHMS) / 32768.0f;
    				/* works like
    				 * Rrtd = (ADC Count / 32786) x Rref;
    				 * where ADC count = rtd vaule
    				 * RREF_OHMS = known refrance resistance on the board
    				 * 32768 = 12^15;
    				 */
    return resistance; // return in ohms
}
/******************************************************************************************************************************/


/******************************************************************************************************************************/
								/* Simple init: enable VBIAS and auto-convert, 2-wire default, filter 50Hz */
void MAX31865_Init(void)
{
	uint8_t config = 0;

	config |= (1 << 7);   // VBIAS ON
	config |= (1 << 6);   // Auto conversion
	config |= (0 << 4);   // 2/4 wire
	config |= (1 << 1);   // Fault clear
	config |= (1 << 0);   // 50 Hz filter

	MAX31865_WriteRegister(0x00, 0xC3);

}
/******************************************************************************************************************************/


/******************************************************************************************************************************/
													/* Read fault register */
uint8_t MAX31865_ReadFault(void)
{
	/* This function reads the fault status register of the MAX31865,
	 * which tells us if there is any wiring issue, open/short RTD,
	 *  under/over-voltage, or general sensor fault as mentioned in Data sheet */
    return MAX31865_ReadRegister(MAX31865_REG_FAULT_STATUS);
    //fault status register address (0x07)
}
/******************************************************************************************************************************/


/******************************************************************************************************************************/
								/* Clear fault status bit by setting bit1 (F_CLR) temporarily then restore */
void MAX31865_ClearFaults(void)
{	/* This function clears any fault flags in the MAX31865 by toggling the
 	 FAULT CLEAR bit in the configuration register (bit 1).*/
    uint8_t config = MAX31865_ReadRegister(MAX31865_REG_CONFIG); // Reads current configuration
    uint8_t cfg_with_clear = config | (1 << 1); 					// set fault clear bit
    MAX31865_WriteRegister(MAX31865_REG_CONFIG, cfg_with_clear); // writes modified configuration to clear fault
    HAL_Delay(2); // delay mandatory because it is a analog device

    /* restore original config (clear the F_CLR bit) */
    MAX31865_WriteRegister(MAX31865_REG_CONFIG, config);
    HAL_Delay(2);
}
/******************************************************************************************************************************/


/******************************************************************************************************************************/
			/* Convert RTD resistance to Temperature using Callendar-Van Dusen quadratic (0°C and above)
   R(T) = R0*(1 + A*T + B*T^2)
   Solve B*T^2 + A*T + (1 - R/R0) = 0
   using A=3.9083e-3, B=-5.775e-7 (standard)
*/
float rtd_to_temperature(float R)
{
    const float R0 = 100.0f;        // PT100
    const float A  = 3.9083e-3f;
    const float B  = -5.775e-7f;

    float ratio = R / R0;

    float temp = (-A + sqrtf(A*A - 4.0f*B*(1.0f - ratio))) / (2.0f*B);
    return temp;
}

/******************************************************************************************************************************/





