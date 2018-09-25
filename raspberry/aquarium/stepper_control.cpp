#include "stepper_control.h"

#pragma once
#include <pigpio.h>
#include <iostream>
#include <vector>
#include <chrono>


Stepper::Stepper(int pin_1, int pin_2, int pin_3, int pin_4)
        :
        pins(4)
{
        if (gpioInitialise() < 0)
        {
                std::cerr << "pigpio initialisation failed" << std::endl;
		exit(1);
        }


        pins[0] = pin_1;
        pins[1] = pin_2;
        pins[2] = pin_3;
        pins[3] = pin_4;
        /* Set GPIO modes */
        gpioSetMode(pins[0], PI_OUTPUT);
	gpioSetMode(pins[1], PI_OUTPUT);
	gpioSetMode(pins[2], PI_OUTPUT);
	gpioSetMode(pins[3], PI_OUTPUT);
}

Stepper::~Stepper()
{
        /* Stop DMA, release resources */
        gpioTerminate();
}

//full rotation 32*64=2048 steps
void Stepper::rotate(int steps, std::chrono::milliseconds duration)
{
	bool rotate_clockwise = (steps > 0);
	steps = abs(steps);
        double delay_time = static_cast<double>(duration.count()) / static_cast<double>(steps) / 1000.0;
        double mechanical_movement_duration = 0.003; //time the magnets need to pull the gearing
        if (delay_time < mechanical_movement_duration)
        {
                std::cerr << "to fast: would skip over some steps and as a result the angle could differ";
                exit(1);
        }
        delay_time -= mechanical_movement_duration;
	int left = 0, right = 1;
	if (rotate_clockwise)
	{
	        for (int i = 0; i < steps; i++)
        	{
			gpioWrite(pins[left], 1); /* on */
        	        gpioWrite(pins[right++], 1); /* on  */
			gpioWrite(pins[left++], 0); /* off */
                	time_sleep(mechanical_movement_duration);
                	if (right == 4)
        	                right = 0;
			if (left == 4)
				left = 0;
                	gpioWrite(pins[left], 0);  /* off */
        	        time_sleep(delay_time);
	        }
	}
	else
	{
		for (int i = 0; i < steps; i++)
                {
                        gpioWrite(pins[right], 1); /* on */
                        gpioWrite(pins[left--], 1); /* on  */
                        gpioWrite(pins[right--], 0); /* off */
                        time_sleep(mechanical_movement_duration);
                        if (right == -1)
                                right = 3;
                        if (left == -1)
                                left = 3;
                        gpioWrite(pins[right], 0);  /* off */
                        time_sleep(delay_time);
                }

	}
        //for savety
        if (gpioRead(pins[0]) == 1) { gpioWrite(pins[0], 0); } /* off */
        if (gpioRead(pins[1]) == 1) { gpioWrite(pins[1], 0); } /* off */
        if (gpioRead(pins[2]) == 1) { gpioWrite(pins[2], 0); } /* off */
        if (gpioRead(pins[3]) == 1) { gpioWrite(pins[3], 0); } /* off */
}
