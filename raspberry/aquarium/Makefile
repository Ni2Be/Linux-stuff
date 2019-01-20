#Makefile
CXX=g++
CXXFLAGS=-Wall -std=c++17 -pthread -lpigpio -lrt
RM=rm -f

#AQUARIUM
aquarium: main.cpp aquarium_controller.cpp aquarium_controller.h
	$(CXX) $(CXXFLAGS) -o aquarium main.cpp aquarium_controller.cpp date.cpp food_control.cpp stepper_control.cpp thermometer.cpp water_replenishment.cpp water_sensor.cpp pump_control.cpp

#MAIN
aquarium_main: aquarium_controller.o main.cpp
	$(CXX) $(CXXFLAGS) -o aquarium_main aquarium_controller.o food_control.o thermometer.o stepper_control.o pump_control.o water_sensor.o water_replenishment.o main.cpp

#CONTROLLER
aquarium_controller: date.o food_control.o thermometer.o stepper_control.o pump_control.o water_sensor.o water_replenishment.o aquarium_controller.cpp aquarium_controller.h
	$(CXX) -c $(CXXFLAGS) -o aquarium_controller.o date.o food_control.o thermometer.o stepper_control.o pump_control.o water_sensor.o water_replenishment.o aquarium_controller.cpp

#DATE
date: date.cpp date.h
	$(CXX) -c $(CXXFLAGS) -o date.o date.cpp

#STEPPER MOON CONTROL
stepper_test: stepper_control.o stepper_test.cpp
	$(CXX) $(CXXFLAGS) -o stepper_test stepper_control.o stepper_test.cpp

stepper_control: stepper_control.cpp stepper_control.h
	$(CXX) -c  $(CXXFLAGS) -o stepper_control.o stepper_control.cpp

#THERMOMETER
thermo_test: thermometer.o thermo_test.cpp
	$(CXX) $(CXXFLAGS) -o thermo_test thermometer.o thermo_test.cpp

thermometer: thermometer.cpp thermometer.h
	$(CXX) -c $(CXXFLAGS) -o thermometer.o thermometer.cpp

#FOOD CONTROL
food_test: food_control.o food_test.cpp
	$(CXX) $(CXXFLAGS) -o food_test food_control.o food_test.cpp

food_control: food_control.cpp food_control.h
	$(CXX) -c $(CXXFLAGS) -o food_control.o food_control.cpp

#WATER REPLENISHMENT
water_control_test: water_replenishment.o water_control_test.cpp
	$(CXX) $(CXXFLAGS) -o water_control_test pump_control.o water_sensor.o water_replenishment.o water_control_test.cpp

water_replenishment: water_sensor.o pump_control.o water_replenishment.cpp water_replenishment.h
	$(CXX) -c $(CXXFLAGS) -o water_replenishment.o water_sensor.o pump_control.o water_replenishment.cpp

water_sensor: water_sensor.cpp water_sensor.h
	$(CXX) -c $(CXXFLAGS) -o water_sensor.o water_sensor.cpp

pump_control: pump_control.cpp pump_control.h
	$(CXX) -c $(CXXFLAGS) -o pump_control.o pump_control.cpp


clean:
	$(RM) thermo_test thermometer.o stepper_test stepper_control.o food_control.o food_test water_sensor.o water_sensor pump_control.o pump_control water_replenishment.o water_replenishment water_control_test 
