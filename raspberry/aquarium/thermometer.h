#pragma once
#include <string>

namespace NB_Sensors
{
	class Thermometer_DS18B20
	{
	public:
		Thermometer_DS18B20(std::string rom_id)
			:
			m_rom_id(rom_id)
			{}

		double get_temperature();
	private:
		std::string m_rom_id;
	};
}

