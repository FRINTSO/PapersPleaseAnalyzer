#pragma once

namespace Documents::V2
{

enum class DataFieldCategory {
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
	DATA_FIELD_CATEGORY_LENGTH,
};

enum class DataFieldType {
	Invalid = 0,
	TextField,
	ImageField,
};

struct Data {
	std::string str;
	DataFieldCategory category;
};

class DocData {
public:
	static const size_t ARRAY_LENGTH = static_cast<size_t>(DataFieldCategory::DATA_FIELD_CATEGORY_LENGTH);
public:
	DocData() = default;
	DocData(std::array<Data, ARRAY_LENGTH> data)
		: m_data{data}
	{}
	/*
	DocData(std::array<Data, ARRAY_LENGTH> data, size_t dataLength)
		: m_data{ data }, m_dataLength{ dataLength }
	{}
	*/

	constexpr const std::array<Data, ARRAY_LENGTH> GetData() const {
		return m_data;
	}

	/*
	constexpr const size_t GetLength() const {
		return m_dataLength;
	}
	*/
private:
	std::array<Data, ARRAY_LENGTH> m_data;
	// size_t m_dataLength;
};



#pragma region Data
#if 0

struct AccessPermitData {
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

struct CertificateOfVaccinationData {
	std::string name;					// Profile::Name
	std::string passportNumber;			// Profile::PassportNumber
	std::string vaccination1;			// Self-referential
	std::string vaccination2;			// Self-referential
	std::string vaccination3;			// Self-referential
};

struct DiplomaticAuthorizationData {
	std::string name;					// Profile::Name
	std::string passportNumber;			// Profile::PassportNumber
	std::string issuingCountry;			// Profile::Nationality
	std::string countryList;
	bool		hasValidSeal;
};

struct EntryPermitData {
	std::string name;					// Profile::Name
	std::string passportNumber;			// Profile::PassportNumber
	std::string purpose;
	std::string duration;
	std::string expirationDate;			// GameState::CurrentDate
	bool		hasValidSeal;			// Self-referential
};

struct EntryTicketData {
	std::string validDate;				// GameState::CurrentDate?
};

struct GrantOfAsylumData {
	std::string name;					// Profile::Name
	std::string country;				// Profile::Nationality
	std::string passportNumber;			// Profile::PassportNumber
	std::string dateOfBirth;			// Profile::DateOfBirth
	std::string height;					// Profile::Height
	std::string weight;					// Profile::Weight
	std::string expirationDate;			// GameState::CurrentDate?
	bool		hasValidSeal;			// Self-referential
};

struct IdentityCardData {
	std::string district;				// Self-referential
	std::string name;					// Profile::Name
	std::string dateOfBirth;			// Profile::DateOfBirth
	std::string height;					// Profile::Height
	std::string weight;					// Profile::Weight
};


struct IdentitySupplementData {
	std::string height;					// Profile::Height
	std::string weight;					// Profile::Weight
	std::string description;
	std::string expirationDate;			// GameState::CurrentDate?
};

struct PassportData {
	std::string name;					// Profile::Name
	std::string dateOfBirth;			// Profile::DateOfBirth
	std::string sex;					// Profile::Sex
	std::string issuingCity;			// GameData::PassportCities?
	std::string expirationDate;			// GameState::CurrentDate?
	std::string passportNumber;			// Profile::PassportNumber
	PassportType passportType;
};

struct WorkPassData {
	std::string name;					// Profile::Name
	std::string field;
	std::string endDate;				// GameState::CurrentDate?
	bool		hasValidSeal;			// Self-referential
};

#endif
#pragma endregion

constexpr const std::string DataFieldCategoryAsString(DataFieldCategory dataFieldCategory) {
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