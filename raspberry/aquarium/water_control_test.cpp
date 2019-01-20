#include "water_replenishment.h"
#include <iostream>

int main()
{
    Water_Replenishment replenisher(200);

    replenisher.check_and_pump();

    std::cout << "end\n";
    
    char ch;
    std::cin >> ch;

    replenisher.check_and_pump();
}