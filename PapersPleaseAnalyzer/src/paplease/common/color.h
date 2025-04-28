#pragma once
#include <type_traits>

#include "paplease/common/common.h"
#include "paplease/types.h"

namespace paplease {

#if OPTIMIZE_COLOR

	struct RgbColor
	{
		constexpr RgbColor()
			: bgrVal(0)
		{}
		constexpr RgbColor(u8 red, u8 green, u8 blue)
			: bgrVal((static_cast<u32>(red) << 16) | (static_cast<u32>(green) << 8) | static_cast<u32>(blue))
		{}

		union
		{
			u32 bgrVal;
			struct
			{
				u8 blue;
				u8 green;
				u8 red;
				u8 _;
			};
		};

		constexpr inline u32 BgrValue() const noexcept
		{
			return bgrVal & 0x00'FF'FF'FF;
		}

		consteval inline u32 BgrValInstant() const noexcept
		{
			return bgrVal;
		}

	};

	struct BgrColor
	{
		constexpr BgrColor(u8 blue, u8 green, u8 red)
			: blue(blue), green(green), red(red)
		{

		}

		u8 blue;
		u8 green;
		u8 red;
	};

#define PRi32_RGB(color) "(" << (i32)color.red << ", " << (i32)color.green << ", " << (i32)color.blue << ")"

#define RGB_VAL(color) color.bgrVal

#define BGR_VAL(color) ((*(const RgbColor*)&color)).BgrValue()

	constexpr auto RgbVal(const RgbColor& color)
	{
		return color.bgrVal;
	}
	constexpr auto BgrVal(const BgrColor& color)
	{
		return ((*(const RgbColor*)&color)).BgrValue();
	}

#else
	struct RgbColor
	{
		u8 red;
		u8 green;
		u8 blue;
	};

	struct BgrColor
	{
		u8 blue;
		u8 green;
		u8 red;
	};


	static constexpr inline u32 ToBgrValue(BgrColor color)
	{
		return (color.blue << 16) | (color.green << 8) | color.red;
	}

	static constexpr inline u32 ToBgrValue(RgbColor color)
	{
		return (color.blue << 16) | (color.green << 8) | color.red;
	}

	static constexpr inline u32 ToRgbValue(RgbColor color)
	{
		return (color.red << 16) | (color.green << 8) | color.blue;
	}

	static constexpr inline u32 ToRgbValue(BgrColor color)
	{
		return (color.red << 16) | (color.green << 8) | color.blue;
	}


#define PRi32_RGB(color) "(" << (i32)color.red << ", " << (i32)color.green << ", " << (i32)color.blue << ")"

#define RGB_VAL(color) ToRgbValue(color)

#define BGR_VAL(color) ToRgbValue(color)

#endif

	struct HSVConfig
	{
		i32 hueMin = 0;
		i32 hueMax = 179;
		i32 satMin = 0;
		i32 satMax = 255;
		i32 valMin = 0;
		i32 valMax = 255;

		bool IsEmpty() const
		{
			return (hueMin | satMin | valMin) == 0
				&& (satMax & valMax) == 255
				&& (hueMax == 179);
		}
	};

	struct HSLConfig
	{
		i32 hueMin = 0;
		i32 hueMax = 179;
		i32 satMin = 0;
		i32 satMax = 255;
		i32 litMin = 0;
		i32 litMax = 255;
	};


	template<i32 RED, i32 GREEN, i32 BLUE>
	struct rgb_color : std::integral_constant<i32, (RED << 16) | (GREEN << 8) | BLUE>{};

	template<i32 RED, i32 GREEN, i32 BLUE>
	static constexpr i32 rgb_color_v = rgb_color<RED, GREEN, BLUE>::value;

}  // namespace paplease
