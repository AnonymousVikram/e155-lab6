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

  // Set up the CSR
  // uint8_t csr = (0b111 << 5) | 0b00000;
  spiSendReceive(0x80); // Send the write address
  spiSendReceive(0xEE); // Send the data to write
                        //
  // Unset the CS pin
  digitalWrite(SPI_CS, PIO_LOW);
  return;
}

void setPrecision(uint8_t prec) {
  digitalWrite(SPI_CS, PIO_HIGH);

  spiSendReceive(0x80);
  if (prec == 8) {
    spiSendReceive(0xE0);
    delay_millis(TIM15, 75);
  } else if (prec == 9) {
    spiSendReceive(0xE2);
    delay_millis(TIM15, 150);
  } else if (prec == 10) {
    spiSendReceive(0xE4);
    delay_millis(TIM15, 300);
  } else if (prec == 11) {
    spiSendReceive(0xE6);
    delay_millis(TIM15, 600);
  } else if (prec == 12) {
    spiSendReceive(0xE8);
    delay_millis(TIM15, 800);
  }

  digitalWrite(SPI_CS, PIO_LOW);
  return;
}

float readTemp() {
  // Set the CS pin HIGH
  digitalWrite(SPI_CS, PIO_HIGH);

  int8_t msb;
  uint8_t lsb;
  // Read the temperature
  spiSendReceive(0x02);
  msb = spiSendReceive(0x44); // Send the read address for the MSB
  digitalWrite(SPI_CS, PIO_LOW);
  digitalWrite(SPI_CS, PIO_HIGH);
  spiSendReceive(0x01);
  lsb = spiSendReceive(0x45); // Send the read address for the LSB

  // Unset the CS pin
  digitalWrite(SPI_CS, PIO_LOW);

  // Calculate the temperature (msb is sign and then 7 bits integer, lsb is 8
  // bits fractional)
  float temp = msb + (lsb / 256.0);

  return temp;
}