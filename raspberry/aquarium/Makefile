#Makefile
CXX=g++
CXXFLAGS=-Wall -std=c++17 -pthread -lpigpio -lrt
RM=rm -f

stepper_test: stepper_control.o stepper_test.cpp
	$(CXX) $(CXXFLAGS) -o stepper_test stepper_control.o stepper_test.cpp

stepper_control.o: stepper_control.cpp stepper_control.h
	$(CXX) -c  $(CXXFLAGS) -o stepper_control.o stepper_control.cpp

thermo_test: thermometer.o thermo_test.cpp
	$(CXX) $(CXXFLAGS) -o thermo_test thermometer.o thermo_test.cpp

thermometer.o: thermometer.cpp thermometer.h
	$(CXX) -c $(CXXFLAGS) -o thermometer.o thermometer.cpp

food_test: food_control.o food_test.cpp
	$(CXX) $(CXXFLAGS) -o food_test food_control.o food_test.cpp

food_control: food_control.cpp food_control.h
	$(CXX) -c $(CXXFLAGS) -o food_control.o food_control.cpp

clean:
	$(RM) thermo_test thermometer.o stepper_test stepper_control.o food_control.o food_test
