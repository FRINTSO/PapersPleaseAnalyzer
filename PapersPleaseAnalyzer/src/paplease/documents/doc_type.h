#pragma once
#include "paplease/types.h"

namespace paplease {
    namespace documents {
            
        enum class DocType : u8
        {
            Invalid = static_cast<u8>(-1),
            AccessPermit = 0,
            CertificateOfVaccination,
            DiplomaticAuthorization,
            EntryPermit,
            EntryTicket,
            GrantOfAsylum,
            IdentityCard,
            IdentitySupplement,
            WorkPass,
            RuleBook,
            Bulletin,
            Transcript,
            Passport,
            Min = AccessPermit,
            Max = Passport,
            Count = Max - Min + 1,
        };

        enum class PassportType : u8
        {
            Invalid = static_cast<uint8_t>(-1),
            Antegria = static_cast<u8>(DocType::Passport),
            Arstotzka,
            Impor,
            Kolechia,
            Obristan,
            Republia,
            UnitedFederation,
            Min = Antegria,
            Max = UnitedFederation,
            Count = Max - Min + 1,
        };

        enum class AppearanceType : u8
        {
            Invalid = static_cast<uint8_t>(-1),
            AccessPermit = 0,
            CertificateOfVaccination,
            DiplomaticAuthorization,
            EntryPermit,
            EntryTicket,
            GrantOfAsylum,
            IdentityCard,
            IdentitySupplement,
            WorkPass,
            RuleBook,
            Bulletin,
            Transcript = static_cast<u8>(DocType::Transcript),
            Passport_Antegria = static_cast<u8>(DocType::Passport),
            Passport_Arstotzka,
            Passport_Impor,
            Passport_Kolechia,
            Passport_Obristan,
            Passport_Republia,
            Passport_UnitedFederation,
            Min = AccessPermit,
            Max = Passport_UnitedFederation,
            Count = Max - Min + 1,
        };

        constexpr DocType ToDocType(AppearanceType appearanceType)
        {
            if (appearanceType >= AppearanceType::Passport_Antegria && appearanceType <= AppearanceType::Passport_UnitedFederation)
            {
                return DocType::Passport;
            }
            return static_cast<DocType>(appearanceType);
        }

        constexpr PassportType ToPassportType(AppearanceType appearanceType)
        {
            if (appearanceType >= AppearanceType::Passport_Antegria && appearanceType <= AppearanceType::Passport_UnitedFederation)
            {
                return static_cast<PassportType>(appearanceType);
            }
            return PassportType::Invalid;
        }

        constexpr AppearanceType ToAppearanceType(DocType documentType, PassportType passportType)
        {
            if (documentType != DocType::Passport) return static_cast<AppearanceType>(documentType);
            return static_cast<AppearanceType>(passportType);
        }

    }  // namespace documents
}  // namespace paplease

#include "paplease/documents/doc_type.inl.h"