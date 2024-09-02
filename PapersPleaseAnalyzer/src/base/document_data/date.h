#pragma once
#include <string>

namespace paplease {
	namespace documents {
		namespace data {

			class Date
			{
			public:
				Date();
				Date(const int d, const int m, const int y);

				bool IsValid() const;

				int GetDay() const;
				int GetMonth() const;
				int GetYear() const;

				void SetDay(const int day);
				void SetMonth(const int month);
				void SetYear(const int year);

				Date operator ++(); // prefix
				Date operator ++(int); // postfix
				Date operator --(); // prefix
				Date operator --(int); // postfix

				std::string ToString() const;
			private:
				int m_year;
				int m_month;
				int m_day;
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
