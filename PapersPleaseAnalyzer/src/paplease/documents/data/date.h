#pragma once
#include "paplease/types.h"

#include <string>

namespace paplease {
	namespace documents {
		namespace data {

			class Date
			{
			public:
				Date();
				Date(const u8 d, const u8 m, const u16 y);

				bool IsValid() const;

				u8 GetDay() const;
				u8 GetMonth() const;
				u16 GetYear() const;

				void SetDay(const u8 day);
				void SetMonth(const u8 month);
				void SetYear(const u16 year);

				Date operator ++(); // prefix
				Date operator ++(int); // postfix
				Date operator --(); // prefix
				Date operator --(int); // postfix

				std::string ToString() const;
			private:
				u16 m_year;
				u8 m_month;
				u8 m_day;
			};

			bool operator ==(const Date&, const Date&); // comparison operators
			bool operator !=(const Date&, const Date&);
			bool operator <(const Date&, const Date&);
			bool operator >(const Date&, const Date&);
			bool operator <=(const Date&, const Date&);
			bool operator >=(const Date&, const Date&);

			std::ostream& operator << (std::ostream& os, const Date& d); // output operator

		}  // namespace data
	}  // namespace documents
}  // namespace paplease
