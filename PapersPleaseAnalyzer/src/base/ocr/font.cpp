#include "pch.h"
#include "base/ocr/font.h"

static const FontInfo g_Invalid{ Typeface::Invalid, 0 };
static const FontInfo g_BM_MiniInfo{ Typeface::BM_Mini, 16 };
static const FontInfo g_MiniKylie{ Typeface::MiniKylie, 12 };
static const FontInfo g_BoothNumber{ Typeface::BoothNumber, 12 };

static Typeface GetTypefaceByDocumentType(DocumentType documentType) {
	switch (documentType)
	{
	case DocumentType::Passport:
		return Typeface::BM_Mini;
	case DocumentType::IdentityCard:
		return Typeface::MiniKylie;
	case DocumentType::DiplomaticAuthorization:
		return Typeface::BM_Mini;
	case DocumentType::EntryTicket:
		return Typeface::BM_Mini;
	case DocumentType::EntryPermit:
		return Typeface::BM_Mini;
	case DocumentType::AccessPermit:
		return Typeface::BM_Mini;
	case DocumentType::WorkPass:
		return Typeface::BM_Mini;
	case DocumentType::GrantOfAsylum:
		return Typeface::BM_Mini;
	case DocumentType::IdentitySupplement:
		return Typeface::BM_Mini;
	case DocumentType::CertificateOfVaccination:
		return Typeface::BM_Mini;
	case DocumentType::Booth:
		return Typeface::BoothNumber;
	default:
		return Typeface::Invalid;
	}

	return Typeface::Invalid;
}

static Typeface GetTypefaceByDocumentType(Documents::V2::DocType documentType) {
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
	default:
		return Typeface::Invalid;
	}

	return Typeface::Invalid;
}

static const FontInfo& GetFontInfoByTypeface(Typeface typeface) {
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
	default:
		return g_Invalid;
	}
}

const FontInfo& GetFontInfo(DocumentType documentType) {
	Typeface type = GetTypefaceByDocumentType(documentType);
	return GetFontInfoByTypeface(type);
}

const FontInfo& GetFontInfo(Documents::V2::DocType documentType) {
	Typeface type = GetTypefaceByDocumentType(documentType);
	return GetFontInfoByTypeface(type);
}