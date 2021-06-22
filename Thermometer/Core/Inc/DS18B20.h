#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "stm32f4xx_hal.h"
#include "gpio.h"
#include "tim.h"

#define DS18B20_Port DS18B20_DQ_GPIO_Port
#define DS18B20_Pin DS18B20_DQ_Pin

int DS18B20_Initialize();
void DS18B20_Write_Byte(uint8_t byte);
uint8_t DS18B20_Read_Byte();

#endif /* INC_DS18B20_H_ */
