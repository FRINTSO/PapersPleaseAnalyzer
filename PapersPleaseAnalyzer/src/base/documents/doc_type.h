#pragma once
#include <array>
#include <string_view>

#include "base/common.h"

namespace paplease {
    namespace documents {
            
        enum class DocType : unsigned int
        {
            Invalid = -1,
            AccessPermit,
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
            Count,
        };

        enum class PassportType : unsigned int
        {
            Invalid = -1,
            Antegria = static_cast<int>(DocType::Passport),
            Arstotzka,
            Impor,
            Kolechia,
            Obristan,
            Republia,
            UnitedFederation,
            Count = 7,
        };

        enum class AppearanceType : unsigned int
        {
            Invalid = -1,
            AccessPermit,
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
            Transcript = static_cast<int>(DocType::Transcript),
            Passport_Antegria = static_cast<int>(DocType::Passport),
            Passport_Arstotzka,
            Passport_Impor,
            Passport_Kolechia,
            Passport_Obristan,
            Passport_Republia,
            Passport_UnitedFederation,
            Count,
        };

#if USE_ENUM_FUNCS

#define DOC_TYPE_NAME_INVALID                    std::string_view("Invalid", 7)
#define DOC_TYPE_NAME_ACCESS_PERMIT              std::string_view("AccessPermit", 12)
#define DOC_TYPE_NAME_CERTIFICATE_OF_VACCINATION std::string_view("CertificateOfVaccination", 24)
#define DOC_TYPE_NAME_DIPLOMATIC_AUTHORIZATION   std::string_view("DiplomaticAuthorization", 23)
#define DOC_TYPE_NAME_ENTRY_PERMIT               std::string_view("EntryPermit", 11)
#define DOC_TYPE_NAME_ENTRY_TICKET               std::string_view("EntryTicket", 11)
#define DOC_TYPE_NAME_GRANT_OF_ASYLUM            std::string_view("GrantOfAsylum", 13)
#define DOC_TYPE_NAME_IDENTITY_CARD              std::string_view("IdentityCard", 12)
#define DOC_TYPE_NAME_IDENTITY_SUPPLEMENT        std::string_view("IdentitySupplement", 18)
#define DOC_TYPE_NAME_WORK_PASS                  std::string_view("WorkPass", 8)
#define DOC_TYPE_NAME_RULE_BOOK                  std::string_view("RuleBook", 8)
#define DOC_TYPE_NAME_BULLETIN                   std::string_view("Bulletin", 8)
#define DOC_TYPE_NAME_TRANSCRIPT                 std::string_view("Transcript", 10)
#define DOC_TYPE_NAME_PASSPORT                   std::string_view("Passport", 8)

#if !defined(DOC_TYPE_NAMES)
#define DOC_TYPE_NAMES                                \
    {                                             \
        DOC_TYPE_NAME_ACCESS_PERMIT,              \
        DOC_TYPE_NAME_CERTIFICATE_OF_VACCINATION, \
        DOC_TYPE_NAME_DIPLOMATIC_AUTHORIZATION,   \
        DOC_TYPE_NAME_ENTRY_PERMIT,               \
        DOC_TYPE_NAME_ENTRY_TICKET,               \
        DOC_TYPE_NAME_GRANT_OF_ASYLUM,            \
        DOC_TYPE_NAME_IDENTITY_CARD,              \
        DOC_TYPE_NAME_IDENTITY_SUPPLEMENT,        \
        DOC_TYPE_NAME_WORK_PASS,                  \
        DOC_TYPE_NAME_RULE_BOOK,                  \
        DOC_TYPE_NAME_BULLETIN,                   \
        DOC_TYPE_NAME_TRANSCRIPT,                 \
        DOC_TYPE_NAME_PASSPORT,                   \
        DOC_TYPE_NAME_INVALID,                    \
    }
#endif

        static constexpr std::string_view& ToStringView(DocType documentType);
        static constexpr bool IsApplicantDocument(DocType documentType);
        static consteval std::array<DocType, static_cast<size_t>(DocType::Count)> GetDocTypeIterator();
#endif

    }  // namespace documents
}  // namespace paplease

#include "base/documents/doc_type.inl.h"