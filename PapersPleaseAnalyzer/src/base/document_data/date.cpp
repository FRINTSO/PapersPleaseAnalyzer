#include "pch.h"
#include "base/document_data/Date.h"
#include <sstream>

namespace Documents::Data {

	///////////////////////////// construction //////////
	Date::Date(const int d, const int m, const int y)
	{
		m_day = d;
		m_month = m;
		m_year = y; // this assumes year is given fully, not Y2K corrections
	};

	///////////////////////////// inline definitions //////////
	Date::Date() { m_year = 0; m_month = 0; m_day = 0; };

	int Date::GetDay() const { return m_day; };
	int Date::GetMonth() const { return m_month; };
	int Date::GetYear() const { return m_year; };

	void Date::SetDay(const int day) { Date::m_day = day; };
	void Date::SetMonth(const int month) { Date::m_month = month; };
	void Date::SetYear(const int year) { Date::m_year = year; };

	bool Date::IsValid() const
	{
		// This function will check the given date is valid or not.
		// If the date is not valid then it will return the value false.
		// Need some more checks on the year, though
		if (m_year < 0) return false;
		if (m_month > 12 || m_month < 1) return false;
		if (m_day > 31 || m_day < 1) return false;
		if ((m_day == 31 &&
			(m_month == 2 || m_month == 4 || m_month == 6 || m_month == 9 || m_month == 11)))
			return false;
		if (m_day == 30 && m_month == 2) return false;
		if (m_year < 2000)
		{
			if ((m_day == 29 && m_month == 2) && !((m_year - 1900) % 4 == 0)) return false;
		};
		if (m_year > 2000)
		{
			if ((m_day == 29 && m_month == 2) && !((m_year - 2000) % 4 == 0)) return false;
		};
		return true;
	};

	bool operator ==(const Date& d1, const Date& d2)
	{
		// check for equality
		if (!d1.IsValid()) { return false; };
		if (!d2.IsValid()) { return false; };
		if ((d1.GetDay() == d2.GetDay())
			&& (d1.GetMonth() == d2.GetMonth())
			&& (d1.GetYear() == d2.GetYear()))
		{
			return true;
		};
		return false;
	}
	bool operator !=(const Date& d1, const Date& d2)
	{
		return !(d1 == d2);
	}
	bool operator <(const Date& d1, const Date& d2)
	{
		if (!d1.IsValid()) { return false; }; // not meaningful, return anything
		if (!d2.IsValid()) { return false; }; // should really be an exception, but ?
		if (d1.GetYear() < d2.GetYear()) { return true; }
		else if (d1.GetYear() > d2.GetYear()) { return false; }
		else if (d1.GetMonth() < d2.GetMonth()) { return true; }
		else if (d1.GetMonth() > d2.GetMonth()) { return false; }
		else if (d1.GetDay() < d2.GetDay()) { return true; }
		return false;
	};
	bool operator >(const Date& d1, const Date& d2)
	{
		if (d1 == d2) { return false; }; // this is strict inequality
		if (d1 < d2) { return false; };
		return true;
	}
	bool operator <=(const Date& d1, const Date& d2)
	{
		if (d1 == d2) { return true; }
		return (d1 < d2);
	}
	bool operator >=(const Date& d1, const Date& d2)
	{
		if (d1 == d2) { return true; };
		return (d1 > d2);
	};

	inline Date NextDate(const Date& d)
	{
		Date ndat;
		if (!d.IsValid()) { return ndat; };
		ndat = Date((d.GetDay() + 1), d.GetMonth(), d.GetYear()); if (ndat.IsValid()) return ndat;
		ndat = Date(1, (d.GetMonth() + 1), d.GetYear()); if (ndat.IsValid()) return ndat;
		ndat = Date(1, 1, (d.GetYear() + 1)); return ndat;
	}
	inline Date PreviousDate(const Date& d)
	{
		Date ndat;
		if (!d.IsValid()) { return ndat; }; // return zero
		ndat = Date((d.GetDay() - 1), d.GetMonth(), d.GetYear()); if (ndat.IsValid()) return ndat;
		ndat = Date(31, (d.GetMonth() - 1), d.GetYear()); if (ndat.IsValid()) return ndat;
		ndat = Date(30, (d.GetMonth() - 1), d.GetYear()); if (ndat.IsValid()) return ndat;
		ndat = Date(29, (d.GetMonth() - 1), d.GetYear()); if (ndat.IsValid()) return ndat;
		ndat = Date(28, (d.GetMonth() - 1), d.GetYear()); if (ndat.IsValid()) return ndat;
		ndat = Date(31, 12, (d.GetYear() - 1)); return ndat;
	};
	Date Date::operator ++(int)
	{ // postfix operator
		Date d = *this;
		*this = NextDate(d);
		return d;
	}
	Date Date::operator ++()
	{ // prefix operator
		*this = NextDate(*this);
		return *this;
	}
	Date Date::operator --(int)
	{ // postfix operator, return current value
		Date d = *this;
		*this = PreviousDate(*this);
		return d;
	}
	Date Date::operator --()
	{ // prefix operator, return new value
		*this = PreviousDate(*this);
		return *this;
	};
	inline long LongDate(const Date& d)
	{
		if (d.IsValid()) { return d.GetYear() * 10000 + d.GetMonth() * 100 + d.GetDay(); };
		return -1;
	};
	std::ostream& operator << (std::ostream& os, const Date& d)
	{
		os << d.GetDay() << "." << d.GetMonth() << "." << d.GetYear();
		return os;
	}

}