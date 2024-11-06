// STM32L432KC_SPI.c
// Vikram Krishna
// vkrishna@hmc.edu
// November 5, 2024
// This file contains the function definitions for the SPI peripheral on the
// STM32L432KC microcontroller.

#include "STM32L432KC.h"

void initSPI(int br, int cpol, int cpha) {
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Get the clock going into the SPI
  pinMode(SPI_SDI, GPIO_ALT);
  pinMode(SPI_SDO, GPIO_ALT);
  pinMode(SPI_CS, GPIO_OUTPUT);
  digitalWrite(SPI_CS, PIO_LOW);
  pinMode(SPI_SCK, GPIO_ALT);
  GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED3); // Make the clock output high speed

  // Tell the MCU which alternate function to use
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL3, 5);
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL4, 5);
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5);

  SPI1->CR1 = 0;
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, br); // Set baud rate divider
  SPI1->CR1 |= (SPI_CR1_MSTR);
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha); // Set the phase
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol); // set the polarity
  SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111); // Set the bit package to 8
  SPI1->CR2 |= (SPI_CR2_FRXTH | SPI_CR2_SSOE);

  SPI1->CR1 |= (SPI_CR1_SPE); // Enable SPI
}

char spiSendReceive(char send) {
  // 1. Wait until the TX buffer is empty
  while (!(SPI1->SR & SPI_SR_TXE))
    ;

  // 2. Write the data to the TX buffer
  *(volatile char *)(&SPI1->DR) = send; // Cast to volatile to prevent
                                        // compiler optimization

  // 3. Wait until the RX buffer is not empty
  while (!(SPI1->SR & SPI_SR_RXNE))
    ;

  volatile char received =
      (volatile char)SPI1->DR; // Again, cast to volatile to
                               // prevent compiler optimization
  // 4. Read the data from the RX buffer
  return received;
}