#include "water_replenishment.h"
#include <iostream>

Water_Replenishment::Water_Replenishment(std::chrono::system_clock::time_point daytime, int milliliter)
    :
    m_daytime(daytime),
    m_milliliter(milliliter),
    sensor_1(5, 6),
    sensor_2(12, 13),
    pump(26)
{

}

void Water_Replenishment::start()
{
    if (sensor_1.is_wet() || sensor_2.is_wet())
    {
        std::cout << "is_wet\n";
        pump.pump_for(std::chrono::milliseconds(1000));
    }
}