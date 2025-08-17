#pragma once
#include "paplease/common/scaled.h"
#include "paplease/documents/doc_type.h"

namespace paplease {
	namespace ocr {

		enum class Typeface
		{
			Invalid = 0,
			_04b03,
			BM_Mini,
			MiniKylie,
			BoothNumber
		};

		struct FontInfo
		{
			const Typeface typeface;
			const scaled<int> size;

			const scaled<int> letterSpacingHorizontal;
			const scaled<int> whitespaceSize;
			const scaled<int> letterSpacingVertical;
			const scaled<int> newlineSize;
		};

		const FontInfo& GetFontInfo(documents::DocType documentType);
		const FontInfo& GetBoothFontInfo();

	}  // namespace ocr
}  // namespace paplease
