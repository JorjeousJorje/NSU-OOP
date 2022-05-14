#pragma once
#include <string>

class date final
{
public:
	date() : day(0), month(0), year(0) {}
	date(int day, int month, int year);
	static bool date_cmp(const date& cur_date, const date& start, const date& end);
	static bool check_start_date(const date& current, const date& start);
	static bool check_end_date(const date& current, const date& end);

	int get_day() const;
	int get_month() const;
	int get_year() const;

	static size_t max_data_length();
	
	static date parse(const std::string& date_str);

	friend std::istream& operator>>(std::istream& in, date& date);
	friend std::ostream& operator<<(std::ostream& out, date& date);
	
private:
	int day;
	int month;
	int year;
};
