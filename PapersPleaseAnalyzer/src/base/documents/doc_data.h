#pragma once
#include "base/documents/data/date.h"
#include "base/documents/data/field_data.h"
#include "base/documents/doc_data_type.h"
#include "base/utils/fixed_array.h"

#include <optional>
#include <variant>

namespace paplease {
	namespace documents {

#pragma region Data

		namespace detail {

			template<FieldCategory Category, typename Enable = void>
			struct field_data_type;

			template <FieldCategory Category>
			using field_data_type_t = typename field_data_type<Category>::type;

			template<FieldCategory Category>
			using FieldDataType = std::optional<std::reference_wrapper<const detail::field_data_type_t<Category>>>;

		}  // namespace detail

		class Data
		{
		public:
			Data() = default;

			template<typename T>
			explicit Data(const T& data, DataType type, bool isBroken = false);

			explicit Data(const std::string& data);
			explicit Data(int data);
			explicit Data(const data::Date& data);
			explicit Data(const data::SIUnitValue& data);
			explicit Data(const data::Vaccine& data);
			explicit Data(const data::StrList& data);
			explicit Data(data::Sex data);

			template<typename T>
			constexpr const T& Get() const
			{
				return std::get<T>(m_data);
			}

			std::string ToText() const;
			DataType Type() const;
			bool IsBroken() const;

			bool operator==(const Data&) const;

		private:
			std::variant<
				int,
				std::string,
				data::Date,
				data::SIUnitValue,
				data::Vaccine,
				data::StrList,
				data::Sex> m_data;
			DataType m_type = DataType::Invalid;
			bool m_isBroken = false;
		};

#pragma endregion

#pragma region FieldData

		class Field
		{  // Represents the data of any field
		public:
			Field() = default;
			Field(const Data& data, const FieldType type, const FieldCategory category);

			const Data& GetData() const;
			template<FieldCategory Category>
			constexpr detail::FieldDataType<Category> GetFieldData() const;

			std::string ToText() const;

			FieldType Type() const;
			FieldCategory Category() const;
			bool IsBroken() const;
		private:
			friend class DocDataBuilder;
		private:
			Data m_data; // temporary
			FieldType m_fieldType = FieldType::Invalid;
			FieldCategory m_fieldCategory = FieldCategory::Invalid; // index in DocData array
			FieldState m_fieldState = FieldState::Empty;
		};

#pragma endregion

#pragma region DocData

		class DocData
		{  // Represents the data of any document
		public:
			static constexpr size_t ArrayLength = static_cast<size_t>(FieldCategory::DATA_FIELD_CATEGORY_LENGTH) - 1;
		public:
			DocData() = default;

			const Field& GetField(FieldCategory category) const;
			template<FieldCategory Category>
			constexpr detail::FieldDataType<Category> GetFieldData() const;

			paplease::utils::FixedRefArray<Field, DocData::ArrayLength> GetAllValidFields() const;

			void PrintAllFields() const // temporary for ease of development
			{
				for (const auto& data : m_data)
				{
					if (data.Type() != FieldType::Text) continue;
					std::cout << data.ToText() << "\n";
				}
			}
		private:
			explicit DocData(const std::array<Field, ArrayLength>& data);
		private:
			friend class DocDataBuilder;

			std::array<Field, ArrayLength> m_data;
		};

#pragma endregion

#pragma region DocDataBuilder

		class DocDataBuilder
		{  // Builds a document data instance
		public:
			static const size_t ArrayLength = static_cast<size_t>(FieldCategory::DATA_FIELD_CATEGORY_LENGTH) - 1;
		public:
			DocDataBuilder() = default;

			bool AddFieldData(const FieldCategory category, Field&& data);
			std::optional<DocData> GetDocData();
			void Clear();
		private:
			std::array<Field, ArrayLength> m_data;
		};

#pragma endregion

	}  // namespace documents
}  // namespace paplease

#include "base/documents/doc_data.inl.h"