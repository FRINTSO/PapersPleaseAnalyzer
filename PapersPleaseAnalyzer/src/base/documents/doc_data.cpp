#include "pch.h"
#include "base/documents/doc_data.h"

#include <array>
#include <cctype>
#include <climits>
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
                        // std::cerr << message << "\n";
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

                static inline Data ProcessDistrict(StrScanner& scanner)
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
                    std::string district = oss.str();

                    const std::string suffix = " DISTRICT";
                    if (district.size() >= suffix.size() &&
                        district.compare(district.size() - suffix.size(), suffix.size(), suffix) == 0)
                    {
                        return Data{ district.substr(0, district.size() - suffix.size()) };
                    }
                    return Data{ std::move(district) };
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

                    return Data{ data::Date{day, month, year}};
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

                    return Data{ data::SIUnitValue{ value, unit } };

                }

                static inline Data ProcessVaccine(StrScanner& scanner)
                {
                    // Skip whitespace, if empty, then allow it, but return empty
                    scanner.SkipWhitespace();
                    if (scanner.IsAtEnd())
                    {
                        return Data{ data::Vaccine(), DataType::Vaccine, true };
                    }

                    // Normal case
                    auto date_data = ProcessDate(scanner);
                    if (date_data.Type() == DataType::Invalid) return err::FailedVaccineProcess();

                    scanner.SkipWhitespace();

                    scanner.StartMatch();

                    while (!scanner.IsAtEnd() && !std::isspace(scanner.Peek())) scanner.Advance();

                    auto name = scanner.MatchToStr();

                    return Data{ data::Vaccine{ date_data.Get<data::Date>(), name } };
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
                    return Data{ std::move(strList) };
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

            static inline Data ProcessTextData(const Data& data, const FieldCategory category)
            {
                StrScanner scanner{ data.Get<std::string>() };

                switch (category)
                { // sort by more clear type first to more vague types last
                    case FieldCategory::Height:
                    case FieldCategory::Weight:
                    {
                        return processing::ProcessSIUnit(scanner);
                    }
                    case FieldCategory::Sex:
                    {
                        return processing::ProcessSex(scanner);
                    }
                    case FieldCategory::CountryList:
                    {
                        return processing::ProcessStrList(scanner);
                    }
                    case FieldCategory::BoothDate:
                    case FieldCategory::ValidDate:
                    case FieldCategory::ExpirationDate:
                    case FieldCategory::DateOfBirth:
                    case FieldCategory::EndDate:
                    {
                        return processing::ProcessDate(scanner);
                    }
                    case FieldCategory::FingerPrints:
                        break;
                    case FieldCategory::Name:
                    {
                        return processing::ProcessName(scanner);
                    }
                    case FieldCategory::Field:
                    case FieldCategory::IssuingCity:
                    case FieldCategory::PhysicalAppearance:
                    case FieldCategory::DurationOfStay:
                    case FieldCategory::Purpose:
                    case FieldCategory::PassportNumber:
                    {
                        return processing::ProcessGenericString(scanner);
                    }
                    case FieldCategory::District:
                    {
                        return processing::ProcessDistrict(scanner);
                    }
                    case FieldCategory::IssuingCountry:
                    {
                        return processing::ProcessCountry(scanner);
                    }
                    case FieldCategory::Vaccination1:
                    case FieldCategory::Vaccination2:
                    case FieldCategory::Vaccination3:
                    {
                        return processing::ProcessVaccine(scanner);
                    }
                    case FieldCategory::BoothCounter:
                        return processing::ProcessGenericInt(scanner);
                    case FieldCategory::Rule1:
                    case FieldCategory::Rule2:
                    case FieldCategory::Rule3:
                    case FieldCategory::Rule4:
                    case FieldCategory::Rule5:
                    case FieldCategory::Rule6:
                    case FieldCategory::Rule7:
                    case FieldCategory::Rule8:
                    case FieldCategory::Rule9:
                    case FieldCategory::Rule10:
                        return processing::ProcessRule(scanner);
                    default:
                        return { data };
                }

                std::cout << "IMPLEMENT ME!!! " << FieldCategoryAsString(category) << " : " << scanner.Start() << "\n";

                return processing::ProcessGenericString(scanner);
            }

            static inline Data ProcessImageData(const Data& data, const FieldCategory category)
            {
                return data;
            }

            static inline Data ProcessFieldData(const Data& data, const FieldType type, const FieldCategory category)
            {
                switch (type)
                {
                    case FieldType::Text:
                        return ProcessTextData(data, category);
                    case FieldType::Image:
                        return ProcessImageData(data, category);
                    case FieldType::Invalid:
                    default:
                        return {};
                }
            }

        }  // namespace utils

#pragma region Data

#pragma region Constructors
        template<typename T>
        Data::Data(T&& data, DataType type, bool isBroken)
            : m_data{ std::forward<T>(data) }, m_type{ type }, m_isBroken{ isBroken }
        {}

        Data::Data(std::string&& data)
            : Data{ std::move(data), DataType::GenericString }
        {}

        Data::Data(int data)
            : Data{ data, DataType::GenericNumber }
        {}

        Data::Data(data::Date&& data) : Data{ std::move(data), DataType::Date }
        {}

        Data::Data(data::SIUnitValue&& data)
            : Data{ std::move(data), DataType::SIUnit }
        {}

        Data::Data(data::Vaccine&& data)
            : Data{ std::move(data), DataType::Vaccine }
        {}

        Data::Data(data::StrList&& data)
            : Data{ std::move(data), DataType::StrList }
        {}

        Data::Data(data::Sex data)
            : Data{ data, DataType::Sex }
        {}

        Data::Data(data::Photo&& data)
            : Data{ std::move(data), DataType::Image}
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

        bool Data::operator==(const Data& other) const
        {
            if (m_type != other.m_type || m_isBroken != other.m_isBroken)
            {
                return false;  // Early exit if types or broken flag don't match
            }

            // Compare the variant data based on its type
            switch (m_type)
            {
                case paplease::documents::DataType::GenericString:
                    return std::get<std::string>(m_data) == std::get<std::string>(other.m_data);
                case paplease::documents::DataType::GenericNumber:
                    return std::get<int>(m_data) == std::get<int>(other.m_data);
                case paplease::documents::DataType::StrList:
                    return std::get<data::StrList>(m_data) == std::get<data::StrList>(other.m_data);
                case paplease::documents::DataType::Date:
                    return std::get<data::Date>(m_data) == std::get<data::Date>(other.m_data);
                case paplease::documents::DataType::SIUnit:
                    return std::get<data::SIUnitValue>(m_data) == std::get<data::SIUnitValue>(other.m_data);
                case paplease::documents::DataType::Vaccine:
                    return std::get<data::Vaccine>(m_data) == std::get<data::Vaccine>(other.m_data);
                case paplease::documents::DataType::Sex:
                    return std::get<data::Sex>(m_data) == std::get<data::Sex>(other.m_data);
                default:
                    return false;
            }
        }

#pragma endregion

#pragma region FieldData

        Field::Field(const Data& data, const FieldType type, const FieldCategory category)
            : m_data{ data }, m_fieldType{ type }, m_fieldCategory{ category }, m_fieldState{ FieldState::Initialized }
        {}

        const Data& Field::GetData() const
        {
            return m_data;
        }

        std::string Field::ToString() const
        {
            return m_data.ToText();
        }

        FieldType Field::Type() const
        {
            return m_fieldType;
        }

        FieldCategory Field::Category() const
        {
            return m_fieldCategory;
        }

        bool Field::IsBroken() const
        {
            return m_data.IsBroken();
        }

#pragma endregion

#pragma region DocData

        std::optional<std::reference_wrapper<const Field>> DocData::GetField(FieldCategory category, bool returnBroken) const
        {
            if (category == FieldCategory::Invalid || category == FieldCategory::Count)
                return std::nullopt;

            size_t index = static_cast<size_t>(category);
            
            const auto& fieldOpt = m_data[index];
            if (!fieldOpt)
            {
                // paplease::LOG_ERR("Field missing for category '{}'. Document may be incomplete or corrupted.", FieldCategoryAsString(category));
                return std::nullopt;
            }

            const auto& field = fieldOpt.value();
            if (field.IsBroken())
            {
                // paplease::LOG_ERR("Broken data in field '{}'. Document may be corrupted or have invalid values.", FieldCategoryAsString(category));

                if (!returnBroken)
                    return std::nullopt;
            }

            return std::cref(field);
        }

        paplease::utils::FixedRefArray<Field, DocData::ArrayLength> DocData::GetAllValidFields() const
        {
            paplease::utils::FixedRefArray<Field, DocData::ArrayLength> fieldsArray{};
            
            for (const auto& dataOpt : m_data)
            {
                if (!dataOpt)
                    continue;

                const auto& data = dataOpt.value();

                if (data.Type() != FieldType::Invalid)
                {
                    fieldsArray.Add(data);
                }
            }

            return fieldsArray;
        }

        bool DocData::HasBrokenData() const
        {
            for (const auto& field : GetAllValidFields())
            {
                if (field->get().IsBroken())
                {
                    return true;
                }
            }

            return false;
        }

        void DocData::PrintAllFields() const // temporary for ease of development
        {
            for (const auto& dataOpt : m_data)
            {
                if (!dataOpt)
                    continue;

                const auto& data = dataOpt.value();
                if (data.Type() != FieldType::Text) continue;
                std::cout << data.ToString() << "\n";
            }
        }

        DocData::DocData(const std::array<Field, ArrayLength>& data)
        {
            for(size_t i = 0; i < ArrayLength; ++i)
                m_data[i] = std::move(data[i]);
        }

#pragma endregion

#pragma region DocDataBuilder

        bool DocDataBuilder::AddFieldData(const FieldCategory category, Field&& data)
        {
            if (category == FieldCategory::Invalid || category == FieldCategory::Count) return false;

            //size_t index = static_cast<size_t>(category) - 1;
            size_t index = static_cast<size_t>(category);

            if (m_data[index].m_fieldState == FieldState::Empty)
            {
                m_data[index] = data;
                return true;
            }

            return false;
        }

        DocData DocDataBuilder::GetDocData()
        {
            for (auto& fieldData : m_data)
            {
                if (fieldData.m_fieldState != FieldState::Empty)
                {
                    fieldData.m_data = utils::ProcessFieldData(fieldData.m_data, fieldData.m_fieldType, fieldData.m_fieldCategory);
                    fieldData.m_fieldState = FieldState::ProcessedData;

                    if (fieldData.IsBroken())
                    {
                        fieldData.m_fieldState = FieldState::BrokenData;
                        // THIS CODE UNDER, IF TOGGLED WILL DISABLE BROKEN DATA FROM BEING ACCEPTED. SO DOCUMENTS WITH MISSING BROKEN DATA WILL BE SKIPPED
                        //this->Clear();
                        //return std::nullopt;
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
                m_data[i] = Field{};
            }
        }

#pragma endregion

    }  // namespace documents
}  // namespace paplease
