// DS1722.c
// Vikram Krishna
// vkrishna@hmc.edu
// November 4, 2024
// Implementation of the DS1722 helpers defined in the header file

#include "DS1722.h"
#include "STM32L432KC_GPIO.h"

void initDS1722() {
  // Initialize the DS1722
  initSPI(0b000, 0, 1); // Initialize the SPI peripheral
  digitalWrite(SPI_CS_PIN, PIO_HIGH); // Set the CS pin high
  writeConfig(0x00); // Write the configuration byte to the DS1722

  // Wait for the DS1722 to finish writing the configuration byte
  while (readTemp() != 0.0);
}

void writeConfig(uint8_t config) {
  // Write the configuration byte to the DS1722
  spiSendReceive(0x80); // Write to the configuration register
  spiSendReceive(config);
}

float readTemp() {
  // Read the temperature from the DS1722
  uint8_t tempMSB = spiSendReceive(0x02); // Read the MSB of the temperature
  uint8_t tempLSB = spiSendReceive(0x01); // Read the LSB of the temperature
  float temp = (tempMSB << 8 | tempLSB) * 2^-4; // Calculate the temperature
  return temp;
}