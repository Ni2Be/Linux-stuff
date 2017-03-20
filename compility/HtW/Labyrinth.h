#pragma once
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <fstream>
#include <functional> 
#include <cctype>
#include <locale>

#ifdef _DEBUG
#define DEBUG
#endif // _DEBUG

namespace N2B
{
	class Actor;
	
	/**
	@see N2B::Actor::what()
	*/
	enum Actor_T
	{
		ACTOR,
		BAT,
		PLAYER,
		WUMPUS
	};

	/**
	@see N2B::Player::shoot()
	*/
	enum Shot_Outcome
	{
		HIT,
		MISS,
		WAKED_THE_WUMPUS
	};

	/**
	@see N2B::Actor::move()
	*/
	enum HtW_Event
	{
		NOTHING,
		GRIPPED_BY_BAT,
		FELL_IN_HOLE,
		GOT_EATEN
	};

	class Room
	{
		friend class Labyrinth;
		friend class Player;
	public:
		//the name of the room
		std::string name;
		//if the room has a hole this will be true
		bool has_hole;
		//if an actor is inside, this will point to him
		Actor *actor;
	private:
		//rooms that are connected with this room
		std::vector<Room*> connected_rooms;
	};

	/**
	An Actor is everything that can move through the labyrinth

	@see N2B::Player
	@see N2B::Bat
	@see N2B::Wumpus
	*/
	class Actor
	{
		friend class Labyrinth;
		friend class Room;
	public:
		/**
		returns the type of the Actor

		@param none
		@return Actor_T
		*/
		virtual Actor_T what() { return ACTOR; }

		/**
		moves the Actor into an other room, returns
		an event that can accure if two actors meet, or
		the player fells into a hole

		@param Room* to move in
		@return Event
		*/
		virtual HtW_Event move(Room* room);
		Room *position;
	};

	/**
	A player has a Bow with a range and can shoot with it

	@see N2B::Actor
	@see N2B::Player::shoot()
	*/
	class Player : public Actor
	{
		friend class Labyrinth;
	private:
		Actor_T what() { return PLAYER; };
	public:
		int bow_range; //range of the bow, 3 would mean 3 Rooms
		Shot_Outcome shoot(std::vector<Room*> shoot_through, int difficulty);
	};

	/**
	A Bat is an actor whoms what() function returns BAT

	@see N2B::Actor
	*/
	class Bat : public Actor
	{
		friend class Labyrinth;
	private:
		Actor_T what() { return BAT; };
	};

	/**
	A Wumpus is an actor whoms what() function returns WUMPUS

	@see N2B::Actor
	*/
	class Wumpus : public Actor
	{
		friend class Labyrinth;
	public:
		HtW_Event move(Room* room);
	private:
		Actor_T what() { return WUMPUS; };
	};

	class Labyrinth
	{
	public:
		//The player of the game
		Player player;
		//the Wumpulus
		Wumpus wump;
		//The Giant Bat
		Bat bat;

		/**
		Generates a new labyrinth, loads room names from rooms.txt
		and prepares the labyrinth with actors and holes

		@param int size, int difficulty
		@see Labyrinth::load_room_names()
		@see Labyrinth::generate_wing()
		@see Labyrinth::merge_wings()
		@see Labyrinth::prepare_labyrinth()
		*/
		Labyrinth(int size, int difficulty);

		/**
		deletes the Labyrinth

		@param none
		@return none
		*/
		~Labyrinth();

		/**
		prints the labyrinths current state to screen

		@param none
		@return none
		*/
		void print();

		/**
		returns the rooms that are connected with the given room

		@param Room*
		@return vector<Room*> connections
		*/
		std::vector<Room*> get_room_connections(const Room* room);

		/**
		returns a random room of the labyrinth

		@param none
		@return Room* to a room of room_list
		*/
		Room* get_random_room();
	private:
		//difficulty has influence on the number of holes and the chance
		//to wake the wumpus
		int difficulty;
		//matrix of the room connections
		std::vector<std::vector<Room*>> labyrinth;
		//list of the rooms for better access
		std::vector<Room*> room_list;
		//list of possible room names
		std::vector<std::string> room_names;
		// Random seed
		std::random_device rd;

		/**
		generates 4 new rooms connected with each other
		represented by a vector<vector> of pointers like:
		0 * * *
		* 0 * *
		* * 0 *
		* * * 0

		@param none
		@return a new wing of 4 rooms connected with each other
		*/
		std::vector<std::vector<Room*>> generate_wing();

		/**
		merges two wings to one by randomly picking two rooms of wing,
		two rooms of wing two and coonect them:
		input:
		w1
		0 * * *
		* 0 * *
		* * 0 *
		* * * 0
		w2
		0 * * *
		* 0 * 0
		* * 0 *
		* 0 * 0

		possible output (w1 picked rooms 1 and 4, w2 picked rooms 2 and 4):
		0 * * 0 0 * 0 0
		* 0 * * 0 0 0 0
		* * 0 * 0 0 0 0
		0 * * 0 0 0 0 *
		0 0 0 0 0 * * *
		* 0 0 0 * 0 * 0
		0 0 0 0 * * 0 *
		0 0 0 * * 0 * 0

		@param two wings
		@return a new wing consisting of the two input wings connected with each other
		*/
		std::vector<std::vector<Room*>> merge_wings(const std::vector<std::vector<Room*>>& w1,
			const std::vector<std::vector<Room*>>& w2);

		/**
		randomly picks two rooms of wing that are connected:
		input:
		wing
		0 1 2 3
		0 * * * 0
		* 0 * * 1
		* * 0 * 2
		* * * 0 3

		possible out come:
		row = 2
		column = 1
		would mean: room 2 and 3

		@param two int references that will contain the out come and the wing
		@return none
		*/
		void pick_room(int &row, int &column, const std::vector<std::vector<Room*>>& wing);

		/**
		randomly picks rooms that will contain holes, the bat and
		the Wumpus. The player allways starts in room 1.

		@param none
		@return none
		*/
		void prepare_labyrinth();

		/**
		adds the connected rooms to every rooms connected vector

		@param none
		@return none
		*/
		void connect_rooms();

		/**
		places the actors of the game in different random rooms

		@param none
		@return none
		*/
		void place_actors();

		/**
		places as many holes as the difficulty level is 
		in different random rooms

		@param none
		@return none
		*/
		void place_holes();

		/**
		loads possible room names from the rooms.txt

		@param none
		@return vector<string> of room names
		*/
		std::vector<std::string> load_room_names();

		/**
		returns a randomly from the room_names vector picked string
		erases that string from room_names

		@param none
		@return string name
		*/
		std::string get_room_name();
	};

	// trim from start (in place) should be replaced by boost
	static inline void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	}

	// trim from end (in place) should be replaced by boost
	static inline void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	}
}
