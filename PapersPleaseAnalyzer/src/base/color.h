#pragma once
#include "base/common.h"

#if COLOR_OPTIMIZATION

struct RgbColor {
	constexpr RgbColor()
		: bgrVal{0}
	{}
	constexpr RgbColor(unsigned char red, unsigned char green, unsigned char blue)
		: bgrVal{ ((uint32_t)red << 16) | ((uint32_t)green << 8) | (uint32_t)blue}
	{}

	union
	{
		uint32_t bgrVal;
		struct
		{
			unsigned char blue;
			unsigned char green;
			unsigned char red;
			unsigned char _;
		};
	};

	constexpr inline uint32_t BgrValue() const noexcept
	{
		return bgrVal & 0x00'FF'FF'FF;
	}

	constexpr inline uint32_t BgrValInstant() const noexcept
	{
		return bgrVal;
	}


};

struct BgrColor
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};

#define PRINT_RGB(color) "(" << (int)color.red << ", " << (int)color.green << ", " << (int)color.blue << ")"

#define RGB_VAL(color) color.bgrVal

#define BGR_VAL(color) ((*(const RgbColor*)&color)).BgrValue()
#else
struct RgbColor
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

struct BgrColor
{
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};


static constexpr inline uint32_t ToBgrValue(BgrColor color)
{
	return (color.blue << 16) | (color.green << 8) | color.red;
}

static constexpr inline uint32_t ToBgrValue(RgbColor color)
{
	return (color.blue << 16) | (color.green << 8) | color.red;
}

static constexpr inline uint32_t ToRgbValue(RgbColor color)
{
	return (color.red << 16) | (color.green << 8) | color.blue;
}

static constexpr inline uint32_t ToRgbValue(BgrColor color)
{
	return (color.red << 16) | (color.green << 8) | color.blue;
}


#define PRINT_RGB(color) "(" << (int)color.red << ", " << (int)color.green << ", " << (int)color.blue << ")"

#define RGB_VAL(color) ToRgbValue(color)

#define BGR_VAL(color) ToRgbValue(color)

#endif
