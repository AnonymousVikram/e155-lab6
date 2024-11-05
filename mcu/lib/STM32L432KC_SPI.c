// STM32L432KC_SPI.c
// Vikram Krishna
// vkrishna@hmc.edu
// November 5, 2024
// This file contains the function definitions for the SPI peripheral on the
// STM32L432KC microcontroller.

#include "STM32L432KC.h"

void initSPI(int br, int cpol, int cpha) {
  // 1. Set up the GPIO pins for SPI
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // Enable the clock for GPIOA
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; // Enable the clock for GPIOB

  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // Enable the clock for SPI1

  // 2. Set up the pins for SPI1
  pinMode(SPI_SDO, GPIO_ALT);   // Set SDO to alternate function mode
  pinMode(SPI_SDI, GPIO_ALT);   // Set SDI to alternate function mode
  pinMode(SPI_CS, GPIO_OUTPUT); // Set CS to output mode
  pinMode(SPI_SCK, GPIO_ALT);   // Set SCK to alternate function mode
  GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED3); // Set SCK to high speed mode

  // 3. Set up the CR1 register
  // I think I'm going crazy :'(
  SPI1->CR1 &= ~SPI_CR1_SPE; // Disable SPI1
  SPI1->CR1 = 0;             // reset all the bits in the CR1 register
  SPI1->CR1 |=
      _VAL2FLD(SPI_CR1_BIDIMODE, 0); // Set to 2-line unidirectional mode
  SPI1->CR1 |=
      _VAL2FLD(SPI_CR1_BIDIOE,
               0); // Reset (shouldn't matter since we are in 2-line mode)
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_CRCEN, 0); // Disable CRC calculation
  SPI1->CR1 |=
      _VAL2FLD(SPI_CR1_CRCNEXT,
               0); // Reset (shouldn't matter since we are not using CRC)
  SPI1->CR1 |= _VAL2FLD(
      SPI_CR1_CRCL, 0); // Reset (shouldn't matter since we are not using CRC)
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_RXONLY, 0); // Set to full-duplex mode
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_SSM, 0);    // Set to hardware slave management
  SPI1->CR1 |=
      _VAL2FLD(SPI_CR1_SSI, 1); // Set to master mode (shouldn't matter since we
                                // are in hardware slave management)
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_LSBFIRST, 0); // Set to MSB first
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, br);      // Set the baud rate
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_MSTR, 1);     // Set to master mode
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);  // Set the clock polarity
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);  // Set the clock phase

  // 4. Set up the CR2 register
  SPI1->CR2 = 0; // Reset all the bits in the CR2 register
  SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111); // Set the data size to 8 bits
  SPI1->CR2 |= _VAL2FLD(SPI_CR2_FRXTH, 0);   // Set to receive FIFO threshold
  SPI1->CR2 |= _VAL2FLD(SPI_CR2_LDMARX, 0);  // Set to not use DMA for receive
  SPI1->CR2 |= _VAL2FLD(SPI_CR2_LDMATX, 0);  // Set to not use DMA for transmit
  SPI1->CR2 |= _VAL2FLD(SPI_CR2_TXEIE, 0); // Disable TX buffer empty interrupt
  SPI1->CR2 |=
      _VAL2FLD(SPI_CR2_RXNEIE, 0); // Disable RX buffer not empty interrupt
  SPI1->CR2 |= _VAL2FLD(SPI_CR2_SSOE, 1); // Set to enable SS output

  SPI1->CR1 |= _VAL2FLD(SPI_CR1_SPE, 1); // Enable SPI1
}

char spiSendReceive(char send) {
  // 1. Wait until the TX buffer is empty
  while (!(SPI1->SR & SPI_SR_TXE))
    ;

  // 2. Write the data to the TX buffer
  *(volatile uint8_t *)&SPI1->DR = send; // Cast to volatile to prevent compiler
                                         // optimization

  if (send >> 3) {
    return 0;
  }
  // 3. Wait until the RX buffer is not empty
  while (!(SPI1->SR & SPI_SR_RXNE))
    ;

  char received = (volatile char)SPI1->DR; // Again, cast to volatile to prevent
                                           // compiler optimization

  // 4. Read the data from the RX buffer
  return received;
}