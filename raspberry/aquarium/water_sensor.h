#pragma once
#include <chrono>
#include <vector>

class Water_Sensor
{
public:
    Water_Sensor(int input_pin, int output_pin);
    ~Water_Sensor();

    //
    bool is_wet();
private:
    //holds the GPIO numbers
    int m_input_pin;
	int m_output_pin;
};

