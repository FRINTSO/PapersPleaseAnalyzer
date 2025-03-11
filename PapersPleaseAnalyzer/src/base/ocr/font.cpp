#include "pch.h"
#include "base/ocr/font.h"

#include <cassert>
#include <iostream>

#include "base/common.h"
#include "base/documents_v2/doc_type.h"

namespace paplease {
	namespace ocr {

		static inline constinit FontInfo g_Invalid{ Typeface::Invalid, 0, -1, -1 };
		static inline constinit FontInfo g_04b04{ Typeface::_04b03, DOWNSCALE(14), DOWNSCALE(2), DOWNSCALE(4) /* Guess 3*/, DOWNSCALE(8), DOWNSCALE(4) };
		static inline constinit FontInfo g_BM_MiniInfo{ Typeface::BM_Mini, DOWNSCALE(16), DOWNSCALE(2), DOWNSCALE(4), DOWNSCALE(4), DOWNSCALE(8) };
		static inline constinit FontInfo g_MiniKylie{ Typeface::MiniKylie, DOWNSCALE(12), DOWNSCALE(4), DOWNSCALE(6), DOWNSCALE(8), DOWNSCALE(4) };
		static inline constinit FontInfo g_BoothNumber{ Typeface::BoothNumber, DOWNSCALE(12), DOWNSCALE(4), DOWNSCALE(6) /* Guess 3 */, DOWNSCALE(8), DOWNSCALE(4) };

		static constexpr Typeface GetTypefaceByDocumentType(const documents::v2::DocType documentType)
		{
			switch (documentType)
			{
				case documents::v2::DocType::Passport:
					return Typeface::BM_Mini;
				case documents::v2::DocType::IdentityCard:
					return Typeface::MiniKylie;
				case documents::v2::DocType::DiplomaticAuthorization:
					return Typeface::BM_Mini;
				case documents::v2::DocType::EntryTicket:
					return Typeface::BM_Mini;
				case documents::v2::DocType::EntryPermit:
					return Typeface::BM_Mini;
				case documents::v2::DocType::AccessPermit:
					return Typeface::BM_Mini;
				case documents::v2::DocType::WorkPass:
					return Typeface::BM_Mini;
				case documents::v2::DocType::GrantOfAsylum:
					return Typeface::BM_Mini;
				case documents::v2::DocType::IdentitySupplement:
					return Typeface::BM_Mini;
				case documents::v2::DocType::CertificateOfVaccination:
					return Typeface::BM_Mini;
				case documents::v2::DocType::RuleBook:
					return Typeface::BM_Mini;
				case documents::v2::DocType::Transcript:
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

		const FontInfo& GetFontInfo(const documents::v2::DocType documentType)
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
