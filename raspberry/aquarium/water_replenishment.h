#pragma once
#include <chrono>
#include <vector>


#include "water_sensor.h"
#include "pump_control.h"

class Water_Replenishment
{
public:
        Water_Replenishment(int milliliter);

        int m_milliliter;
        
        void check_and_pump();
private:
    Pump pump;
    Water_Sensor sensor_1;
    Water_Sensor sensor_2;
};

