#include <iostream>
#include <wiringPi.h>

#include <stdio.h>

int main()
{
    if (wiringPiSetup() == -1)
        return 1;

    //GPIO 17 == WPin 0
    pinMode(0, OUTPUT);

    for (int i = 0; i < 10; i++)
    {
        digitalWrite(0, 1);
        std::cout << "on\n";
        delay(100);

        digitalWrite(0, 0);
        std::cout << "off\n";
        delay(100);
    }
}
