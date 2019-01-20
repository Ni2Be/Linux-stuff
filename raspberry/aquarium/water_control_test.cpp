#include "water_replenishment.h"
#include <iostream>

#include <pigpio.h>

int main()
{
    if (gpioInitialise() < 0)
    {
        std::cerr << "pigpio initialisation failed" << std::endl;
		exit(1);
	}

    Water_Replenishment replenisher(200);

    replenisher.check_and_pump();



    std::cout << "end\n";
    char ch;
    std::cin >> ch;

    gpioTerminate();
    std::cout << "terminated\n";
}