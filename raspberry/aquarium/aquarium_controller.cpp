#include "aquarium_controller.h"

#include <thread>

#include "date.h"


Aquarium_Controller::Aquarium_Controller()
    :
    food_control(19, 16),
    stepper(9, 25, 11, 8),
    replenisher(200),
    thermometer("0217c1401cff")
{
}

Aquarium_Controller::~Aquarium_Controller()
{
}

void Aquarium_Controller::start()
{
    //ensure that the moon led is off
    start_moonset();
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

        Date plan_today(std::chrono::system_clock::now());
        
        //7:00 MOONSET
        plan_today.set_hour(7);
        plan_today.set_minute(0);
        plan_today.set_second(0);
        Date time_now(std::chrono::system_clock::now());
        if (time_now.hour() < plan_today.hour())
        {
            //wait for moonset
            std::this_thread::sleep_until(plan_today.get_time_point());
            start_moonset();
        }
        
        //9:00 WATER REPLENTISHMENT
        plan_today.set_hour(9);
        plan_today.set_minute(0);
        plan_today.set_second(0);
        time_now = Date(std::chrono::system_clock::now());
        if (time_now.hour() < plan_today.hour())
        {
            //wait for Water Replenishment
            std::this_thread::sleep_until(plan_today.get_time_point());
            start_water_replenishment();
        }

        //13:00 FEEDING
        plan_today.set_hour(13);
        plan_today.set_minute(0);
        plan_today.set_second(0);
        time_now = Date(std::chrono::system_clock::now());
        if (time_now.hour() < plan_today.hour())
        {
            //wait for Feeding
            std::this_thread::sleep_until(plan_today.get_time_point());
            start_feeding();
        }

        //16:00 WATER REPLENTISHMENT
        plan_today.set_hour(16);
        plan_today.set_minute(0);
        plan_today.set_second(0);
        time_now = Date(std::chrono::system_clock::now());
        if (time_now.hour() < plan_today.hour())
        {
            //wait for Water Replenishment
            std::this_thread::sleep_until(plan_today.get_time_point());
            start_water_replenishment();
        }

        
        //21:00 MOONRISE
        plan_today.set_hour(21);
        plan_today.set_minute(0);
        plan_today.set_second(0);
        Date time_now(std::chrono::system_clock::now());
        if (time_now.hour() < plan_today.hour())
        {
            //wait for moonrise
            std::this_thread::sleep_until(plan_today.get_time_point());
            start_moonrise();
        }

        //next day 00:01 START SLEEPMODE
        plan_today.set_hour(0);
        plan_today.set_minute(1);
        plan_today.set_second(0);
        plan_today.add_day(1);
        Date time_now(std::chrono::system_clock::now());
        if (time_now.hour() < plan_today.hour())
        {
            //wait for tomorrow
            std::this_thread::sleep_until(plan_today.get_time_point());
        }

    }
}

void Aquarium_Controller::start_moonrise()
{
    stepper.rotate(2048, std::chrono::milliseconds(std::chrono::minutes(30)));
}

void Aquarium_Controller::start_moonset()
{
    stepper.rotate(-2048, std::chrono::milliseconds(std::chrono::minutes(30)));
}

void Aquarium_Controller::start_water_replenishment()
{
    replenisher.check_and_pump();
}

void Aquarium_Controller::log_temperature()
{
    //TODO
    std::cout << thermometer.get_temperature();
}

void Aquarium_Controller::start_feeding()
{
	food_control.rotate();
}