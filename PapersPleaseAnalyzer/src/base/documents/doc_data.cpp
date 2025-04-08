#include "pch.h"
#include "base/documents/doc_data.h"

#include <array>
#include <cctype>
#include <climits>
#include <intrin.h>
#include <iosfwd>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_set>

#include "base/documents/data/date.h"
#include "base/documents/data/field_data.h"
#include "base/documents/doc_data_type.h"
#include "base/documents/utils/str_scanner.h"
#include "base/utils/log.h"
#include "base/utils/strfuncs.h"

namespace paplease {
    namespace documents {
        namespace utils {

            namespace processing {
                namespace err {

                    static inline Data FailedProcess(const std::string& message)
                    {
                        std::cerr << message << "\n";
                        return Data{ "", DataType::Invalid, true };
                    }

                    static inline Data FailedGenericIntProcess()
                    {
                        return FailedProcess("INVALID INTEGER");
                    }

                    static inline Data FailedNameProcess()
                    {
                        return FailedProcess("INVALID NAME");
                    }

                    static inline Data FailedDateProcess()
                    {
                        return FailedProcess("INVALID DATE");
                    }

                    static inline Data FailedSIUnitProcess()
                    {
                        return FailedProcess("INVALID SI UNIT");
                    }

                    static inline Data FailedVaccineProcess()
                    {
                        return FailedProcess("INVALID VACCINE");
                    }

                    static inline Data FailedRuleProcess()
                    {
                        return FailedProcess("INVALID RULE");
                    }

                }  // namespace err

                static inline Data ProcessGenericString(StrScanner& scanner)
                {
                    scanner.SkipWhitespace();

                    std::ostringstream oss;

                    scanner.StartMatch();
                    while (!scanner.IsAtEnd() && scanner.Peek() != ' ') scanner.Advance();
                    oss << scanner.MatchToStr();
                    while (!scanner.IsAtEnd())
                    {
                        scanner.SkipWhitespace();
                        scanner.StartMatch();
                        while (!scanner.IsAtEnd() && scanner.Peek() != ' ') scanner.Advance();
                        oss << " " << scanner.MatchToStr();
                    }
                    return Data{ oss.str() };
                }

                static inline Data ProcessCountry(StrScanner& scanner)
                {
                    auto data = ProcessGenericString(scanner);
                    auto str = data.ToText();
                    if (str == "UNITEDFED")
                    {
                        return Data{ "United Federation" };
                    }
                    return data;
                }

                static inline Data ProcessGenericInt(StrScanner& scanner)
                {
                    scanner.SkipWhitespace();
                    scanner.StartMatch();

                    while (!scanner.IsAtEnd() && scanner.MatchDigits(1, StrScanner::MatchRemaining)) scanner.Advance();
                    int number = scanner.MatchToInt();

                    scanner.SkipWhitespace();
                    if (!scanner.IsAtEnd()) return err::FailedGenericIntProcess();

                    return Data{ number };
                }

                static inline Data ProcessName(StrScanner& scanner)
                {
                    // LASTNAME , FIRSTNAME or FIRSTNAME ' ' LASTNAME

                    scanner.SkipWhitespace();

                    scanner.StartMatch();
                    if (!scanner.MatchAlnumSequence()) return err::FailedNameProcess();
                    std::string name1 = scanner.MatchToStr();

                    if (scanner.Match(','))
                    {
                        scanner.SkipWhitespace();
                        scanner.StartMatch();
                        if (!scanner.MatchAlnumSequence()) return err::FailedNameProcess();
                        std::string name2 = scanner.MatchToStr();
                        return Data{ paplease::utils::strfuncs::Capitalize(name2) + " " + paplease::utils::strfuncs::Capitalize(name1) };
                    }
                    else if (scanner.Match(' '))
                    {
                        scanner.SkipWhitespace();
                        scanner.StartMatch();
                        if (!scanner.MatchAlnumSequence()) return err::FailedNameProcess();
                        std::string name2 = scanner.MatchToStr();
                        return Data{ paplease::utils::strfuncs::Capitalize(name1) + " " + paplease::utils::strfuncs::Capitalize(name2) };
                    }
                    else
                    {
                        return err::FailedNameProcess();
                    }
                }

                static inline Data ProcessDate(StrScanner& scanner)
                {
                    // dateData \s*\d{2}\.\d{2}\.\d{4}\s* // this or
                    // // dateData \s*\d{2}\.\d{2}\.\d{2}\s* // this
                    // day.month.year
                    // year is either 2 or 4 digits

                    data::Date date;

                    scanner.SkipWhitespace();

                    // Match 2 digits - Day
                    scanner.StartMatch();
                    if (!scanner.MatchDigits(2)) return err::FailedDateProcess();
                    int day = scanner.MatchToInt();

                    // Match dot
                    if (!scanner.Match('.')) return err::FailedDateProcess();

                    // Match 2 digits - Month
                    scanner.StartMatch();
                    if (!scanner.MatchDigits(2)) return err::FailedDateProcess();
                    int month = scanner.MatchToInt();

                    // Match dot
                    scanner.Match('.');

                    // Match 2-4 digits - Year
                    scanner.StartMatch();
                    if (!scanner.MatchDigits(2, 4)) return err::FailedDateProcess();
                    auto year_digits = scanner.MatchLength();
                    int year = scanner.MatchToInt();

                    if (year_digits == 2)
                    {
                        year += 1900;
                    }

                    data::Date dateData{ day, month, year };
                    return Data{ dateData };
                }

                static inline Data ProcessSIUnit(StrScanner& scanner)
                {
                    // \d+cm
                    // \d+kg
                    scanner.SkipWhitespace();

                    // digits
                    scanner.StartMatch();
                    if (!scanner.MatchDigits(0, INT_MAX)) return err::FailedSIUnitProcess();
                    int value = scanner.MatchToInt();

                    data::SIUnit unit;

                    // SI UNIT
                    if (scanner.MatchStr("cm"))
                    {
                        unit = data::SIUnit::CM;
                    }
                    else if (scanner.MatchStr("kg"))
                    {
                        unit = data::SIUnit::KG;
                    }
                    else
                    {
                        return err::FailedSIUnitProcess();
                    }

                    data::SIUnitValue siUnitData{ value, unit };
                    return Data{ siUnitData };

                }

                static inline Data ProcessVaccine(StrScanner& scanner)
                {
                    // Skip whitespace, if empty, then allow it, but return empty
                    scanner.SkipWhitespace();
                    if (scanner.IsAtEnd())
                    {
                        return Data{ "" };
                    }

                    // Normal case
                    auto date_data = ProcessDate(scanner);
                    if (date_data.Type() == DataType::Invalid) return err::FailedVaccineProcess();

                    scanner.SkipWhitespace();

                    scanner.StartMatch();

                    while (!scanner.IsAtEnd() && !std::isspace(scanner.Peek())) scanner.Advance();

                    auto name = scanner.MatchToStr();

                    data::Vaccine vaccineData{ date_data.Get<data::Date>(), name };

                    return Data{ vaccineData };
                }

                static inline Data ProcessStrList(StrScanner& scanner)
                {
                    data::StrList strList{};

                    do
                    {
                        scanner.SkipWhitespace();
                        scanner.StartMatch();
                        while (!scanner.IsAtEnd() && scanner.Peek() != ',') scanner.Advance();
                        strList.strs.push_back(scanner.MatchToStr());

                    } while (scanner.Match(','));
                    return Data{ strList };
                }

                static inline Data ProcessSex(StrScanner& scanner)
                {
                    scanner.SkipWhitespace();

                    scanner.StartMatch();
                    if (scanner.Match('F'))
                    {
                        return Data{ data::Sex::Female };
                    }
                    else if (scanner.Match('M'))
                    {
                        return Data{ data::Sex::Male };
                    }
                    else
                    {
                        return err::FailedProcess("INVALID SEX");
                    }
                }

                static inline Data ProcessRule(StrScanner& scanner)
                {
                    static const std::unordered_set<std::string> rules{
                        "all documents must be current",
                        "all kolechians must be searched",
                        "arstotzkan citizens must have an id card",
                        "arstotzkan citizens only",
                        "asylum seekers must have a grant",
                        "citizens must have an id card",
                        "confiscate all arstotzkan passports",
                        "confiscate arstotzkan passports belonging to altan residents",
                        "diplomats require authorization",
                        "entrant must have a passport",
                        "entrant must have polio vaccine cert",
                        "foreigners require an access permit",
                        "foreigners require an entry permit",
                        "foreigners require an entry ticket",
                        "foreigners require an id supplement",
                        "no entry from United Federation",
                        "no entry from impor",
                        "no weapons or contraband",
                        "workers must have a work pass",
                        ""
                    };


                    auto data = ProcessGenericString(scanner);
                    const auto& text = paplease::utils::strfuncs::ToLower(data.Get<std::string>());

                    if (rules.find(text) == rules.end())
                    {
                        ::paplease::LOG_ERR("Failed Rule: {}", text);
                        return err::FailedRuleProcess();
                    }

                    return data;
                }

            }  // namespace processing

            static inline Data ProcessTextData(const Data& data, const DataFieldCategory category)
            {
                StrScanner scanner{ data.Get<std::string>() };

                switch (category)
                { // sort by more clear type first to more vague types last
                    case DataFieldCategory::Height:
                    case DataFieldCategory::Weight:
                    {
                        return processing::ProcessSIUnit(scanner);
                    }
                    case DataFieldCategory::Sex:
                    {
                        return processing::ProcessSex(scanner);
                    }
                    case DataFieldCategory::CountryList:
                    {
                        return processing::ProcessStrList(scanner);
                    }
                    case DataFieldCategory::BoothDate:
                    case DataFieldCategory::ValidDate:
                    case DataFieldCategory::ExpirationDate:
                    case DataFieldCategory::DateOfBirth:
                    case DataFieldCategory::EndDate:
                    {
                        return processing::ProcessDate(scanner);
                    }
                    //case DataFieldCategory::Duration:
                    case DataFieldCategory::FingerPrints:
                        break;
                    case DataFieldCategory::Name:
                    {
                        return processing::ProcessName(scanner);
                    }
                    case DataFieldCategory::Field:
                    case DataFieldCategory::IssuingCity:
                    // case DataFieldCategory::Description:
                    case DataFieldCategory::PhysicalAppearance:
                    case DataFieldCategory::District:
                    //case DataFieldCategory::Nationality:
                    case DataFieldCategory::DurationOfStay:
                    case DataFieldCategory::Purpose:
                    case DataFieldCategory::PassportNumber:
                    {
                        return processing::ProcessGenericString(scanner);
                    }
                    case DataFieldCategory::IssuingCountry:
                    {
                        return processing::ProcessCountry(scanner);
                    }
                    case DataFieldCategory::Vaccination1:
                    case DataFieldCategory::Vaccination2:
                    case DataFieldCategory::Vaccination3:
                    {
                        return processing::ProcessVaccine(scanner);
                    }
                    case DataFieldCategory::BoothCounter:
                        return processing::ProcessGenericInt(scanner);
                    case DataFieldCategory::Rule1:
                    case DataFieldCategory::Rule2:
                    case DataFieldCategory::Rule3:
                    case DataFieldCategory::Rule4:
                    case DataFieldCategory::Rule5:
                    case DataFieldCategory::Rule6:
                    case DataFieldCategory::Rule7:
                    case DataFieldCategory::Rule8:
                    case DataFieldCategory::Rule9:
                    case DataFieldCategory::Rule10:
                        return processing::ProcessRule(scanner);
                    default:
                        return { data };
                }

                std::cout << "IMPLEMENT ME!!! " << DataFieldCategoryAsString(category) << " : " << scanner.Start() << "\n";

                return processing::ProcessGenericString(scanner);
            }

            static inline Data ProcessFieldData(const Data& data, const FieldType type, const DataFieldCategory category)
            {
                switch (type)
                {
                    case FieldType::Text:
                        return ProcessTextData(data, category);
                    case FieldType::Image:
                        // Not implemented
                    case FieldType::Invalid:
                    default:
                        return {};
                }
            }

        }  // namespace utils

#pragma region Data

#pragma region Constructors
        template<typename T>
        Data::Data(const T& data, DataType type, bool isBroken)
            : m_data{ data }, m_type{ type }, m_isBroken{ isBroken }
        {}

        Data::Data(const std::string& data)
            : Data{ data, DataType::GenericString }
        {}

        Data::Data(int data)
            : Data{ data, DataType::GenericNumber }
        {}

        Data::Data(const data::Date& data) : Data{ data, DataType::Date }
        {}

        Data::Data(const data::SIUnitValue& data)
            : Data{ data, DataType::SIUnit }
        {}

        Data::Data(const data::Vaccine& data)
            : Data{ data, DataType::Vaccine }
        {}

        Data::Data(const data::StrList& data)
            : Data{ data, DataType::StrList }
        {}

        Data::Data(data::Sex data)
            : Data{ data, DataType::Sex }
        {}

#pragma endregion

        std::string Data::ToText() const
        {
            switch (m_type)
            {
                case DataType::Invalid:
                    return "INVALID DATA";
                case DataType::GenericString:
                    return this->Get<std::string>();
                case DataType::GenericNumber:
                    return std::to_string(this->Get<int>());
                case DataType::Date:
                {
                    const data::Date& date = this->Get<data::Date>();
                    std::ostringstream oss;
                    oss << date.GetDay() << "."
                        << date.GetMonth() << "."
                        << date.GetYear();
                    return oss.str();
                }
                case DataType::SIUnit:
                {
                    const data::SIUnitValue& siUnitValue = this->Get<data::SIUnitValue>();
                    std::ostringstream oss;
                    oss << siUnitValue.value;

                    switch (siUnitValue.unit)
                    {
                        case data::SIUnit::KG:
                            oss << " kg";
                            break;
                        case data::SIUnit::CM:
                            oss << " cm";
                            break;
                    }

                    return oss.str();
                }
                case DataType::Vaccine:
                {
                    const auto& vaccine = this->Get<data::Vaccine>();

                    std::ostringstream oss;
                    const auto& date = vaccine.date;
                    oss << date.GetDay() << "."
                        << date.GetMonth() << "."
                        << date.GetYear();
                    oss << "\t"
                        << vaccine.name;
                    return oss.str();
                }
                case DataType::StrList:
                {
                    const auto& strList = this->Get<data::StrList>();
                    std::ostringstream oss;
                    int i;
                    for (i = 0; i < strList.strs.size() - 1; i++)
                    {
                        oss << strList.strs[i] << ", ";
                    }
                    oss << strList.strs[i];
                    return oss.str();
                }
                case DataType::Sex:
                {
                    const auto sex = this->Get<data::Sex>();
                    if (sex == data::Sex::Female)
                    {
                        return "Female";
                    }
                    else
                    {
                        return "Male";
                    }
                }
                case DataType::Image:
                    return "Image cannot be represented as text!";
                default:
                    return "ERROR!";
            }
        }

        DataType Data::Type() const
        {
            return m_type;
        }

        bool Data::IsBroken() const
        {
            return m_isBroken;
        }

#pragma endregion

#pragma region FieldData

        FieldData::FieldData(const Data& data, const FieldType type, const DataFieldCategory category)
            : m_data{ data }, m_fieldType{ type }, m_fieldCategory{ category }, m_fieldState{ DataFieldState::Initialized }
        {}

        const Data& FieldData::GetData() const
        {
            return m_data;
        }

        std::string FieldData::ToText() const
        {
            return m_data.ToText();
        }

        FieldType FieldData::Type() const
        {
            return m_fieldType;
        }

        DataFieldCategory FieldData::Category() const
        {
            return m_fieldCategory;
        }

        bool FieldData::IsBroken() const
        {
            return m_data.IsBroken();
        }

#pragma endregion

#pragma region DocData

        const FieldData& DocData::GetField(DataFieldCategory category) const
        {
            if (category == DataFieldCategory::Invalid) return {};
            size_t index = static_cast<size_t>(category) - 1;

            if (m_data[index].IsBroken())
            {
                paplease::LOG_ERR("Broken data");
                __debugbreak();
                return {};
            }

            return m_data[index];
        }

        paplease::utils::FixedRefArray<FieldData, DocData::ArrayLength> DocData::GetAllValidFields() const
        {
            paplease::utils::FixedRefArray<FieldData, DocData::ArrayLength> fieldsArray{};
            
            for (const auto& data : m_data)
            {
                if (data.Type() != FieldType::Invalid)
                {
                    fieldsArray.Add(data);
                }
            }

            return fieldsArray;
        }

        DocData::DocData(const std::array<FieldData, ArrayLength>& data)
            : m_data{ data }
        {}

#pragma endregion

#pragma region DocDataBuilder

        bool DocDataBuilder::AddFieldData(const DataFieldCategory category, FieldData&& data)
        {
            if (category == DataFieldCategory::Invalid) return false;

            size_t index = static_cast<size_t>(category) - 1; // DataFieldCategory::Invalid is not a field

            if (m_data[index].m_fieldState == DataFieldState::Empty)
            {
                m_data[index] = data;
                return true;
            }

            return false;
        }

        std::optional<DocData> DocDataBuilder::GetDocData()
        {
            for (auto& fieldData : m_data)
            {
                if (fieldData.m_fieldState != DataFieldState::Empty)
                {
                    fieldData.m_data = utils::ProcessFieldData(fieldData.m_data, fieldData.m_fieldType, fieldData.m_fieldCategory);
                    fieldData.m_fieldState = DataFieldState::ProcessedData;

                    if (fieldData.IsBroken())
                    {
                        this->Clear();
                        return std::nullopt;
                    }
                }
            }
            DocData docData{ m_data };
            this->Clear();
            return docData;
        }

        void DocDataBuilder::Clear()
        {
            for (size_t i = 0; i < m_data.size(); i++)
            {
                m_data[i] = FieldData{};
            }
        }

#pragma endregion

    }  // namespace documents
}  // namespace paplease
