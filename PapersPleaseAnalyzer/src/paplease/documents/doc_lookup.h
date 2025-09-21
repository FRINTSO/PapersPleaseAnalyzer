#pragma once
#include "paplease/documents/doc_type.h"
#include "paplease/types.h"

#include <string>

namespace paplease {
    namespace documents {

        struct DocLookup
        {
            DocType documentType;
            PassportType passportType = PassportType::Invalid;

            constexpr DocLookup()
                : documentType(DocType::Invalid), passportType(PassportType::Invalid)
            {}
            constexpr DocLookup(DocType documentType)
                : documentType(documentType)
            {}
            constexpr DocLookup(DocType documentType, PassportType passportType)
                : documentType(documentType), passportType(passportType)
            {}
            constexpr DocLookup(AppearanceType appearanceType)
                : documentType(ToDocType(appearanceType)), passportType(ToPassportType(appearanceType))
            {}

            constexpr bool IsValid() const noexcept { return documentType != DocType::Invalid; }
            constexpr bool IsPassport() const noexcept { return documentType == DocType::Passport; }
            constexpr bool AppliesTo(const DocLookup other) const noexcept
            {
                if (this->IsPassport() && other.IsPassport())
                {
                    if (other.passportType == PassportType::Invalid)
                    {
                        return true;
                    }
                    return passportType == other.passportType;
                }
                return documentType == other.documentType;
            }
            std::string ToString() const noexcept;
        };

        constexpr bool operator==(const DocLookup& left, const DocLookup& right)
        {
            return left.documentType == right.documentType && left.passportType == right.passportType;
        }

    }  // namespace documents
}  // namespace paplease

namespace std {
    template <>
    struct hash<paplease::documents::DocLookup>
    {
        static_assert(sizeof(paplease::documents::DocLookup) == sizeof(paplease::u16));
        size_t operator()(const paplease::documents::DocLookup& key) const noexcept
        {
            const paplease::documents::DocLookup* pLookup = &key;
            paplease::u16 value = *reinterpret_cast<const paplease::u16*>(pLookup);
            return std::hash<paplease::u16>{}(value);
        }
    };
}