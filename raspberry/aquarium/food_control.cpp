#include "food_control.h"

#pragma once
#include <pigpio.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <functional>

Food_Control::Food_Control(int input_pin, int output_pin)
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
	gpioSetMode(output_pin, PI_OUTPUT);
}

Food_Control::~Food_Control()
{
        /* Stop DMA, release resources */
        gpioTerminate();
}

//
void Food_Control::rotate()
{
	gpioSetAlertFunc(m_input_pin, NULL);
	gpioWrite(m_output_pin, 1); /* on */
	double initial_wait = 2.0; //wait one secound so that the stop singnal doesn't triggers to early
	time_sleep(initial_wait);
	gpioSetAlertFuncEx(m_input_pin, stop_rotation_cb, this);
}

void stop_rotation_cb(int gpio, int level, uint32_t tick, void* food_control)
{
	if (level == 0) // change to low
		static_cast<Food_Control*>(food_control)->stop_rotation();
}

void Food_Control::stop_rotation()
{
	gpioWrite(m_output_pin, 0); /* off */
}

