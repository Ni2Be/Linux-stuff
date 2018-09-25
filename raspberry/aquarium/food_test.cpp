#include <iostream>

#include "food_control.h"


int main ()
{
	std::cout << "start" << std::endl;

	Food_Control food_control(19, 16);

	std::cout << "start rotation.." << std::endl;

	food_control.rotate();

	std::cout << "waiting.." << std::endl;

	std::cout << "enter key: ";

	char ch;
	std::cin >> ch;

	std::cout << std::endl << "end" << std::endl;

}
