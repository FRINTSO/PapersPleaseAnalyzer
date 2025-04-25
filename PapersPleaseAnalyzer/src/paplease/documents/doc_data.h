#pragma once
#include "paplease/documents/data/date.h"
#include "paplease/documents/data/field_data.h"
#include "paplease/documents/data/photo.h"
#include "paplease/documents/doc_data_type.h"
#include "paplease/documents/doc_layout.h"
#include "paplease/core/fixed.h"
#include "paplease/core/optref.h"

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
			using FieldDataType = core::OptRef<const detail::field_data_type_t<Category>>;

		}  // namespace detail

		class Data
		{
		public:
			Data() = default;

			template<typename T>
			explicit Data(T&& data, DataType type, bool isBroken = false);

			explicit Data(std::string&& data);
			explicit Data(int data);
			explicit Data(data::Date&& data);
			explicit Data(data::SIUnitValue&& data);
			explicit Data(data::Vaccine&& data);
			explicit Data(data::StrList&& data);
			explicit Data(data::Sex data);
			explicit Data(data::Photo&& data);

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
				data::Sex,
				data::Photo> m_data;
			DataType m_type = DataType::Invalid;
			bool m_isBroken = false;
		};

		static_assert(std::is_move_constructible_v<Data>);
		static_assert(std::is_move_assignable_v<Data>);

#pragma endregion

#pragma region FieldData

		class Field
		{  // Represents the data of any field
		public:
			Field() = default;
#if DOCDATA_OPTIMIZATION
			explicit Field(Data&& data, FieldType type, FieldCategory category);
#else
			Field(const Data& data, const FieldType type, const FieldCategory category);
#endif
			const Data& GetData() const;
			template<FieldCategory Category>
			constexpr detail::FieldDataType<Category> GetFieldData() const;

			std::string ToString() const;

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
#if DOCDATA_OPTIMIZATION
		class DocData
		{  // Represents the data of any document
			static constexpr size_t MaxFields = 10;
			using index_type = unsigned char;
			static_assert(std::is_unsigned_v<index_type> && std::numeric_limits<index_type>::max() + 1 >= MaxFields,
						  "index_type must be unsigned and large enough to hold MaxFields.");
		public:
			DocData() = default;

			template<FieldCategory Category>
			constexpr detail::FieldDataType<Category> GetFieldData() const;

			core::OptRef<const Field> GetField(FieldCategory category, bool returnBroken) const;
			bool HasBrokenData() const;

		private:
			void AddField(Field&& field);
			bool TryAddField(Field&& field);
			void Reset();

			friend class DocDataBuilder;

		private:
			core::FixedArray<Field, MaxFields> m_fieldStorage;
			core::FixedTable<FieldCategory, index_type> m_fieldLookup;
		};

		static_assert(std::is_move_constructible_v<DocData>);
		static_assert(std::is_move_assignable_v<DocData>);
#else
		class DocData
		{  // Represents the data of any document
		public:
			static constexpr size_t ArrayLength = static_cast<size_t>(FieldCategory::Count);
		public:
			DocData() = default;

			std::optional<std::reference_wrapper<const Field>> GetField(FieldCategory category, bool returnBroken) const;
			template<FieldCategory Category>
			constexpr detail::FieldDataType<Category> GetFieldData() const;

			core::FixedRefArray<Field, DocData::ArrayLength> GetAllValidFields() const;

			bool HasBrokenData() const;

			void PrintAllFields() const; // temporary for ease of development
		private:
			explicit DocData(const std::array<Field, ArrayLength>& data);
		private:
			friend class DocDataBuilder;

			std::array<std::optional<Field>, ArrayLength> m_data;
		};
#endif

#pragma endregion

#pragma region DocDataBuilder

#if DOCDATA_OPTIMIZATION
		class DocDataBuilder
		{  // Builds a document data instance
		public:
			DocDataBuilder() = default;

			bool AddField(Field&& data, FieldCategory category);
			DocData Build();

		private:
			DocData m_data;
		};
#else
		class DocDataBuilder
		{  // Builds a document data instance
		public:
			static const size_t ArrayLength = static_cast<size_t>(FieldCategory::Count);
		public:
			DocDataBuilder() = default;

			bool AddFieldData(const FieldCategory category, Field&& data);
			DocData GetDocData();
			void Clear();
		private:
			std::array<Field, ArrayLength> m_data;
		};
#endif

#pragma endregion

	}  // namespace documents
}  // namespace paplease

#include "paplease/documents/doc_data.inl.h"