#include "pch.h"
#include "base/documents_v2/doc_data.h"

#include "base/document_data/date.h"

#include <array>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace Documents::V2 {

    namespace Utils
    {
        class StrScanner
        {
        public:
            StrScanner(const std::string& str)
                : m_start{ str.c_str() }, m_current{ m_start }
            {}

            constexpr inline bool IsAtEnd()
            {
                return *m_current == '\0';
            }

            constexpr char Advance()
            {
                m_current++;
                return m_current[-1];
            }

            constexpr char Peek()
            {
                return *m_current;
            }

            constexpr bool Match(char expected)
            {
                if (this->IsAtEnd()) return false;
                if (*m_current != expected) return false;
                m_current++;
                return true;
            }

            constexpr bool MatchDigits(size_t digits = 1)
            {
                assert(digits != 0);

                for (size_t i = 0; i < digits; i++)
                {
                    if (this->IsAtEnd()) return false;
                    if (!this->IsDigit(*m_current)) return false;
                    m_current++;
                }
                return true;
            }

            constexpr bool MatchDigits(const size_t minDigits, const size_t maxDigits)
            {
                assert(minDigits <= maxDigits);
                assert(minDigits != 0);

                size_t count = 0;
                
                while (count < maxDigits)
                {
                    if (this->IsAtEnd() || !this->IsDigit(*m_current))
                    {
                        break;
                    }
                    m_current++;
                    count++;
                }

                return count >= minDigits;
            }

            constexpr bool MatchStr(const std::string& str)
            {
                for (size_t i = 0; i < str.length(); i++)
                {
                    if (this->IsAtEnd()) return false;
                    if (this->Peek() != str[i]) return false;
                    m_current++;
                }
                return true;
            }

            constexpr bool MatchAlnumSequence()
            {
                size_t length = 0;
                while (!this->IsAtEnd() && std::isalnum(*m_current))
                {
                    m_current++;
                    length++;
                }

                return length != 0;
            }

            inline void SkipWhitespace()
            {
                char c;
                while (std::isspace(c = this->Peek())) this->Advance();
            }

            constexpr void StartMatch()
            {
                m_start = m_current;
            }

            inline int MatchToInt()
            {
                const char* start = m_start;
                m_start = m_current;
                return std::atoi(start);
            }

            constexpr inline size_t MatchLength() const
            {
                return (size_t)(m_current - m_start);
            }

            inline long long MatchToNumber()
            {
                const char* start = m_start;
                m_start = m_current;
                return std::atoll(start);
            }

            inline std::string MatchToStr()
            {
                const char* start = m_start;
                m_start = m_current;
                return std::string(start, (size_t)(m_current - start));
            }


            static constexpr bool IsDigit(char c)
            {
                return c >= '0' && c <= '9';
            }

            constexpr const char const* Start() const noexcept
            {
                return m_start;
            }

            constexpr const char const* Current() const noexcept
            {
                return m_current;
            }
        public:
            static constexpr auto MatchRemaining{ static_cast<size_t>(-1) };
        private:
            const char* m_start;
            const char* m_current;
        };

        namespace StringFunctions
        {
            static constexpr inline std::string Trim(const std::string& str)
            {
                size_t start = str.find_first_not_of(" \t\n\r\f\v");
                size_t end = str.find_last_not_of(" \t\n\r\f\v");

                if (start == std::string::npos) {
                    return ""; // String is all whitespace
                }

                return str.substr(start, end - start + 1);
            }

            static constexpr inline std::string Capitalize(const std::string& str)
            {
                if (str.empty()) {
                    return str;
                }

                std::string result = str;

                result[0] = std::toupper(static_cast<char>(result[0]));

                for (size_t i = 1; i < result.size(); i++)
                {
                    result[i] = std::tolower(static_cast<char>(result[i]));
                }

                return result;
            }

            
        }

        namespace DataProcessing
        {
            namespace Err
            {
                static inline Data FailedProcess(const std::string& message)
                {
                    std::cerr << message << "\n";
                    return Data{"", DataType::Invalid, true};
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
            }

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

            static inline Data ProcessGenericInt(StrScanner& scanner)
            {
                scanner.SkipWhitespace();
                scanner.StartMatch();

                while (!scanner.IsAtEnd() && scanner.MatchDigits(1, StrScanner::MatchRemaining)) scanner.Advance();
                int number = scanner.MatchToInt();

                scanner.SkipWhitespace();
                if (!scanner.IsAtEnd()) return Err::FailedGenericIntProcess();

                return Data{ number };
            }

            static inline Data ProcessName(StrScanner& scanner)
            {
                using namespace StringFunctions;

                // LASTNAME , FIRSTNAME or FIRSTNAME ' ' LASTNAME

                scanner.SkipWhitespace();

                scanner.StartMatch();
                if (!scanner.MatchAlnumSequence()) return Err::FailedNameProcess();
                std::string name1 = scanner.MatchToStr();

                if (scanner.Match(','))
                {
                    scanner.SkipWhitespace();
                    scanner.StartMatch();
                    if (!scanner.MatchAlnumSequence()) return Err::FailedNameProcess();
                    std::string name2 = scanner.MatchToStr();
                    return Data{ Capitalize(name2) + " " + Capitalize(name1) };
                }
                else if (scanner.Match(' '))
                {
                    scanner.SkipWhitespace();
                    scanner.StartMatch();
                    if (!scanner.MatchAlnumSequence()) return Err::FailedNameProcess();
                    std::string name2 = scanner.MatchToStr();
                    return Data{ Capitalize(name1) + " " + Capitalize(name2) };
                }
                else
                {
                    return Err::FailedNameProcess();
                }
            }

            static inline Data ProcessDate(StrScanner& scanner)
            {
                // dateData \s*\d{2}\.\d{2}\.\d{4}\s* // this or
                // // dateData \s*\d{2}\.\d{2}\.\d{2}\s* // this
                // day.month.year
                // yead is either 2 or 4 digits

                Documents::Data::Date date;

                scanner.SkipWhitespace();

                // Match 2 digits - Day
                scanner.StartMatch();
                if (!scanner.MatchDigits(2)) return Err::FailedDateProcess();
                int day = scanner.MatchToInt();

                // Match dot
                if (!scanner.Match('.')) return Err::FailedDateProcess();

                // Match 2 digits - Month
                scanner.StartMatch();
                if (!scanner.MatchDigits(2)) return Err::FailedDateProcess();
                int month = scanner.MatchToInt();

                // Match dot
                scanner.Match('.');

                // Match 2-4 digits - Year
                scanner.StartMatch();
                if (!scanner.MatchDigits(2, 4)) return Err::FailedDateProcess();
                auto year_digits = scanner.MatchLength();
                int year = scanner.MatchToInt();

                if (year_digits == 2)
                {
                    year += 1900;
                }
                
                Documents::Data::Date dateData{ day, month, year };
                return Data{ dateData };
            }

            static inline Data ProcessSIUnit(StrScanner& scanner)
            {
                // \d+cm
                // \d+kg
                scanner.SkipWhitespace();

                // digits
                scanner.StartMatch();
                if (!scanner.MatchDigits(1, INT_MAX)) return Err::FailedSIUnitProcess();
                int value = scanner.MatchToInt();

                Documents::Data::SIUnit unit;

                // SI UNIT
                if (scanner.MatchStr("cm"))
                {
                    unit = Documents::Data::SIUnit::CM;
                }
                else if (scanner.MatchStr("kg"))
                {
                    unit = Documents::Data::SIUnit::KG;
                }
                else
                {
                    return Err::FailedSIUnitProcess();
                }

                Documents::Data::SIUnitValue siUnitData{ value, unit };
                return Data{ siUnitData };

            }

            static inline Data ProcessVaccine(StrScanner& scanner)
            {
                auto date_data = ProcessDate(scanner);
                if (date_data.Type() == DataType::Invalid) return Err::FailedVaccineProcess();

                scanner.SkipWhitespace();
                
                scanner.StartMatch();
                
                while (!scanner.IsAtEnd() && !std::isspace(scanner.Peek())) scanner.Advance();

                auto name = scanner.MatchToStr();

                Documents::Data::Vaccine vaccineData{ date_data.Get<Documents::Data::Date>(), name };

                return Data{ vaccineData };
            }

            static inline Data ProcessStrList(StrScanner& scanner)
            {
                Documents::Data::StrList strList{};

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
                    return Data{ Documents::Data::Sex::Female };
                }
                else if (scanner.Match('M'))
                {
                    return Data{ Documents::Data::Sex::Male };
                }
                else
                {
                    return Err::FailedProcess("INVALID SEX");
                }
            }
        }

        static inline Data ProcessTextData(const Data& data, const DataFieldCategory category)
        {
            StrScanner scanner{ data.Get<std::string>() };

            switch (category) { // sort by more clear type first to more vague types last
                case DataFieldCategory::Height:
                case DataFieldCategory::Weight:
                {
                    return DataProcessing::ProcessSIUnit(scanner);
                }
                case Documents::V2::DataFieldCategory::Sex:
                {
                    return DataProcessing::ProcessSex(scanner);
                }
                case DataFieldCategory::CountryList:
                    return DataProcessing::ProcessStrList(scanner);
                case DataFieldCategory::BoothDate:
                case DataFieldCategory::ValidDate:
                case DataFieldCategory::ExpirationDate:
                case DataFieldCategory::DateOfBirth:
                    return DataProcessing::ProcessDate(scanner);
                case DataFieldCategory::Duration:
                    break;
                case DataFieldCategory::EndDate:
                    break;
                case DataFieldCategory::Field:
                    break;
                case DataFieldCategory::FingerPrints:
                    break;
                case DataFieldCategory::HasValidSeal:
                    break;
                case DataFieldCategory::Name:
                {
                    return DataProcessing::ProcessName(scanner);
                }
                case DataFieldCategory::IssuingCity:
                case DataFieldCategory::Description:
                case DataFieldCategory::District:
                case DataFieldCategory::IssuingCountry:
                case DataFieldCategory::Nationality:
                case DataFieldCategory::DurationOfStay:
                case DataFieldCategory::Purpose:
                case DataFieldCategory::PassportNumber:
                {
                    return DataProcessing::ProcessGenericString(scanner);
                }
                case DataFieldCategory::PassportType:
                    break;
                case DataFieldCategory::PhysicalAppearance:
                    break;
                case DataFieldCategory::Vaccination1:
                case DataFieldCategory::Vaccination2:
                case DataFieldCategory::Vaccination3:
                {
                    return DataProcessing::ProcessVaccine(scanner);
                }
                case DataFieldCategory::BoothCounter:
                    return DataProcessing::ProcessGenericInt(scanner);
                default:
                    return { data };
            }

            std::cout << "IMPLEMENT ME!!! " << DataFieldCategoryAsString(category) << " : " << scanner.Start() << "\n";

            return DataProcessing::ProcessGenericString(scanner);
        }

        static inline Data ProcessFieldData(const Data& data, const FieldType type, const DataFieldCategory category)
        {
            switch (type) {
                case Documents::V2::FieldType::Text:
                    return ProcessTextData(data, category);
                case Documents::V2::FieldType::Image:
                    // Not implemented
                case Documents::V2::FieldType::Invalid:
                default:
                    return {};
            }
        }
    }

#pragma region Constructors
    template<typename T>
    Data::Data(const T& data, DataType type)
        : m_data{ data }, m_type{ type }, m_isBroken{ false }
    {}

    template<typename T>
    Data::Data(const T& data, DataType type, bool isBroken)
        : m_data{data}, m_type{type}, m_isBroken{isBroken}
    {}

    Data::Data(const std::string& data)
        : Data{data, DataType::GenericString}
    {}

    Data::Data(const long long data)
        : Data{data, DataType::GenericNumber}
    {}

    Data::Data(const Documents::Data::Date& data) : Data{data, DataType::Date}
    {}

    Data::Data(const Documents::Data::SIUnitValue& data)
        : Data{data, DataType::SIUnit}
    {}

    Data::Data(const Documents::Data::Vaccine& data)
        : Data{data, DataType::Vaccine}
    {}

    Data::Data(const Documents::Data::StrList& data)
        : Data{data, DataType::StrList}
    {}

    Data::Data(const Documents::Data::Sex data)
        : Data{data, DataType::Sex}
    {}

#pragma endregion

    std::string Data::Text() const
    {
        switch (m_type)
        {
            case DataType::Invalid:
                return "INVALID DATA";
            case DataType::GenericString:
                return this->Get<std::string>();
            case DataType::GenericNumber:
                return std::to_string(this->Get<long long>());
            case DataType::Date:
            {
                const Documents::Data::Date& date = this->Get<Documents::Data::Date>();
                std::ostringstream oss;
                oss << date.GetDay() << "."
                    << date.GetMonth() << "."
                    << date.GetYear();
                return oss.str();
            }
            case DataType::SIUnit:
            {
                const Documents::Data::SIUnitValue& siUnitValue = this->Get<Documents::Data::SIUnitValue>();
                std::ostringstream oss;
                oss << siUnitValue.value;

                switch (siUnitValue.unit)
                {
                    case Documents::Data::SIUnit::KG:
                        oss << " kg";
                        break;
                    case Documents::Data::SIUnit::CM:
                        oss << " cm";
                        break;
                }

                return oss.str();
            }
            case DataType::Vaccine:
            {
                const auto& vaccine = this->Get<Documents::Data::Vaccine>();

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
                const auto& strList = this->Get<Documents::Data::StrList>();
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
                const auto sex = this->Get<Documents::Data::Sex>();
                if (sex == Documents::Data::Sex::Female)
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

    FieldData::FieldData(const Data& data, const FieldType type, const DataFieldCategory category)
        : m_data{ data }, m_fieldType{ type }, m_fieldCategory{ category }, m_fieldState{DataFieldState::Initialized}
    {}

    const Data& FieldData::GetData() const
    {
        return m_data;
    }

    std::string FieldData::Text() const {
        return m_data.Text();
    }

    FieldType FieldData::Type() const
    {
        return m_fieldType;
    }

    FieldData DocData::Get(DataFieldCategory category) const
    {
        if (category == DataFieldCategory::Invalid) return {};
        size_t index = static_cast<size_t>(category) - 1;
        return m_data[index];
    }

    DocData::DocData(const std::array<FieldData, ArrayLength>& data)
        : m_data{ data }
    {}

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

    DocData DocDataBuilder::GetDocData()
    {
        for (auto& fieldData : m_data)
        {
            if (fieldData.m_fieldState != DataFieldState::Empty) {
                fieldData.m_data = Utils::ProcessFieldData(fieldData.m_data, fieldData.m_fieldType, fieldData.m_fieldCategory);
                // fieldData.m_fieldType = fieldData.m_data.Type(); // messy -- maybe I want to know what general type a field is?
                fieldData.m_fieldState = DataFieldState::ProcessedData;
            }
        }
        DocData docData{m_data};
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

}