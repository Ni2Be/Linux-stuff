#include "pump_control.h"

#pragma once
#include <pigpio.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <chrono>


Pump::Pump(int pin)
        :
        m_pin(pin)
{
    if (gpioInitialise() < 0)
    {
           std::cerr << "pigpio initialisation failed" << std::endl;
	    exit(1);
    }

    /* Set GPIO mode */
    gpioSetMode(m_pin, PI_OUTPUT);
}

Pump::~Pump()
{
    /* Stop DMA, release resources */
    gpioTerminate();
}

//
void Pump::pump_for(std::chrono::milliseconds duration)
{
	double delay_time = static_cast<double>(duration.count()) / 1000.0f;

	gpioWrite(m_pin, 1); /* on */
    std::cout << "pump on\n";
    time_sleep(delay_time);
    std::cout << "pump off\n";
    if(gpioWrite(m_pin, 0))
        std::cout << "pin: " << m_pin << " off\n";
    /* off */
}
