#pragma once
#include "base/document_data/date.h"
#include "base/document_data/field_data.h"

#include <variant>

namespace Documents::V2 {

	enum class DataFieldCategory
	{
		Invalid = 0,
		Nationality,
		CountryList,
		DateOfBirth,
		Description,
		District,
		Duration,
		DurationOfStay,
		EndDate,
		ExpirationDate,
		Field,
		FingerPrints,
		HasValidSeal,
		Height,
		IssuingCity,
		IssuingCountry,
		Name,
		PassportNumber,
		PassportType,
		Photo,
		PhysicalAppearance,
		Purpose,
		Sex,
		ThumbPrint,
		Vaccination1,
		Vaccination2,
		Vaccination3,
		ValidDate,
		Weight,
		BoothDate,
		BoothCounter,

		DATA_FIELD_CATEGORY_LENGTH,
	};

	enum class DataType
	{
		Invalid = 0,
		GenericString,
		GenericNumber,
		StrList,
		Date,
		Image,
		SIUnit,
		Vaccine,
		Sex,
	};

	enum class FieldType
	{
		Invalid = 0,
		Text,
		Image,
	};

	enum class DataFieldState
	{
		Empty = 0,
		Initialized,
		ProcessedData
	};

	class Data
	{
	public:

	public:
		Data() = default;
		template<typename T>
		Data(const T& data, DataType type);
		template<typename T>
		Data(const T& data, DataType type, bool isBroken);

		Data(const std::string& data);
		Data(const long long data);
		Data(const Documents::Data::Date& data);
		Data(const Documents::Data::SIUnitValue& data);
		Data(const Documents::Data::Vaccine& data);
		Data(const Documents::Data::StrList& data);
		Data(const Documents::Data::Sex data);

		template<typename T>
		constexpr const T& Get() const
		{
			return std::get<T>(m_data);
		}

		std::string Text() const;
		DataType Type() const;

	private:
		std::variant<
			long long,
			std::string,
			Documents::Data::Date,
			Documents::Data::SIUnitValue,
			Documents::Data::Vaccine,
			Documents::Data::StrList,
			Documents::Data::Sex> m_data;
		DataType m_type;
		bool m_isBroken;
	};

	class FieldData
	{  // Represents the data of any field
	public:
		FieldData() = default;
		FieldData(const Data& data, const FieldType type, const DataFieldCategory category);

		const Data& GetData() const;
		std::string Text() const;
		FieldType Type() const;
	private:
		friend class DocDataBuilder;
	private:
		Data m_data; // temporary
		FieldType m_fieldType;
		DataFieldCategory m_fieldCategory; // index in DocData array
		DataFieldState m_fieldState;
	};

	class DocData
	{  // Represents the data of any document
	public:
		static const size_t ArrayLength = static_cast<size_t>(DataFieldCategory::DATA_FIELD_CATEGORY_LENGTH) - 1;
	public:
		DocData() = default;

		FieldData Get(DataFieldCategory category) const;
		void PrintAllFields() const // temporary for ease of development
		{
			for (const auto& data : m_data)
			{
				if (data.Type() != FieldType::Text) continue;
				std::cout << data.Text() << "\n";
			}
		}
	private:
		DocData(const std::array<FieldData, ArrayLength>& data);
	private:
		friend class DocDataBuilder;

		std::array<FieldData, ArrayLength> m_data;
	};

	class DocDataBuilder
	{  // Builds a document data instance
	public:
		static const size_t ArrayLength = static_cast<size_t>(DataFieldCategory::DATA_FIELD_CATEGORY_LENGTH) - 1;
	public:
		DocDataBuilder() = default;

		bool AddFieldData(const DataFieldCategory category, FieldData&& data);
		DocData GetDocData();
		void Clear();
	private:
		std::array<FieldData, ArrayLength> m_data;
	};


#pragma region Data
#if 0

	struct AccessPermitData
	{
		std::string name;					// Profile::Name
		std::string country;				// Profile::Nationality
		std::string passportNumber;			// Profile::PassportNumber
		std::string purpose;				// Profile
		std::string durationOfStay;			// GameState::Conversation?
		std::string height;					// Profile::Height
		std::string weight;					// Profile::Weight
		std::string physicalAppearance;		// Profile
		std::string expirationDate;			// GameState::CurrentDate?
		bool		hasValidSeal;			// Self-referential
	};

	struct CertificateOfVaccinationData
	{
		std::string name;					// Profile::Name
		std::string passportNumber;			// Profile::PassportNumber
		std::string vaccination1;			// Self-referential
		std::string vaccination2;			// Self-referential
		std::string vaccination3;			// Self-referential
	};

	struct DiplomaticAuthorizationData
	{
		std::string name;					// Profile::Name
		std::string passportNumber;			// Profile::PassportNumber
		std::string issuingCountry;			// Profile::Nationality
		std::string countryList;
		bool		hasValidSeal;
	};

	struct EntryPermitData
	{
		std::string name;					// Profile::Name
		std::string passportNumber;			// Profile::PassportNumber
		std::string purpose;
		std::string duration;
		std::string expirationDate;			// GameState::CurrentDate
		bool		hasValidSeal;			// Self-referential
	};

	struct EntryTicketData
	{
		std::string validDate;				// GameState::CurrentDate?
	};

	struct GrantOfAsylumData
	{
		std::string name;					// Profile::Name
		std::string country;				// Profile::Nationality
		std::string passportNumber;			// Profile::PassportNumber
		std::string dateOfBirth;			// Profile::DateOfBirth
		std::string height;					// Profile::Height
		std::string weight;					// Profile::Weight
		std::string expirationDate;			// GameState::CurrentDate?
		bool		hasValidSeal;			// Self-referential
	};

	struct IdentityCardData
	{
		std::string district;				// Self-referential
		std::string name;					// Profile::Name
		std::string dateOfBirth;			// Profile::DateOfBirth
		std::string height;					// Profile::Height
		std::string weight;					// Profile::Weight
	};


	struct IdentitySupplementData
	{
		std::string height;					// Profile::Height
		std::string weight;					// Profile::Weight
		std::string description;
		std::string expirationDate;			// GameState::CurrentDate?
	};

	struct PassportData
	{
		std::string name;					// Profile::Name
		std::string dateOfBirth;			// Profile::DateOfBirth
		std::string sex;					// Profile::Sex
		std::string issuingCity;			// GameData::PassportCities?
		std::string expirationDate;			// GameState::CurrentDate?
		std::string passportNumber;			// Profile::PassportNumber
		PassportType passportType;
	};

	struct WorkPassData
	{
		std::string name;					// Profile::Name
		std::string field;
		std::string endDate;				// GameState::CurrentDate?
		bool		hasValidSeal;			// Self-referential
	};

#endif
#pragma endregion

	
	const DocData ProcessDocData();

	constexpr const std::string DataFieldCategoryAsString(DataFieldCategory dataFieldCategory)
	{
		switch (dataFieldCategory)
		{
			case DataFieldCategory::Invalid:
				return "Invalid";
			case DataFieldCategory::Nationality:
				return "Nationality";
			case DataFieldCategory::CountryList:
				return "CountryList";
			case DataFieldCategory::DateOfBirth:
				return "DateOfBirth";
			case DataFieldCategory::Description:
				return "Description";
			case DataFieldCategory::District:
				return "District";
			case DataFieldCategory::Duration:
				return "Duration";
			case DataFieldCategory::DurationOfStay:
				return "DurationOfStay";
			case DataFieldCategory::EndDate:
				return "EndDate";
			case DataFieldCategory::ExpirationDate:
				return "ExpirationDate";
			case DataFieldCategory::Field:
				return "Field";
			case DataFieldCategory::FingerPrints:
				return "FingerPrints";
			case DataFieldCategory::HasValidSeal:
				return "HasValidSeal";
			case DataFieldCategory::Height:
				return "Height";
			case DataFieldCategory::IssuingCity:
				return "IssuingCity";
			case DataFieldCategory::IssuingCountry:
				return "IssuingCountry";
			case DataFieldCategory::Name:
				return "Name";
			case DataFieldCategory::PassportNumber:
				return "PassportNumber";
			case DataFieldCategory::PassportType:
				return "PassportType";
			case DataFieldCategory::Photo:
				return "Photo";
			case DataFieldCategory::PhysicalAppearance:
				return "PhysicalAppearance";
			case DataFieldCategory::Purpose:
				return "Purpose";
			case DataFieldCategory::Sex:
				return "Sex";
			case DataFieldCategory::ThumbPrint:
				return "ThumbPrint";
			case DataFieldCategory::Vaccination1:
				return "Vaccination1";
			case DataFieldCategory::Vaccination2:
				return "Vaccination2";
			case DataFieldCategory::Vaccination3:
				return "Vaccination3";
			case DataFieldCategory::ValidDate:
				return "ValidDate";
			case DataFieldCategory::Weight:
				return "Weight";
			default:
				return "";
		}
	}


} // namespace Documents::V2