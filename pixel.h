#ifndef _PIXEL_H

#define _PIXEL_H

#include <stdio.h>

typedef enum {
	BLACK	= '0',
	RED	= '1',
	GREEN	= '2',
	YELLOW	= '3',
	BLUE	= '4',
	MAGENTA	= '5',
	CYAN 	= '6',
	WHITE 	= '7',
} COLOR_CODE;

typedef struct {
	COLOR_CODE foreground;
	COLOR_CODE background;
} COLOR;

typedef struct {
	char symbol;
	COLOR color;
} PIXEL;

COLOR color(COLOR_CODE fore, COLOR_CODE back);

PIXEL pixel(char symbol, COLOR color);

void PIXEL_draw(PIXEL self, FILE* dest);

#endif
