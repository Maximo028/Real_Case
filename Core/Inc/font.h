#ifndef FONT_H
#define FONT_H

#include <stdint.h>  // Para uint8_t

#define FONT_WIDTH 6   // Ancho de cada carácter en píxeles
#define FONT_HEIGHT 8  // Alto de cada carácter en píxeles

extern const uint8_t FONT_6X8[96][6];  // Declaración de la matriz de caracteres
extern const uint8_t DOGXL128_EtechBlack[];
extern const uint8_t DOGXL128_EtechWhite[];
extern const uint8_t DOGXL128_EtechName[];
#endif  // FONT_H
