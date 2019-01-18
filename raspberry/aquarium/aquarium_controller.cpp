#include "aquarium_controller.h"
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>


Aquarium_Controller::Aquarium_Controller()
    :
    food_control(19, 16)
{
    //TODO should ensure that the moon light is off
}

Aquarium_Controller::~Aquarium_Controller()
{

}

void Aquarium_Controller::start()
{
    while(true)
    {
        //TODO read the instructions stream 

        //TODO read tokens 
        /*
        TOKEN:
        F = Feed
        R = moonRise
        S = moonSet
        W = water replenishment
        T = log temperature
        W# = wait for # minutes 
        */



        std::tm tm = {};
        std::stringstream ss("Jan 9 2014 12:35:34");
        ss >> std::get_time(&tm, "%b %d %Y %H:%M:%S");
        auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

       //wait for moonrise
       std::this_thread::sleep_until(std::chrono::system_clock::time_point());

    }
}

void Aquarium_Controller::start_moonrise()
{

}

void Aquarium_Controller::start_moonset()
{

}

void Aquarium_Controller::start_water_replenishment()
{

}

void Aquarium_Controller::log_temperature()
{

}

void Aquarium_Controller::start_feeding()
{
	food_control.rotate();
}