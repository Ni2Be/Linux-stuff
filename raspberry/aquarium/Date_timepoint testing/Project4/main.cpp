#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <ctime>
#include <string>

#include "Date.h"


int main()
{
	Date date(std::chrono::system_clock::now());

	std::cout << date.get_date_string();
	
	//date.add_second(10);
	//std::this_thread::sleep_until(date.get_time_point());
	std::this_thread::sleep_for(std::chrono::seconds(2));
	date = Date(std::chrono::system_clock::now());

	std::cout << date.get_date_string();

	char ch;
	std::cin >> ch;
}