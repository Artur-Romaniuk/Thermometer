/*
 * DS18B20.c
 *
 *  Created on: Jun 22, 2021
 *      Author: artur
 */

#include "DS18B20.h"

//static functions declarations
static void micro_delay(uint16_t delay);
static void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
static void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
static void Set_Pin_Output_Defualt();
static void Set_Pin_Input_Defualt();

//functions

void test(uint16_t time, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin,1);
	for(int i=0; i<10;i++){
		micro_delay(time);
	}
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin,0);

}
int DS18B20_Initialize() {
	uint8_t response;
	Set_Pin_Output(DS18B20_Port, DS18B20_Pin);
	HAL_GPIO_WritePin(DS18B20_Port, DS18B20_Pin, GPIO_PIN_RESET);
	micro_delay(480); //datasheet
	Set_Pin_Input(DS18B20_Port, DS18B20_Pin);
	micro_delay(100); //datasheet
	if (!HAL_GPIO_ReadPin(DS18B20_Port, DS18B20_Pin)) {
		response = 0; //correct
	} else {
		response = 1; //empty line, error
	}
	micro_delay(380);
	return response;
}

void DS18B20_Write_Byte(uint8_t byte) {
	for (int i = 0; i < 8; i++) {
		if ((byte & (1 << i)) != 0) {
			Set_Pin_Output(DS18B20_Port, DS18B20_Pin);
			HAL_GPIO_WritePin(DS18B20_Port, DS18B20_Pin, GPIO_PIN_RESET);
			micro_delay(1);
			Set_Pin_Input(DS18B20_Port, DS18B20_Pin);
			micro_delay(60);
		} else {
			Set_Pin_Output(DS18B20_Port, DS18B20_Pin);
			HAL_GPIO_WritePin(DS18B20_Port, DS18B20_Pin, GPIO_PIN_RESET);
			micro_delay(60);
			Set_Pin_Input(DS18B20_Port, DS18B20_Pin);
		}
	}
}

uint8_t DS18B20_Read_Byte() {
	uint8_t byte = 0;
	Set_Pin_Input_Defualt();
	for(int i=0; i<8; i++)
	{
		Set_Pin_Output_Defualt();
		HAL_GPIO_WritePin(DS18B20_Port, DS18B20_Pin, GPIO_PIN_RESET);
		micro_delay(2);
		Set_Pin_Input_Defualt();
		micro_delay(10);
		if(HAL_GPIO_ReadPin(DS18B20_Port, DS18B20_Pin))
		{
			byte|=1<<i;
		}
		micro_delay(50);
	}
	return byte;
}

//static functions

static void micro_delay(uint16_t delay) {
	__HAL_TIM_SET_COUNTER(&htim6, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim6)) < delay)
		;
}

static void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

static void Set_Pin_Output_Defualt()
{
	Set_Pin_Output(DS18B20_Port, DS18B20_Pin);
}

static void Set_Pin_Input_Defualt()
{
	Set_Pin_Input(DS18B20_Port, DS18B20_Pin);
}
