#include <iostream>
#include <wiringPi.h>
#include <stdio.h>

int main()
{
	if (wiringPiSetup() == -1)
	{
		return 1;
	}
	//GPIO 15 == WPin 16
	//GPIO 17 == WPin 0
	//GPIO 18 == WPin 1
	//GPIO 27 == WPin 2
	pinMode(16, OUTPUT);
	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(2, OUTPUT);

	char ch = 'a';
	int rotations = 0;
	int delay_time = 0;
	do
	{
		std::cout << "rotations: \n";
		std::cin >> rotations;
		std::cout << "delay: \n";
		std::cin >> delay_time;
		for (int i = 0; i < rotations; i++)
		{
			if (i == 0)
			{digitalWrite(16, 1);}
			delay(delay_time);
			digitalWrite(0, 1);
			digitalWrite(16, 0);
			delay(delay_time);
			digitalWrite(1, 1);
			digitalWrite(0, 0);
			delay(delay_time);
			digitalWrite(2, 1);
			digitalWrite(1, 0);
			delay(delay_time);
			digitalWrite(16, 1);
			digitalWrite(2, 0);
			if (i == rotations - 1)
			{digitalWrite(16, 0);}
		}

		std::cout << "quit type: q\n"; 
		std::cin >> ch;
	} while (ch != 'q');
}
