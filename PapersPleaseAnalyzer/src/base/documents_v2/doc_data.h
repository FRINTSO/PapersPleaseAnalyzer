#pragma once
#include "base/document_data/date.h"
#include "base/document_data/field_data.h"
#include "base/documents_v2/doc_data_type.h"

#include <optional>
#include <variant>

namespace paplease {
	namespace documents {
		namespace v2 {

#pragma region Data

			class Data
			{
			public:
				Data() = default;
				template<typename T>
				Data(const T& data, DataType type);
				template<typename T>
				Data(const T& data, DataType type, bool isBroken);

				Data(const std::string& data);
				Data(const int data);
				Data(const data::Date& data);
				Data(const data::SIUnitValue& data);
				Data(const data::Vaccine& data);
				Data(const data::StrList& data);
				Data(const data::Sex data);

				template<typename T>
				constexpr const T& Get() const
				{
					return std::get<T>(m_data);
				}

				std::string Text() const;
				DataType Type() const;
				bool Broken() const;

			private:
				std::variant<
					int,
					std::string,
					data::Date,
					data::SIUnitValue,
					data::Vaccine,
					data::StrList,
					data::Sex> m_data;
				DataType m_type;
				bool m_isBroken;
			};

#pragma endregion

#pragma region FieldData

			class FieldData
			{  // Represents the data of any field
			public:
				FieldData() = default;
				FieldData(const Data& data, const FieldType type, const DataFieldCategory category);

				const Data& GetData() const;
				std::string Text() const;
				FieldType Type() const;
				bool Broken() const;
			private:
				friend class DocDataBuilder;
			private:
				Data m_data; // temporary
				FieldType m_fieldType;
				DataFieldCategory m_fieldCategory; // index in DocData array
				DataFieldState m_fieldState;
			};

#pragma endregion

#pragma region DocData

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

#pragma endregion

#pragma region DocDataBuilder

			class DocDataBuilder
			{  // Builds a document data instance
			public:
				static const size_t ArrayLength = static_cast<size_t>(DataFieldCategory::DATA_FIELD_CATEGORY_LENGTH) - 1;
			public:
				DocDataBuilder() = default;

				bool AddFieldData(const DataFieldCategory category, FieldData&& data);
				std::optional<DocData> GetDocData();
				void Clear();
			private:
				std::array<FieldData, ArrayLength> m_data;
			};

#pragma endregion

		}  // namespace v2
	}  // namespace documents
}  // namespace paplease
