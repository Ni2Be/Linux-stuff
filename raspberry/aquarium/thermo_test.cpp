#include <iostream>

#include "thermometer.h"


int main ()
{
	std::cout << "start" << std::endl;

	NB_Sensors::Thermometer_DS18B20 thermometer("0217c1401cff");

	std::cout << "temperature: " << thermometer.get_temperature() << "°C" << std::endl;

	std::cout << "end" << std::endl;
}
