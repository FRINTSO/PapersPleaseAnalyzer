#include "pch.h"
#include "base/ocr/font.h"

#include <cassert>
#include <iostream>

#include "base/common.h"
#include "base/documents/doc_type.h"

namespace paplease {
	namespace ocr {

		static inline constinit FontInfo g_Invalid{ Typeface::Invalid, 0, -1, -1, -1, -1 };
		static inline constinit FontInfo g_04b04{ Typeface::_04b03, 14, 2, 4, 8, 4 };
		static inline constinit FontInfo g_BM_MiniInfo{ Typeface::BM_Mini, 16, 2, 4, 4, 8 };
		static inline constinit FontInfo g_MiniKylie{ Typeface::MiniKylie, 12, 2, 4, 4, 4 };
		static inline constinit FontInfo g_BoothNumber{ Typeface::BoothNumber, 12, 4, 6, 8, 4 };

		static constexpr Typeface GetTypefaceByDocumentType(const documents::DocType documentType)
		{
			switch (documentType)
			{
				case documents::DocType::AccessPermit:
				case documents::DocType::CertificateOfVaccination:
				case documents::DocType::DiplomaticAuthorization:
				case documents::DocType::EntryPermit:
				case documents::DocType::EntryTicket:
				case documents::DocType::GrantOfAsylum:
				case documents::DocType::IdentitySupplement:
				case documents::DocType::Passport:
				case documents::DocType::RuleBook:
				case documents::DocType::WorkPass:
					return Typeface::BM_Mini;
				case documents::DocType::IdentityCard:
					return Typeface::MiniKylie;
				case documents::DocType::Transcript:
					return Typeface::_04b03;
				default:
				{
					std::cerr << "Missing Document Typeface Info\n";
					assert(false);
				}
			}
		}

		static constexpr const FontInfo& GetFontInfoByTypeface(Typeface typeface)
		{
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

		const FontInfo& GetFontInfo(const documents::DocType documentType)
		{
			Typeface type = GetTypefaceByDocumentType(documentType);
			return GetFontInfoByTypeface(type);
		}

		const FontInfo& GetBoothFontInfo()
		{
			return g_BoothNumber;
		}

	}  // namesapce ocr
}  // namespace paplease
