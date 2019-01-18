#include "water_replenishment.h"
#include <iostream>

Water_Replenishment::Water_Replenishment(int milliliter)
    :
    m_milliliter(milliliter),
    sensor_1(5, 6),
    sensor_2(12, 13),
    pump(26)
{

}

void Water_Replenishment::check_and_pump()
{
    std::cout << "checking..\n";
    if (!sensor_1.is_wet() && !sensor_2.is_wet())
    {
        std::cout << "sensors not wet, start pumping\n";
        pump.pump_for(std::chrono::milliseconds(1000));
    }
    else
    {
        std::cout << "sensors wet, doing nothing\n";
    }
}