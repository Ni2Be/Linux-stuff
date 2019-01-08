#pragma once
#include <chrono>
#include <vector>

class Pump
{
public:
        Pump(int pin);
        ~Pump();

        //full rotation 32*64=2048 steps
        void pump_for(std::chrono::milliseconds duration);
private:
        //holds the GPIO numbers
        int m_pin;
};

