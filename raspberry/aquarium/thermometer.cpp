#include "thermometer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>


double NB_Sensors::Thermometer_DS18B20::get_temperature()
{
                std::ifstream thermo_file("/sys/bus/w1/devices/28-" + m_rom_id + "/w1_slave");

                if(!thermo_file)
                        std::cerr << "error: " << "can't open sensor" << std::endl
			<< "check /sys/bus/devices/ if thermometer is reachable" << std::endl;

                std::stringstream temp_data;
                std::string line;
                while(getline(thermo_file, line))
                        temp_data << line;

                std::regex pattern("(t=)(\\d+)|(-\\d+)");
                std::smatch result;
                std::string temp = temp_data.str();
                std::regex_search(temp, result, pattern);

                return std::stod(result[2].str()) / 1000;
}
