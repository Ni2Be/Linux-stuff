#include "Date.h"



Date::Date()
{
}

Date::Date(int year, int month, int day, int hour, int minute, int second)
	:
	m_year(year),
	m_month(month),
	m_day(day),
	m_hour(hour),
	m_minute(minute),
	m_second(second)
{
}

Date::Date(std::chrono::system_clock::time_point date)
{
	auto date_string = time_to_string(date);
	read_string(date_string);
}

Date::Date(std::string date)
{
	read_string(date);
}

std::string Date::get_date_string()
{
	return time_to_string(to_time());
}

std::chrono::system_clock::time_point Date::get_time_point()
{
	return to_time();
}


void Date::read_string(std::string date)
{
	auto year   = date.substr(20, 24);

	//TODO parsen
	auto month  = date.substr(5, 8);
	auto day    = date.substr(8, 10);
	auto hour   = date.substr(11, 13);
	auto minute = date.substr(14, 16);
	auto second = date.substr(17, 19);
	
	std::stringstream ss;
	ss << year;
	ss >> m_year;
	ss << month;
	ss >> m_month;
	ss << day;
	ss >> m_day;
	ss << hour;
	ss >> m_hour;
	ss << minute;
	ss >> m_minute;
	ss << second;
	ss >> m_second;
}

std::chrono::system_clock::time_point Date::to_time(std::string date)
{
	std::tm tm = {};
	std::stringstream ss;
	ss << date;

	// 012345678910121416182022
	//"Mon Jan 00 00:00:00 0000"
	ss >> std::get_time(&tm, "%a %b  %d %H:%M:%S %Y");
	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
	return tp;
}

std::chrono::system_clock::time_point Date::to_time()
{
	std::string date = "Mon Jan 00 00:00:00 0000";

	change_year(date, m_year);
	change_day(date, m_day);
	change_hour(date, m_hour);
	change_minute(date, m_minute);
	change_second(date, m_second);

	return to_time(date);
}

std::string Date::time_to_string(std::chrono::system_clock::time_point date)
{
	struct tm buf;
	char dateString[26];
	time_t time = std::chrono::system_clock::to_time_t(date);
	localtime_s(&buf, &time);
	asctime_s(dateString, 26, &buf);
	return dateString;
}

void Date::change_year(std::string& date, int year)
{
	date.replace(20, 4, std::to_string(year));
}

void Date::change_day(std::string& date, int day)
{
	if (log10(day) < 1)
		date.replace(8, 2, " " + std::to_string(day));
	else
		date.replace(8, 2, std::to_string(day));
}

void Date::change_hour(std::string& date, int hour)
{
	if (log10(hour) < 1)
		date.replace(11, 2, "0" + std::to_string(hour));
	else
		date.replace(11, 2, std::to_string(hour));
}

void Date::change_minute(std::string& date, int minute)
{
	if (log10(minute) < 1)
		date.replace(14, 2, "0" + std::to_string(minute));
	else
		date.replace(14, 2, std::to_string(minute));
}

void Date::change_second(std::string& date, int second)
{
	if (log10(second) < 1)
		date.replace(14, 2, "0" + std::to_string(second));
	else
		date.replace(14, 2, std::to_string(second));
}
