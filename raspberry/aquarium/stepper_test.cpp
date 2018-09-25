#include <iostream>
#include <chrono>

#include "stepper_control.h"

int main()
{
        Stepper stepper(9, 25, 11, 8);

        char ch = 'a';
        int steps = 0;
        do
        {
                std::cout << "steps (2048steps == 360deg): ";
                std::cin >> steps;
                int duration_input;
                std::cout << "\nhow many millisec should it take: ";
                std::cin >> duration_input;
                std::chrono::milliseconds duration(duration_input);

                stepper.rotate(steps, duration);

                std::cout << "quit type: q\n";
                std::cin >> ch;
        } while (ch != 'q');


        return 0;
}

