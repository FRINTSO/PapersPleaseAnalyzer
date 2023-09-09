#pragma once

struct RgbColor {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

#define PRINT_RGB(color) "(" << (int)color.red << ", " << (int)color.green << ", " << (int)color.blue << ")"

#define RGB_VAL(color) ((color.red << 8) | (color.green << 4) | (color.blue))

#define BGR_VAL(color) ((color.blue << 8) | (color.green << 4) | (color.red))