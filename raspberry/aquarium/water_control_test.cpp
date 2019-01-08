#include "water_replenishment.h"
#include <iostream>

int main()
{
    Water_Replenishment replenisher(std::chrono::system_clock::now(), 200);

    replenisher.start();

    std::cout << "end\n";
    
    char ch;
    std::cin >> ch;

}