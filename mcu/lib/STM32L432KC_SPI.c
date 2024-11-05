// STM32L432KC_SPI.c
// Vikram Krishna
// vkrishna@hmc.edu
// November 4, 2024
// Implementation of the SPI helpers defined in the header file

#include "STM32L432KC_SPI.h"
#include "STM32L432KC_GPIO.h"

void initSPI(int br, int cpol, int cpha) {
  // 1. Configure clock for SPI
  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

  // 2. Configure GPIO pins for SPI
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

  // 3. Set the pins to be alternate functions
  pinMode(SPI_SCK_PIN, GPIO_ALT);
  pinMode(SPI_SDO_PIN, GPIO_ALT);
  pinMode(SPI_SDI_PIN, GPIO_ALT);
  pinMode(SPI_CS_PIN, GPIO_OUTPUT);

  // 4. Specify which alternate function in the GPIO registers
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL3, 5); // SCK
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL4, 5); // MISO
  GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5); // MOSI

  // Set the output speed to be high to match the SPI clock speed for the SCK
  // pin
  GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED3);

  // 5. Configure the SPI_CR1 register
  SPI1->CR1 = _VAL2FLD(SPI_CR1_BR, br);

  // 6. Configure the SPI_CR2 register
  SPI1->CR1 |= (SPI_CR1_MSTR);
  SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_LSBFIRST | SPI_CR1_SSM);
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);
  SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);
  SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111);
  SPI1->CR2 |= (SPI_CR2_FRXTH | SPI_CR2_SSOE);
  // SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111); // 8-bit data size
  // SPI1->CR2 |= SPI_CR2_FRXTH; // RXNE event is generated if the FIFO level is
  //                             // greater than or equal to 8-bit
  // SPI1->CR2 |= SPI_CR2_SSOE; // SS output is enabled in master mode and when
  // the
  //                            // SPI is enabled

  // 7. Enable the SPI peripheral
  SPI1->CR1 |= (SPI_CR1_SPE);
  digitalWrite(SPI_CS_PIN, PIO_HIGH);
  return;
}

char spiSendReceive(char send) {
  // 1. Wait until the TX buffer is empty
  while (!(SPI1->SR & SPI_SR_TXE))
    ;

  // 2. Write the data to the data register
  *(volatile char *)(&SPI1->DR) = send;

  // 3. Wait until the RX buffer is full
  while (!(SPI1->SR & SPI_SR_RXNE))
    ;
  char rec = (volatile char)SPI1->DR;
  // 4. Read the data from the data register
  return rec;
}