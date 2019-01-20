#include "water_sensor.h"

#pragma once
#include <pigpio.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <functional>

Water_Sensor::Water_Sensor(int input_pin, int output_pin)
    :
    m_input_pin(input_pin),
	m_output_pin(output_pin)
{
    if (gpioInitialise() < 0)
    {
        std::cerr << "pigpio initialisation failed" << std::endl;
		exit(1);
	}

    /* Set GPIO modes */
    gpioSetMode(input_pin, PI_INPUT);
	gpioSetPullUpDown(input_pin, PI_PUD_DOWN);
	gpioSetMode(output_pin, PI_OUTPUT);
}

Water_Sensor::~Water_Sensor()
{
    /* Stop DMA, release resources */
    gpioTerminate();
}

//
bool Water_Sensor::is_wet()
{
	bool wet = false;
	gpioWrite(m_output_pin, 1); /* on */
	gpioSetPullUpDown(m_input_pin, PI_PUD_DOWN);
	if(gpioRead(m_input_pin) == 1)
	{
		std::cout << "Pin: " << m_input_pin << ", state: " <<  gpioRead(m_input_pin) << ", is wet!\n";
		wet = true;
	}
	else
	{
		std::cout << "Pin: " << m_input_pin << ", state: " <<  gpioRead(m_input_pin) << ", not wet\n";	
	}
	gpioWrite(m_output_pin, 0); /* off */
	return wet;
}
