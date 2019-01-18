#include "food_control.h"

class Aquarium_Controller
{
public:
    Aquarium_Controller();
    ~Aquarium_Controller();

    void start();
private:
    Food_Control food_control;


    void start_moonrise();
    void start_moonset();

    void start_water_replenishment();

    void log_temperature();

    void start_feeding();
}