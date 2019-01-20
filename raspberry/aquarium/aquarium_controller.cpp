#include "aquarium_controller.h"

#include <thread>

#include "date.h"


Aquarium_Controller::Aquarium_Controller()
    :
    food_control(19, 16),
    stepper(9, 25, 11, 8),
    replenisher(500),
    thermometer("0217c1401cff")
{
}

Aquarium_Controller::~Aquarium_Controller()
{
}

void Aquarium_Controller::start()
{
    //ensure that the moon led is off
    stepper.rotate(-2048, std::chrono::milliseconds(std::chrono::minutes(1)));
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
            std::cout << "waiting for moonset\n";
            std::this_thread::sleep_until(plan_today.get_time_point());
            start_moonset();
        }
        else
        {
            std::cout << "skiped moonset\n";
        }
        
        //9:00 WATER REPLENTISHMENT
        plan_today.set_hour(9);
        plan_today.set_minute(0);
        plan_today.set_second(0);
        time_now = Date(std::chrono::system_clock::now());
        if (time_now.hour() < plan_today.hour())
        {
            //wait for Water Replenishment
            std::cout << "waiting for replenishment\n";
            std::this_thread::sleep_until(plan_today.get_time_point());
            start_water_replenishment();
        }
        else
        {
            std::cout << "skiped replenishment\n";
        }

        //13:00 FEEDING
        plan_today.set_hour(13);
        plan_today.set_minute(0);
        plan_today.set_second(0);
        time_now = Date(std::chrono::system_clock::now());
        if (time_now.hour() < plan_today.hour())
        {
            //wait for Feeding
            std::cout << "waiting for feeding\n";
            std::this_thread::sleep_until(plan_today.get_time_point());
            start_feeding();
        }
        else
        {
            std::cout << "skiped feeding\n";
        }

        //16:00 WATER REPLENTISHMENT
        plan_today.set_hour(16);
        plan_today.set_minute(0);
        plan_today.set_second(0);
        time_now = Date(std::chrono::system_clock::now());
        if (time_now.hour() < plan_today.hour())
        {
            //wait for Water Replenishment
            std::cout << "waiting for replenishment\n";
            std::this_thread::sleep_until(plan_today.get_time_point());
            start_water_replenishment();
        }
        else
        {
            std::cout << "skiped replenishment\n";
        }

        
        //21:00 MOONRISE
        plan_today.set_hour(21);
        plan_today.set_minute(0);
        plan_today.set_second(0);
        time_now = Date(std::chrono::system_clock::now());
        if (time_now.hour() < plan_today.hour())
        {
            //wait for moonrise
            std::cout << "waiting for moonrise\n";
            std::this_thread::sleep_until(plan_today.get_time_point());
            start_moonrise();
        }
        else
        {
            std::cout << "skiped moonrise\n";
        }

        //next day 00:01 START SLEEPMODE
        plan_today.set_hour(0);
        plan_today.set_minute(1);
        plan_today.set_second(0);
        plan_today.add_day(1);
        time_now = Date(std::chrono::system_clock::now());
        
        std::cout << "waiting for tomorrow\n";
        //wait for tomorrow
        std::this_thread::sleep_until(plan_today.get_time_point());
    }
}

void Aquarium_Controller::start_moonrise()
{
    std::cout << "moonrise\n";
    stepper.rotate(2048, std::chrono::milliseconds(std::chrono::minutes(30)));
    std::cout << "end moonrise\n";
}

void Aquarium_Controller::start_moonset()
{
    std::cout << "moonset\n";
    stepper.rotate(-2048, std::chrono::milliseconds(std::chrono::minutes(30)));
    std::cout << "end moonset\n";
}

void Aquarium_Controller::start_water_replenishment()
{
    std::cout << "check and pump\n";
    replenisher.check_and_pump();
    std::cout << "end check and pump\n";
}

void Aquarium_Controller::log_temperature()
{
    //TODO
    std::cout << "log temp\n";
    std::cout << thermometer.get_temperature();
}

void Aquarium_Controller::start_feeding()
{
    std::cout << "feeding\n";
	food_control.rotate();
    std::cout << "end feeding\n";
}