#pragma once
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <ctime>
#include <string>


class Date
{
public:
	Date();
	Date(int year, int month, int day, int hour, int minute, int second);
	Date(std::chrono::system_clock::time_point date);
	Date(std::string date = "Mon Jan 00 00:00:00 0000");

	std::string get_date_string(); 
	std::chrono::system_clock::time_point get_time_point();

	int& year()   { return m_year; }
	int& month()  { return m_month; }
	int& day()    { return m_day; }
	int& hour()   { return m_hour; }
	int& minute() { return m_minute; }
	int& second() { return m_second; }
private:
	int m_year;
	int m_month;
	int m_day;
	int m_hour;
	int m_minute;
	int m_second;

	void read_string(std::string date);

	std::chrono::system_clock::time_point to_time();
	std::chrono::system_clock::time_point to_time(std::string date);
	std::string time_to_string(std::chrono::system_clock::time_point date);


	void change_year(std::string& date, int year);
	void change_day(std::string& date, int day);
	void change_hour(std::string& date, int hour);
	void change_minute(std::string& date, int minute);
	void change_second(std::string& date, int second);
};

