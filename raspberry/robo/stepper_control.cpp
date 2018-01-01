//TODO save left right to member
//TODO handle negative angles

/*
stepper_pigpio.cpp

gcc -o stepper stepper_control.cpp -lpigpio -lrt -lpthread -lstdc++

sudo ./stepper_pigpio
*/


#include <pigpio.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>



class Stepper
{
public:
	enum Speed
	{
		SLOW,
		FAST
	};

	Stepper(int pin_1, int pin_2, int pin_3, int pin_4)
		:
		pins(4)
	{
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

	~Stepper()
	{
		/* Stop DMA, release resources */
		gpioTerminate();
	}

	void rotate(double angle, Speed speed = FAST)
	{
		double delay_time = 0.0025; //FAST
		if (speed == SLOW)
		{
			delay_time = 0.005;
		}


		//full rotation 32*64=2048 steps
		int steps = (abs(angle) / 360) * 2048;

		int left = 0, right = 1;
		for (int i = 0; i < steps; i++)
		{
			gpioWrite(pins[right++], 1); /* on */
			gpioWrite(pins[left++], 0); /* off */
			if (right == 4)
				right = 0;
			if (left == 4)
				left = 0;
			time_sleep(delay_time);
		}
		//for savety
		if (gpioRead(pins[0]) == 1) { gpioWrite(pins[0], 0); } /* off */
		if (gpioRead(pins[1]) == 1) { gpioWrite(pins[1], 0); } /* off */
		if (gpioRead(pins[2]) == 1) { gpioWrite(pins[2], 0); } /* off */
		if (gpioRead(pins[3]) == 1) { gpioWrite(pins[3], 0); } /* off */
	}

private:
	//holds the GPIO numbers 
	std::vector<int> pins;

	//saves the state of the last active GPIO pind / magnets (0 to 3)
	int m_left;
	int m_right;
};



int main()
{
	if (gpioInitialise() < 0)
	{
		fprintf(stderr, "pigpio initialisation failed\n");
		return 1;
	}
	Stepper stepper(15, 17, 18, 27);

	char ch = 'a';
	double angle = 0;
	do
	{
		std::cout << "angle: \n";
		std::cin >> angle;
	
		stepper.rotate(angle);

		std::cout << "quit type: q\n";
		std::cin >> ch;
	} while (ch != 'q');


	return 0;
}
