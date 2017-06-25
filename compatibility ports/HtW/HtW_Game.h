#pragma once
#include "Labyrinth.h"

#ifdef _WIN32
#define WIN_SYSTEM
#include <Windows.h>
#include <stdlib.h>
#endif
namespace N2B
{
#ifndef WIN_SYSTEM
	enum HANDLE
	{
		NO_MEANING
	};
#endif // !WIN_SYSTEM

	enum Color
	{
		WHITE,
		GREY,
		RED,
		BLUE,
		YELLOW,
		GREEN
	};

	/**
	A Hunt the Wumps game contains a Labyrinth and
	is responsible for the user interaction

	@see N2B::Labyrinth()
	@see N2B::HtW_Game::start()
	*/
	class HtW_Game
	{
	public:
		/**
		HtW_Game generates a Labyrinth

		@see N2B::Labyrinth()
		*/
		HtW_Game(int lab_size, int difficulty);
		
		/**
		start starts the game_loop

		@param none
		@return none
		@see game_loop()
		*/
		void start();
	private:
		//controls of the game_loop()
		enum Controller_state
		{
			INVALID,
			SHOOT,
			MOVE,
			WIN,
			LOSE
		};

		//@see N2B::Labyrinth::difficulty
		int difficulty;

		//labyrinth where the game takes place
		Labyrinth lab;

		//current state of the game in game_loop
		Controller_state controller;

		/**
		The game loop with all the user interaction and
		the rules like when the whumpus ate you the game is over

		@param none
		@return none
		@see N2B::HtW_Game::print_what_is_near()
		@see N2B::HtW_Game::print_room_names()
		@see N2B::HtW_Game::build_shot()
		@see N2B::HtW_Game::get_user_input()
		@see N2B::HtW_Game::get_random_room()
		*/
		int game_loop();

		/**
		prints the room names of a given vector of rooms

		@param vector of room*
		@return none
		*/
		void print_room_names(std::vector<N2B::Room*> rooms);

		/**
		asks the user through which rooms he want to shoot and
		builds a vector of room* out of it

		@param none
		@return vector<Room*> rooms that will be shot though
		*/
		std::vector<N2B::Room*> build_shot();
		
		/**
		reads input of chars and transformes them into Controller_states

		@param none
		@return Controller_state
		*/
		Controller_state get_user_input();

		/**
		returns a random Room* of a given Room* vector

		@param vector of Room*
		@return Room*
		*/
		Room* get_random_room(std::vector<N2B::Room*> rooms);


#ifdef WIN_SYSTEM
		void print_change_color(HANDLE hConsole, N2B::Color c);
#else //just to remember there will another function be compiled
		void print_change_color(HANDLE hConsole, N2B::Color c);
#endif //WIN_SYSTEM
		void print_clear_console();

		/**
		prints a hint what could be inside the roomes
		of a given vector of room*

		@param vector of room*
		@return none
		*/
#ifdef WIN_SYSTEM
		void print_what_is_near(HANDLE hConsole, std::vector<N2B::Room*> rooms);
#else //just to remember there will another function be compiled
		void print_what_is_near(HANDLE hConsole, std::vector<N2B::Room*> rooms);
#endif //WIN_SYSTEM
		void print_castle();
		void print_wumpus();
		void print_bat();
		void print_doors();
		void print_hole();
		void print_hit();
		void print_bow();
	};
}
