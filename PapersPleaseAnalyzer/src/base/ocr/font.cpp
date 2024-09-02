#include "pch.h"
#include "base/ocr/font.h"

static inline constinit FontInfo g_Invalid{ Typeface::Invalid, 0, -1, -1 };
static inline constinit FontInfo g_04b04{ Typeface::_04b03, 14, 1, 2 /* Guess 3*/, 4, 2 };
static inline constinit FontInfo g_BM_MiniInfo{ Typeface::BM_Mini, 16, 1, 2, 2, 4 };
static inline constinit FontInfo g_MiniKylie{ Typeface::MiniKylie, 12, 2, 3, 4, 2 };
static inline constinit FontInfo g_BoothNumber{ Typeface::BoothNumber, 12, 2, 3 /* Guess 3 */, 4, 2 };

static constexpr Typeface GetTypefaceByDocumentType(Documents::V1::DocumentType documentType) {
	switch (documentType)
	{
	case Documents::V1::DocumentType::Passport:
		return Typeface::BM_Mini;
	case Documents::V1::DocumentType::IdentityCard:
		return Typeface::MiniKylie;
	case Documents::V1::DocumentType::DiplomaticAuthorization:
		return Typeface::BM_Mini;
	case Documents::V1::DocumentType::EntryTicket:
		return Typeface::BM_Mini;
	case Documents::V1::DocumentType::EntryPermit:
		return Typeface::BM_Mini;
	case Documents::V1::DocumentType::AccessPermit:
		return Typeface::BM_Mini;
	case Documents::V1::DocumentType::WorkPass:
		return Typeface::BM_Mini;
	case Documents::V1::DocumentType::GrantOfAsylum:
		return Typeface::BM_Mini;
	case Documents::V1::DocumentType::IdentitySupplement:
		return Typeface::BM_Mini;
	case Documents::V1::DocumentType::CertificateOfVaccination:
		return Typeface::BM_Mini;
	case Documents::V1::DocumentType::Booth:
		return Typeface::BoothNumber;
	default:
		return Typeface::Invalid;
	}
}

static constexpr Typeface GetTypefaceByDocumentType(const Documents::V2::DocType documentType)
{ // I think TypeFace should be in DocAppearance
	switch (documentType)
	{
		case Documents::V2::DocType::Passport:
			return Typeface::BM_Mini;
		case Documents::V2::DocType::IdentityCard:
			return Typeface::MiniKylie;
		case Documents::V2::DocType::DiplomaticAuthorization:
			return Typeface::BM_Mini;
		case Documents::V2::DocType::EntryTicket:
			return Typeface::BM_Mini;
		case Documents::V2::DocType::EntryPermit:
			return Typeface::BM_Mini;
		case Documents::V2::DocType::AccessPermit:
			return Typeface::BM_Mini;
		case Documents::V2::DocType::WorkPass:
			return Typeface::BM_Mini;
		case Documents::V2::DocType::GrantOfAsylum:
			return Typeface::BM_Mini;
		case Documents::V2::DocType::IdentitySupplement:
			return Typeface::BM_Mini;
		case Documents::V2::DocType::CertificateOfVaccination:
			return Typeface::BM_Mini;
		case Documents::V2::DocType::RuleBook:
			return Typeface::BM_Mini;
		case Documents::V2::DocType::Transcript:
			return Typeface::_04b03;
		default:
		{
			std::cerr << "Missing Document Typeface Info\n";
			assert(false);
		}
	}
}

static constexpr const FontInfo& GetFontInfoByTypeface(Typeface typeface) {
	switch (typeface)
	{
	case Typeface::Invalid:
		return g_Invalid;
	case Typeface::BM_Mini:
		return g_BM_MiniInfo;
	case Typeface::MiniKylie:
		return g_MiniKylie;
	case Typeface::BoothNumber:
		return g_BoothNumber;
	case Typeface::_04b03:
		return g_04b04;
	default:
		return g_Invalid;
	}
}

const FontInfo& GetFontInfo(const Documents::V1::DocumentType documentType) {
	Typeface type = GetTypefaceByDocumentType(documentType);
	return GetFontInfoByTypeface(type);
}

const FontInfo& GetFontInfo(const Documents::V2::DocType documentType) {
	Typeface type = GetTypefaceByDocumentType(documentType);
	return GetFontInfoByTypeface(type);
}