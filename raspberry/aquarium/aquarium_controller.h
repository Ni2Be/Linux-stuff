#include "food_control.h"
#include "stepper_control.h"
#include "water_replenishment.h"
#include "thermometer.h"

class Aquarium_Controller
{
public:
    Aquarium_Controller();
    ~Aquarium_Controller();

    void start();
private:
    Food_Control food_control;
    Stepper stepper;
    Water_Replenishment replenisher;
	NB_Sensors::Thermometer_DS18B20 thermometer;

    void start_moonrise();
    void start_moonset();

    void start_water_replenishment();

    void log_temperature();

    void start_feeding();
};
