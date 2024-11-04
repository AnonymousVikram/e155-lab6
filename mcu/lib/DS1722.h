// DS1722.h
// Vikram Krishna
// vkrishna@hmc.edu
// November 4, 2024
// This file provides the prototypes for the DS1722 helper functions

#ifndef DS1722_H
#define DS1722_H

#include <stdint.h>
#include <stm32l432xx.h>
#include "STM32L432KC_SPI.h"

void initDS1722();

void writeConfig(uint8_t config);

float readTemp();

#endif