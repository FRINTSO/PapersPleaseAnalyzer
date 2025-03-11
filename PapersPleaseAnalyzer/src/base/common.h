#pragma once

namespace paplease {

#pragma region Options

#define DO_OPTIMIZATIONS 1
#define STRICT_DOCUMENT_SCANNING 1
#define USE_CONST_REF 0
#define USE_EXPERIMENTAL 1

#pragma endregion

#if USE_EXPERIMENTAL
#define MATCH_ONE_COLOR 0
#define MATCH_BORDER_COLOR 0 // NOT IMPLEMENTED
#define USE_ENUM_FUNCS 1
#define USE_NAMESPACE_ENUMS 1
#endif

#if DO_OPTIMIZATIONS
#define COLOR_OPTIMIZATION 1
#define CHEEKY_OPTIMIZATION 1
#define DOWNSCALE_OPTIMIZATION 0
#define EFFECTIVE_SCANNING_OPTIMIZATION 1
#define OCR_CHAR_CHECKSUM_OPTIMIZATION 1
#else
#define COLOR_OPTIMIZATION 0
#define CHEEKY_OPTIMIZATION 0
#define DOWNSCALE_OPTIMIZATION 0
#define EFFECTIVE_SCANNING_OPTIMIZATION 0
#define OCR_CHAR_CHECKSUM_OPTIMIZATION 0
#endif
#undef DO_OPTIMIZATIONS

#define IS_DOWNSCALED DOWNSCALE_OPTIMIZATION
	constexpr float SCALE = 0.5f;

	constexpr int Scale(int x)
	{
		if constexpr (IS_DOWNSCALED)
		{
			return static_cast<int>(x * SCALE);
		}
		else
		{
			return x;
		}
	}

#define DOWNSCALE(x) Scale(x)

#undef PREFER_MACRO

}  // namespace paplease
