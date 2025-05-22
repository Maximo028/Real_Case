#ifndef DOGXL128_H
#define DOGXL128_H

#include "stm32l4xx_hal.h"

#define DOGXL128_WIDTH 128
#define DOGXL128_HEIGHT 64
#define DOGXL128_NUM_PAGES 8

#define DOGXL128_SPI hspi1  // Definir el SPI usado
#define DOGXL128_CS_PORT GPIOC
#define DOGXL128_CS_PIN GPIO_PIN_7
#define DOGXL128_DC_PORT GPIOA
#define DOGXL128_DC_PIN GPIO_PIN_9
#define DOGXL128_RST_PORT GPIOA
#define DOGXL128_RST_PIN GPIO_PIN_8

#define DOGXL128_SET_PAGE 0xB0
#define DOGXL128_UPPER_COLUMN 0x10
#define DOGXL128_LOWER_COLUMN 0x00
extern SPI_HandleTypeDef DOGXL128_SPI;

void DOGXL128_Init(void);
void DOGXL128_Reset(void);
void DOGXL128_WriteCommand(uint8_t cmd);
void DOGXL128_WriteData(uint8_t data);
void DOGXL128_SetPixel(uint8_t p, uint8_t c, uint8_t data);
void DOGXL128_CleanScreen(void);
void DOGXL128_TestPattern(void);
void DOGXL128_DrawString(uint8_t page, uint8_t col, const char *str);
void DOGXL128_DrawChar(uint8_t page, uint8_t col, char c);
void DOGXL128_DisplayImage(const uint8_t *image);

#endif // DOGXL128_H
