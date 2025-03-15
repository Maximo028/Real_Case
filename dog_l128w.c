#include "stm32l4xx_hal.h"
#include "dogxl.h"
#include "font.h"

extern SPI_HandleTypeDef DOGXL128_SPI;

//Function that allows to send commands to the LCD
void DOGXL128_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(DOGXL128_DC_PORT, DOGXL128_DC_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DOGXL128_CS_PORT, DOGXL128_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&DOGXL128_SPI, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(DOGXL128_CS_PORT, DOGXL128_CS_PIN, GPIO_PIN_SET);
}

//Function that allows to send data to the LCD
void DOGXL128_WriteData(uint8_t data) {
    HAL_GPIO_WritePin(DOGXL128_DC_PORT, DOGXL128_DC_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(DOGXL128_CS_PORT, DOGXL128_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&DOGXL128_SPI, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(DOGXL128_CS_PORT, DOGXL128_CS_PIN, GPIO_PIN_SET);
}

//Reset screen
void DOGXL128_Reset() {
    HAL_GPIO_WritePin(DOGXL128_RST_PORT, DOGXL128_RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(DOGXL128_RST_PORT, DOGXL128_RST_PIN, GPIO_PIN_SET);
    HAL_Delay(10);
}

//Inicialize the screen with the commands below
void DOGXL128_Init() {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
	DOGXL128_Reset();
    DOGXL128_WriteCommand(0x40);  //Display Start Line 0
    DOGXL128_WriteCommand(0xA1);  //ADC Reverse
    DOGXL128_WriteCommand(0xC0);  //Normal COM0-COM63
    DOGXL128_WriteCommand(0xA6);  //Display Normal
    DOGXL128_WriteCommand(0xA2);  //Set BIAS 1/9
    DOGXL128_WriteCommand(0x2F);  //Booster
    DOGXL128_WriteCommand(0xF8);  //Internal Booster 4x
    DOGXL128_WriteCommand(0x00);  //Internal Booster 4x
    DOGXL128_WriteCommand(0x27);  //V0 voltage regulator
    DOGXL128_WriteCommand(0x81);  //Contrast set
    DOGXL128_WriteCommand(0x10);  //Contrast set
    DOGXL128_WriteCommand(0xAC);  //No indicator
    DOGXL128_WriteCommand(0x00);  //No indicator
    DOGXL128_WriteCommand(0xAF);  //Display ON
    DOGXL128_WriteCommand(0xB0);  //Page 0



}
//Set pixel from page "p" to column "c"
void DOGXL128_SetPixel(uint8_t p, uint8_t c, uint8_t data) {
    // Implementar lógica de mapeo de píxeles en framebuffer
	uint8_t i=0x00;
	uint8_t j=0x00;

	for(i = p;i < 8; i++){
		DOGXL128_WriteCommand(DOGXL128_SET_PAGE | i);
		for(j = c; j < 128; j++){
			DOGXL128_WriteCommand(DOGXL128_LOWER_COLUMN | (j & 0xf));
			DOGXL128_WriteCommand(DOGXL128_UPPER_COLUMN | ((j >> 4) & 0xf));
			//DOGXL128_WriteCommand(0x4f);
			DOGXL128_WriteData(data);
		}
	}
}

//Clear All the Screen
void DOGXL128_CleanScreen() {
	DOGXL128_SetPixel(0,0,0x00);
}

//Draw a Char (c) in the page and column sent
void DOGXL128_DrawChar(uint8_t page, uint8_t col, char c) {
    if (c < 32 || c > 127) return;  // Solo caracteres imprimibles

    const uint8_t *char_data = FONT_6X8[c - 32]; // Obtener datos del carácter

    DOGXL128_WriteCommand(DOGXL128_SET_PAGE | page);  // Seleccionar página
    DOGXL128_WriteCommand(DOGXL128_LOWER_COLUMN | (col & 0x0F));
    DOGXL128_WriteCommand(DOGXL128_UPPER_COLUMN | ((col >> 4) & 0x0F));

    for (uint8_t i = 0; i < 6; i++) {
        DOGXL128_WriteData(char_data[i]);  // Enviar cada columna del carácter
    }
}
//Draw a String in the page and col sent
void DOGXL128_DrawString(uint8_t page, uint8_t col, const char *str) {
    while (*str) {
        DOGXL128_DrawChar(page, col, *str++);
        col += 6; // Avanzar 6 píxeles para el siguiente carácter
        if (col >= DOGXL128_WIDTH) { // Si se sale de la pantalla, ir a la siguiente línea
            col = 0;
            page++;
            if (page >= 8) break; // Máximo 8 páginas (líneas)
        }
    }
}

//Point test
void DOGXL128_TestPattern() {
    for (uint8_t page = 0; page < 8; page++) {
    		  DOGXL128_WriteCommand(0xB0 | page);  // Seleccionar página

    		  for (uint8_t col = 0; col < DOGXL128_WIDTH; col++) {
    			  if(col%2==0){
    				  DOGXL128_WriteCommand(0x00 | (col & 0xf));         // Configurar columna alta
    				  DOGXL128_WriteCommand(0x10 | ((col >> 4) & 0xf));         // Configurar columna baja
    				  DOGXL128_WriteData(0xAA); // Patrón de prueba (10101010 en binario)
    		      }
    		  }
    }
}

void DOGXL128_DisplayImage(const uint8_t *image) {
    for (uint8_t page = 0; page < 8; page++) {
        DOGXL128_WriteCommand(DOGXL128_SET_PAGE | page);
        DOGXL128_WriteCommand(DOGXL128_LOWER_COLUMN | 0x00);
        DOGXL128_WriteCommand(DOGXL128_UPPER_COLUMN | 0x00);

        for (uint8_t col = 0; col < 128; col++) {
            DOGXL128_WriteData(image[page * 128 + col]); // Enviar byte a byte
        }
    }
}
