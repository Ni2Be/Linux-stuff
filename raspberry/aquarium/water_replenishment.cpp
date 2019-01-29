#include "water_replenishment.h"
#include <iostream>

Water_Replenishment::Water_Replenishment(int milliliter)
    :
    m_milliliter(milliliter),
    sensor_1(5, 6),
    sensor_2(12, 13),
    pump(4)
{

}

void Water_Replenishment::check_and_pump()
{
    std::cout << "checking..\n";
    bool is_wet_s1 = sensor_1.is_wet();
    bool is_wet_s2 = sensor_2.is_wet(); 
    if (is_wet_s2 || is_wet_s2)
    {
        std::cout << "at least one sensor is wet, doing nothing\n";
    }
    else
    {   
        std::cout << "sensors not wet, start pumping\n";
        //500ml/55s 1ml/0.11s
        pump.pump_for(std::chrono::milliseconds(110 * m_milliliter));
    }
}