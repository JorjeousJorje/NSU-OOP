#include "date.hpp"
#include <exception>
#include <iomanip>
#include <sstream>
#include <stdexcept>


date::date(int day_, int month_, int year_)
{
	if (day_ <= 0 || month_ <= 0 || year_ <= 0 || month_ > 12 || day_ > 31) {
		throw std::invalid_argument("invalid date");;
	}
	if(month_ == 4 || month_ == 6 || month_ == 9 || month_ == 11) {
		if (day_ > 30) {
			throw std::invalid_argument("invalid date");;
		}
	}
	day = day_;
	month = month_;
	year = year_;
}

bool date::check_end_date(const date& current, const date& end)
{
	if (current.get_year() > end.get_year()) {
		return false;
	}
	if (current.get_year() == end.get_year()) {
		if (current.get_month() > end.get_month()) {
			return false;
		}
		if (current.get_month() == end.get_month()) {
			if (current.get_day() > end.get_day()) {
				return false;
			}
			return true;
		}
		return true;
	}
	return true;
}

bool date::check_start_date(const date& current, const date& start)
{
	if (start.get_year() > current.get_year()) {
		return false;
	}
	if (start.get_year() == current.get_year()) {
		if (start.get_month() > current.get_month()) {
			return false;
		}
		if (start.get_month() == current.get_month()) {
			if (start.get_day() > current.get_day()) {
				return false;
			}
			return true;
		}
		return true;
	}
	return true;
}

bool date::date_cmp(const date& cur_date, const date& start, const date& end)
{
	if (check_end_date(cur_date, end)) {
		if (check_start_date(cur_date, start)) {
			return true;
		}
	}
	return false;
}

int date::get_day() const
{
	return day;
}

int date::get_month() const
{
	return month;
}

int date::get_year() const
{
	return year;
}

size_t date::max_data_length()
{
	return 10;
}

date date::parse(const std::string& date_str)
{
	std::istringstream in(date_str);

	int day{};
	int month{};
	int year{};
	char ch{};

	if (!(in >> day) || !(in >> ch) || ch != '.') {
		throw std::invalid_argument("Invalid date");
	}

	if (!(in >> month) || !(in >> ch) || ch != '.') {
		throw std::invalid_argument("Invalid date");
	}

	if (!(in >> year)) {
		throw std::invalid_argument("Invalid date");
	}
	return date{ day, month, year };
}

std::istream& operator>>(std::istream& in, date& date)
{
	std::string date_str;

	if (!(in >> date_str)) {
		throw std::invalid_argument("Invalid date");
	}
	
	date = date::parse(date_str);
	return in;
}

std::ostream& operator<<(std::ostream& out, date& date)
{
	out << std::setfill('0')
		<< std::setw(2) << date.get_day() << '.'
		<< std::setw(2) << date.get_month() << '.'
		<< std::setw(4) << date.get_year();

	return out;
}
