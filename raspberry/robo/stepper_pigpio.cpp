
/*
stepper_pigpio.cpp

gcc -o stepper_pigpio stepper_pigpio.cpp -lpigpio -lrt -lpthread -lstdc++

sudo ./stepper_pigpio
*/


#include <pigpio.h>
#include <stdio.h>
#include <iostream>

int main()
{
	if (gpioInitialise() < 0)
	{
		fprintf(stderr, "pigpio initialisation failed\n");
		return 1;
	}

	/* Set GPIO modes */
	gpioSetMode(15, PI_OUTPUT);
	gpioSetMode(17, PI_OUTPUT);
	gpioSetMode(18, PI_OUTPUT);
	gpioSetMode(27, PI_OUTPUT);

	char ch = 'a';
	int rotations = 0;
	double delay_time = 0;
	do
	{
		std::cout << "rotations: \n";
		std::cin >> rotations;
		std::cout << "delay: \n";
		std::cin >> delay_time;


		//full rotation 32*64=2048 steps
		if (rotations > 0) { gpioWrite(15, 1); }  /* on */
		for (int i = 0; i < rotations; i++)
		{
			time_sleep(delay_time);
			gpioWrite(17, 1); /* on */
			gpioWrite(15, 0); /* off */
			time_sleep(delay_time);
			gpioWrite(18, 1); /* on */
			gpioWrite(17, 0); /* off */
			time_sleep(delay_time);
			gpioWrite(27, 1); /* on */
			gpioWrite(18, 0); /* off */
			time_sleep(delay_time);
			gpioWrite(15, 1); /* on */
			gpioWrite(27, 0); /* off */
			time_sleep(delay_time);
		}
		if (gpioRead(15) == 1) { digitalWrite(15, 0); } /* off */

		std::cout << "quit type: q\n";
		std::cin >> ch;
	} while (ch != 'q');

	/* Stop DMA, release resources */
	gpioTerminate();

	return 0;
}
