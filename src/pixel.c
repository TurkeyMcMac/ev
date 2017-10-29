#include "pixel.h"

COLOR color(COLOR_CODE fore, COLOR_CODE back) {
	COLOR c;

	c.foreground = fore;
	c.background = back;

	return c;
}

PIXEL pixel(char symbol, COLOR color) {
	PIXEL p;

	p.symbol = symbol;
	p.color = color;

	return p;
}

int PIXEL_draw(PIXEL self, FILE* dest) {
	return fprintf(dest, "\x1B[3%c;4%cm%c",
		self.color.foreground,
		self.color.background,
		self.symbol
	);
}
