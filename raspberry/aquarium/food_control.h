#pragma once
#include <chrono>
#include <vector>

class Food_Control
{
public:
        Food_Control(int input_pin, int output_pin);
        ~Food_Control();

        //
        static void rotate();
	static void stop_rotation();
private:
        //holds the GPIO numbers
        static int m_input_pin;
	static int m_output_pin;
};

static void stop_rotation_cb(int, int, uint32_t, void*);
