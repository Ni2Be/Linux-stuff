#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <ctime>
#include <string>

#include "Date.h"


std::chrono::system_clock::time_point string_to_time(std::string date)
{
	std::tm tm = {};
	std::stringstream ss;
	ss << date;
	
	//"Thu Jan  9 12:35:34 2014"
	ss >> std::get_time(&tm, "%a %b  %d %H:%M:%S %Y");
	auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
	return tp;
}

std::string time_to_string(std::chrono::system_clock::time_point date)
{
	struct tm buf;
	char dateString[26];
	time_t time = std::chrono::system_clock::to_time_t(date);
	localtime_s(&buf, &time);
	asctime_s(dateString, 26, &buf);
	return dateString;
}

std::string set_day(std::string date, int day)
{
	if (log10(day) < 1)
		date.replace(9, 1, std::to_string(day));
	else
		date.replace(8, 2, std::to_string(day));

	return date;
}

std::string set_hour(std::string date, int hour)
{
	if (log10(hour) < 1)
		date.replace(11, 2, "0" + std::to_string(hour));
	else
		date.replace(11, 2, std::to_string(hour));

	return date;
}

std::string set_minute(std::string date, int minute)
{
	if (log10(minute) < 1)
		date.replace(14, 2, "0" + std::to_string(minute));
	else
		date.replace(14, 2, std::to_string(minute));

	return date;
}


int main()
{
	//auto date_string = time_to_string(std::chrono::system_clock::now());

	//std::cout << "before convert: " << date_string << std::endl;

	////set day
	//date_string = set_minute(date_string, 12);
	//auto date_time = string_to_time(date_string);
	//std::cout << "replace: " << date_string << std::endl;

	//std::cout << "sleeping.." << std::endl;
	//
	//std::this_thread::sleep_until(date_time);

	//std::cout << "wakeup" << std::endl;

	//date_string = time_to_string(date_time);

	//std::cout << "after convert: " << date_string << std::endl;


	Date date(std::chrono::system_clock::now());

	std::cout << date.day() << "." << date.month() << "." << date.year() << ", " << date.hour() << ":" << date.minute() << date.second() << std::endl;

	std::cout << date.get_date_string();

	char ch;
	std::cin >> ch;
}