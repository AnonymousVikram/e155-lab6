// STM32L432KC_SPI.h
// Vikram Krishna
// vkrishna@hmc.edu
// November 5, 2024
// This file contains the function prototypes for the SPI peripheral on the
// STM32L432KC microcontroller.

#ifndef STM32L4_SPI_H
#define STM32L4_SPI_H

#include <stdint.h>
#include <stm32l432xx.h>

//////////////////////////////////////////////////////////////////////////////
// Pin definitions
//////////////////////////////////////////////////////////////////////////////

#define SPI_SCK PB3
#define SPI_CS PB7
#define SPI_SDO PB5
#define SPI_SDI PB4

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

/* Enables the SPI peripheral and intializes its clock speed (baud rate),
 * polarity, and phase.
 *    -- br: (0b000 - 0b111). The SPI clk will be the master clock / 2^(BR+1).
 *    -- cpol: clock polarity (0: inactive state is logical 0, 1: inactive state
 * is logical 1).
 *    -- cpha: clock phase (0: data captured on leading edge of clk and changed
 * on next edge, 1: data changed on leading edge of clk and captured on next
 * edge) Refer to the datasheet for more low-level details. */
void initSPI(int br, int cpol, int cpha);

/* Transmits a character (1 byte) over SPI and returns the received character.
 *    -- send: the character to send over SPI
 *    -- return: the character received over SPI */
char spiSendReceive(char send);

#endif