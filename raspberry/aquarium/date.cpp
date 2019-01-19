#include "date.h"


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
	m_year   = std::stoi(date.substr(20, 24));
	m_month  = month_to_int(date.substr(4, 3));
	m_day    = std::stoi(date.substr(8, 2));
	m_hour   = std::stoi(date.substr(11, 2));
	m_minute = std::stoi(date.substr(14, 2));
	m_second = std::stoi(date.substr(17, 2));
}

std::chrono::system_clock::time_point Date::to_time(std::string date)
{
	std::tm tm = {};
	std::stringstream ss;
	ss << date;

	// 012345678910121416182022 index helper
	//"Mon Jan 00 00:00:00 0000"
	ss >> std::get_time(&tm, "%a %b  %d %H:%M:%S %Y");
	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
	return tp;
}

std::chrono::system_clock::time_point Date::to_time()
{
	std::string date = "Mon Jan 00 00:00:00 0000";

	change_year(date, m_year);
	change_month(date, m_month);
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

void Date::change_month(std::string& date, int month)
{
	date.replace(4, 3, int_to_month(month));
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
		date.replace(17, 2, "0" + std::to_string(second));
	else
		date.replace(17, 2, std::to_string(second));
}

int Date::month_to_int(std::string month_str)
{
	if      (month_str == "Jan")
		return 1;
	else if (month_str == "Feb")
		return 2;
	else if (month_str == "Mar")
		return 3;
	else if (month_str == "Apr")
		return 4;
	else if (month_str == "May")
		return 5;
	else if (month_str == "Jun")
		return 6;
	else if (month_str == "Jul")
		return 7;
	else if (month_str == "Aug")
		return 8;
	else if (month_str == "Sep")
		return 9;
	else if (month_str == "Oct")
		return 10;
	else if (month_str == "Nov")
		return 11;
	else if (month_str == "Dec")
		return 12;
	else
		return -1;
}

std::string Date::int_to_month(int month)
{
	switch (month)
	{
	case 1:
		return "Jan";
	case 2:
		return "Feb";
	case 3:
		return "Mar";
	case 4:
		return "Apr";
	case 5:
		return "May";
	case 6:
		return "Jun";
	case 7:
		return "Jul";
	case 8:
		return "Aug";
	case 9:
		return "Sep";
	case 10:
		return "Oct";
	case 11:
		return "Nov";
	case 12:
		return "Dec";
	default:
		return "";
	}
}


void Date::add_year(int count)
{
	m_year += count;
}

void Date::add_month(int count)
{
	//m_month from 1-12 to 0-11
	int all_months = m_month - 1 + count;
	int years = all_months / 12;
	if (years > 0)
		add_year(years);
	m_month = all_months % 12 + 1; //back to 1-12
}

//ADD DAY
void Date::add_day(int count)
{
	int month_length = 0;
	int days_this_month = 0;

	while (count != 0) 
	{
		month_length = days_in_month(m_year, m_month);
		days_this_month = month_length - m_day;

		if (days_this_month - count < 0) 
		{
			add_month(1);
			m_day = 0;
			count -= days_this_month;
		}
		else 
		{
			m_day += count;
			break;
		}
	}
}

int Date::days_in_month(int year, int mm) 
{
	switch (mm)
	{
	case 2: //Feb
		if (is_leapyear(year)) { return 29; } else { return 28; }
	case 4: case 6: case 9: case 11: //Apr Jun Sep Nov
		return 30;
	default:
		return 31;
	}
}

bool Date::is_leapyear(int yyyy) 
{
	if (((yyyy % 4 == 0) && (yyyy % 100 != 0)) || (yyyy % 400 == 0)) return true;
	return false;
}
//END ADD DAY

void Date::add_hour(int count)
{
	int all_hours = m_hour + count;
	int days = all_hours / 24;
	if (days > 0)
		add_day(days);
	m_hour = all_hours % 24;
}

void Date::add_minute(int count)
{
	int all_minutes = m_minute + count;
	int hours = all_minutes / 60;
	if (hours > 0)
		add_hour(hours);
	m_minute = all_minutes % 60;
}

void Date::add_second(int count)
{
	int all_seconds = m_second + count;
	int minutes = all_seconds / 60;
	if (minutes > 0)
		add_minute(minutes);
	m_second = all_seconds % 60;
}
