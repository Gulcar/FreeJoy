/**
  ******************************************************************************
  * @file           : hx711.h
  * @brief          : Header for hx711.c file.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HX711_H__
#define __HX711_H__

#include <stdbool.h>
#include <stdint.h>
#include "common_types.h"
#include "periphery.h"

void HX711_SetDTPin(GPIO_TypeDef* port, uint16_t pin);
void HX711_SetSCKPin(GPIO_TypeDef* port, uint16_t pin);

// ali je data pripravljen za source
bool HX711_DataReady(int8_t source);

// vrne 24-bitno PREDZNACENO vrednost v 32 bitih
// next_source pove na kateri input nastavi naslednje branje
int32_t HX711_ReadValue32(int8_t next_source);

// vrne 16-bitno predznaceno vrednost
// next_source pove na kateri input nastavi naslednje branje
analog_data_t HX711_ReadValue(int8_t next_source);

#endif 	/* __HX711_H__ */

