#pragma once
#include <chrono>
#include <vector>


#include "water_sensor.h"
#include "pump_control.h"

class Water_Replenishment
{
public:
        Water_Replenishment(std::chrono::system_clock::time_point daytime, int milliliter);

        std::chrono::system_clock::time_point m_daytime;
        int m_milliliter;
        
        void start();

private:
    Pump pump;
    Water_Sensor sensor_1;
    Water_Sensor sensor_2;
};

