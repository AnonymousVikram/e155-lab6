// DS1722.c
// Vikram Krishna
// vkrishna@hmc.edu
// November 5, 2024
// This file contains the function prototypes for the DS1722 temperature sensor.

#include "DS1722.h"
#include "STM32L432KC.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_SPI.h"
#include "STM32L432KC_TIM.h"
#include <stdint.h>
void initTempSensor() {
  // Set the CS pin HIGH
  digitalWrite(SPI_CS, PIO_HIGH);
  delay_millis(TIM15, 1000);

  // Set up the CSR
  uint8_t csr = 0b00000;
  spiSendReceive(0x80);               // Send the write address
  spiSendReceive((0b111 << 5) | csr); // Send the data to write
  delay_millis(TIM15, 1000);

  // Unset the CS pin
  digitalWrite(SPI_CS, PIO_LOW);
}

float readTemp() {
  // Set the CS pin HIGH
  digitalWrite(SPI_CS, PIO_HIGH);

  // Read the temperature
  uint8_t msb = spiSendReceive(0x02); // Send the read address for the MSB
  uint8_t lsb = spiSendReceive(0x01); // Send the read address for the LSB

  // Unset the CS pin
  digitalWrite(SPI_CS, PIO_LOW);

  // Calculate the temperature (msb is the integer part, lsb is the fractional
  // part)
  float temp = msb + (lsb / 256.0);
  return temp;
}