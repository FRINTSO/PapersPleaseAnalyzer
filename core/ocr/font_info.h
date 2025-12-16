#ifndef PAPLEASE_OCR_FONT_INFO_H
#define PAPLEASE_OCR_FONT_INFO_H

#include <paplease/compiler.h>
#include <paplease/ocr.h>

struct font_info {
	typeface tf;
	int max_pixels_tall;

	int letter_spacing_horizontal;
	int whitespace_pixel_width;
	int letter_spacing_vertical;
	int newline_size;
};

static constexpr font_info BOOTH_FONT_INFO{ .tf = typeface::booth,
					    .max_pixels_tall = 12,
					    .letter_spacing_horizontal = 4,
					    .whitespace_pixel_width = 6,
					    .letter_spacing_vertical = 8,
					    .newline_size = 4 };

static constexpr font_info BM_MINI_FONT_INFO{ .tf = typeface::bm_mini,
					      .max_pixels_tall = 16,
					      .letter_spacing_horizontal = 2,
					      .whitespace_pixel_width = 4,
					      .letter_spacing_vertical = 4,
					      .newline_size = 8 };

static constexpr font_info T04b04_FONT_INFO{ .tf = typeface::t04b03,
					     .max_pixels_tall = 14,
					     .letter_spacing_horizontal = 2,
					     .whitespace_pixel_width = 4,
					     .letter_spacing_vertical = 8,
					     .newline_size = 4 };

static constexpr font_info MINI_KYLIE_FONT_INFO{ .tf = typeface::mini_kylie,
						 .max_pixels_tall = 12,
						 .letter_spacing_horizontal = 2,
						 .whitespace_pixel_width = 4,
						 .letter_spacing_vertical = 4,
						 .newline_size = 4 };

inline const font_info &font_info_for(typeface tf)
{
	switch (tf) {
	case typeface::booth:
		return BOOTH_FONT_INFO;
	case typeface::bm_mini:
		return BM_MINI_FONT_INFO;
	case typeface::t04b03:
		return T04b04_FONT_INFO;
	case typeface::mini_kylie:
		return MINI_KYLIE_FONT_INFO;
	default:
		unreachable();
	}
}

#endif // PAPLEASE_OCR_FONT_INFO_H
