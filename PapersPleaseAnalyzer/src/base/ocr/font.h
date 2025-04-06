#pragma once
#include "base/documents_v2/doc_type.h"

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
			const int size;

			const int letterSpacingHorizontal;
			const int whitespaceSize;
			const int letterSpacingVertical;
			const int newlineSize;
		};

		const FontInfo& GetFontInfo(documents::v2::DocType documentType);
		const FontInfo& GetBoothFontInfo();

	}  // namespace ocr
}  // namespace paplease
