#include "aquarium_controller.h"

#include <thread>
#include <iostream>

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

        using namespace date;
	    using namespace std::chrono;

	    auto today = floor<days>(system_clock::now());

        std::cout << "-------------------\n" << today << "\n";
        std::cout << "New Day: " << system_clock::now() << "\n";
        
        //7:00 MOONSET
        auto moon_set_time = today + 7h;
        auto time_now = system_clock::now();
        std::cout << time_now << "\n";
        if (time_now < moon_set_time)
        {
            //wait for moonset
            std::cout << "waiting for moonset\n";
            std::this_thread::sleep_until(moon_set_time);
            start_moonset();
        }
        else
        {
            std::cout << "skiped moonset\n";
        }
        
        //9:00 WATER REPLENTISHMENT
        auto water_rep_time_0 = today + 9h;
        time_now = system_clock::now();
        std::cout << time_now << "\n";
        if (time_now < water_rep_time_0)
        {
            //wait for Water Replenishment
            std::cout << "waiting for replenishment\n";
            std::this_thread::sleep_until(water_rep_time_0);
            start_water_replenishment();
        }
        else
        {
            std::cout << "skiped replenishment\n";
        }

        //13:00 FEEDING
        auto feeding_time_0 = today + 13h;
        time_now = system_clock::now();
        std::cout << time_now << "\n";
        if (time_now < feeding_time_0)
        {
            //wait for Feeding
            std::cout << "waiting for feeding\n";
            std::this_thread::sleep_until(feeding_time_0);
            start_feeding();
        }
        else
        {
            std::cout << "skiped feeding\n";
        }
        
        //16:00 WATER REPLENTISHMENT
        auto water_rep_time_1 = today + 16h;
        time_now = system_clock::now();
        std::cout << time_now << "\n";
        if (time_now < water_rep_time_1)
        {
            //wait for Water Replenishment
            std::cout << "waiting for replenishment\n";
            std::this_thread::sleep_until(water_rep_time_1);
            start_water_replenishment();
        }
        else
        {
            std::cout << "skiped replenishment\n";
        }

        
        //21:00 MOONRISE
        auto moonrise_time = today + 16h;
        time_now = system_clock::now();
        std::cout << time_now << "\n";
        if (time_now < moonrise_time)
        {
            //wait for moonrise
            std::cout << "waiting for moonrise\n";
            std::this_thread::sleep_until(moonrise_time);
            start_moonrise();
        }
        else
        {
            std::cout << "skiped moonrise\n";
        }

        //next day 00:01 START SLEEPMODE
        auto next_day_time = today + days(1);
        time_now = system_clock::now();
        std::cout << time_now << "\n";
        std::cout << "waiting for tomorrow\n";
        //wait for tomorrow
        std::this_thread::sleep_until(next_day_time);
    }
}

void Aquarium_Controller::start_moonrise()
{ 
        using namespace date;
	    using namespace std::chrono;
    std::cout << system_clock::now() << " moonrise\n";
    stepper.rotate(2048, std::chrono::milliseconds(std::chrono::minutes(30)));
    std::cout << system_clock::now() << " end moonrise\n";
}

void Aquarium_Controller::start_moonset()
{
        using namespace date;
	    using namespace std::chrono;
    std::cout << system_clock::now() << " moonset\n";
    stepper.rotate(-2048, std::chrono::milliseconds(std::chrono::minutes(30)));
    std::cout << system_clock::now() << " end moonset\n";
}

void Aquarium_Controller::start_water_replenishment()
{
        using namespace date;
	    using namespace std::chrono;
    std::cout << system_clock::now() << " check and pump\n";
    replenisher.check_and_pump();
    std::cout << system_clock::now() << " end check and pump\n";
}

void Aquarium_Controller::log_temperature()
{
        using namespace date;
	    using namespace std::chrono;
    //TODO
    std::cout << system_clock::now() << " log temp\n";
    std::cout << thermometer.get_temperature();
}

void Aquarium_Controller::start_feeding()
{
        using namespace date;
	    using namespace std::chrono;
    std::cout << system_clock::now() << " feeding\n";
	food_control.rotate();
    std::cout << "end feeding\n";
}