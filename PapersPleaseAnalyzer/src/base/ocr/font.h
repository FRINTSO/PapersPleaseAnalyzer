#pragma once

#include "base/documents/document.h"
#include "base/documents_v2/doc_type.h"

// https://designmodo.com/letterform
// Anatomy:
// 1. Baseline
// 2. Cap height
// 3. Crossbar
// 4. Serif
// 5. Mean line
// 6. Bowl
// 7. Descender
// 8. Counter
// 9. Stem
// 10. Tittle
// 11. Terminal
// 12. Ascender
// 13. Leg
// 14. Ligature
// 15. X-height

enum class Typeface {
	Invalid = 0,
	_04b03,
	BM_Mini,
	MiniKylie,
	BoothNumber
};

struct FontInfo {
	const Typeface typeface;
	const int size;

	const int letterSpacingHorizontal;
	const int whitespaceSize;
	const int letterSpacingVertical;
	const int newlineSize;
};

const FontInfo& GetFontInfo(Documents::V1::DocumentType documentType);
const FontInfo& GetFontInfo(Documents::V2::DocType documentType);