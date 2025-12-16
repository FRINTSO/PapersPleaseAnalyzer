#ifndef PAPLEASE_DATE_H
#define PAPLEASE_DATE_H

#include <cstdio>
#include <string>
#include <format>

#include <paplease/types.h>

struct date_t {
	u8 day;
	u8 month;
	u16 year;

	constexpr bool operator==(const date_t &other) const noexcept
	{
		return day == other.day && month == other.month &&
			   year == other.year;
	}

	/*constexpr bool operator<(const date_t &other)
	{
		if (year != other.year)
			return year < other.year;

		if (month != other.month)
			return month < other.month;

		return day < other.day;
	}*/
	constexpr bool empty() const noexcept
	{
		return day == 0 && month == 0 && year == 0;
	}
};

constexpr bool operator<(const date_t& a, const date_t& b) {
	if (a.year != b.year) return a.year < b.year;
	if (a.month != b.month) return a.month < b.month;
	return a.day < b.day;
}

inline bool parse_date(date_t &out, const std::string &text)
{
	int day, month, year;
	if (sscanf(text.c_str(), "%d.%d.%d", &day, &month, &year) != 3)
		return false;
	if (year < 100)
		year += 1900;
	out = { (u8)day, (u8)month, (u16)year };
	return true;
}

inline std::string format_date(const date_t &date)
{
	return std::format("{}.{}.{}", date.day, date.month, date.year);
}

#endif // PAPLEASE_DATE_H
