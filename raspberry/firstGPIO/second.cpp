#include <iostream>
#include <wiringPi.h>

#include <stdio.h>

void input();

int main()
{
    if (wiringPiSetup() == -1)
        return 1;

    //GPIO 17 == WPin 0
    pinMode(0, OUTPUT);

    pinMode(5, INPUT);
    
    bool clicked = false;
    for (int i = 0; i < 100; i++)
    {
        if (digitalRead(5) == 1)
            clicked = true;
        else
            clicked = false;
        if (clicked)
        {
            digitalWrite(0, 1);
            std::cout << "on\n";
        }   
        else
        {
            digitalWrite(0, 0);
            std::cout << "off\n";
            delay(100);
        }
        delay(100);
    }
    pinMode(5, OUTPUT);
    digitalWrite(0, 0);
    digitalWrite(5, 0);
}
