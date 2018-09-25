#pragma once
#include <chrono>
#include <vector>

class Stepper
{
public:
        Stepper(int pin_1, int pin_2, int pin_3, int pin_4);
        ~Stepper();

        //full rotation 32*64=2048 steps
        void rotate(int steps, std::chrono::milliseconds duration);
private:
        //holds the GPIO numbers
        std::vector<int> pins;
};

