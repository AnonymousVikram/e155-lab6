// DS1722.h
// Vikram Krishna
// vkrishna@hmc.edu
// November 5, 2024
// This file contains the function prototypes for the DS1722 temperature sensor.

#ifndef DS1722_H
#define DS1722_H

#include <stdint.h>
/////////////////////////////////////////////////////////////////////////////
// Function prototypes
/////////////////////////////////////////////////////////////////////////////
/* Initializes the DS1722 temperature sensor. */
void initTempSensor(void);

void setPrecision(uint8_t prec);

/* Reads the temperature from the DS1722 temperature sensor.
 *    -- return: the temperature in degrees Celsius */
float readTemp(void);

#endif