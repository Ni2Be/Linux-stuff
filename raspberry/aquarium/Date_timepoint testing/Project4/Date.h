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
	//CONSTRUCTOR

	//all member will be initiated as 0s
	Date();
	//creates a Date out of int
	Date(int year, int month, int day, int hour, int minute, int second);
	//creates a Date out of a time_point
	Date(std::chrono::system_clock::time_point date);
	/*
	creates a Date object out of a string formated like
	"Mon Jan 00 00:00:00 0000"
	*/
	Date(std::string date = "Mon Jan 00 00:00:00 0000");

	//PUBLIC FUNCTIONS

	//returns the date as a string formated like "Mon Jan 00 00:00:00 0000"
	std::string get_date_string();
	//returns the date as a time_point
	std::chrono::system_clock::time_point get_time_point();


	//GET/SET

	void add_year(int count = 1);
	void add_month(int count = 1);
	void add_day(int count = 1);
	void add_hour(int count = 1);
	void add_minute(int count = 1);
	void add_second(int count = 1);

	void set_year(int year)  { m_year = year; };
	void set_month(int mon)  { m_month = mon; };
	void set_day(int day)    { m_day = day; };
	void set_hour(int hour)  { m_hour = hour; };
	void set_minute(int min) { m_minute = min; };
	void set_second(int sec) { m_second = sec; };


	int year()   const { return m_year; }
	int month()  const { return m_month; }
	int day()    const { return m_day; }
	int hour()   const { return m_hour; }
	int minute() const { return m_minute; }
	int second() const { return m_second; }
private:
	//MEMBER

	int m_year;
	int m_month;
	int m_day;
	int m_hour;
	int m_minute;
	int m_second;


	//PRIVATE FUNCTIONS

	/*
	reads a stting formated like:
	"Mon Jan 00 00:00:00 0000"
	and sets the Date values
	*/
	void read_string(std::string date);

	//converts Date to time_point
	std::chrono::system_clock::time_point to_time();
	//converts string to time_point
	std::chrono::system_clock::time_point to_time(std::string date);
	//converts time_point to string
	std::string time_to_string(std::chrono::system_clock::time_point date);

	//sets the year in a string formated like: "Mon Jan 00 00:00:00 0000"
	void change_year(std::string& date, int year);
	//sets the month in a string formated like: "Mon Jan 00 00:00:00 0000"
	void change_month(std::string& date, int month);
	//sets the day in a string formated like: "Mon Jan 00 00:00:00 0000"
	void change_day(std::string& date, int day);
	//sets the hour in a string formated like: "Mon Jan 00 00:00:00 0000"
	void change_hour(std::string& date, int hour);
	//sets the minute in a string formated like: "Mon Jan 00 00:00:00 0000"
	void change_minute(std::string& date, int minute);
	//sets the second in a string formated like: "Mon Jan 00 00:00:00 0000"
	void change_second(std::string& date, int second);

	//ADD_DAY HELPER
	// returns the amount of the days in the month
	int days_in_month(int year, int mm);
	// true if leapyear
	bool is_leapyear(int yyyy);


	/*
	converts a string formated like "Jan" to an int like 1
	returns -1 for an unknown value
	*/
	int month_to_int(std::string month_str);
	/*
	converts an int to
	a string formated like "Jan" 
	returns "" for an unknown value
	*/
	std::string int_to_month(int month);
};

