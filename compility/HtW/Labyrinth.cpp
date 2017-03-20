#include "Labyrinth.h"

using namespace N2B;

Labyrinth::Labyrinth(int size, int difficulty)
 {
	if (size < 2 || size > 10) size = 3;
	if (difficulty > size) difficulty = size;
	this->difficulty = difficulty;
	room_names = load_room_names();
	labyrinth = generate_wing();
	for (int i = 0; i < size; i++)
	{
		std::vector<std::vector<Room*>> wing;
		wing = generate_wing();
		labyrinth = merge_wings(labyrinth, wing);
	}
	prepare_labyrinth();
#ifdef DEBUG
	std::cout << "DEBUG:\n";
	this->print();
	std::cout << "END DEBUG\n";
#endif //DEBUG
}

Labyrinth::~Labyrinth()
{
	for (auto room : room_list)
	{
		delete room;
	}
}

std::vector<Room*> Labyrinth::get_room_connections(const Room* room)
{
	std::vector<Room*> connections(room->connected_rooms);
	return connections;
}

Room* Labyrinth::get_random_room()
{
	// Initialize Mersenne Twister pseudo-random number generator
	std::mt19937 gen(rd());
	// Generate pseudo-random numbers
	// uniformly distributed in range (1, room_list.size() - 1)
	std::uniform_int_distribution<> dis(0, room_list.size() - 1);
	return room_list[dis(gen)];
}

std::vector<std::vector<Room*>> Labyrinth::generate_wing()
{
	std::vector<std::vector<Room*>> wing;

	for (int i = 0; i < 4; i++)
	{
		Room* room = new Room();
		room->name = get_room_name();
		//std::stringstream ss; old
		//ss << room_list.size(); old
		//room->name = ss.str(); old
		room_list.push_back(room);
	}

	for (int i = 0; i < 4; i++)
	{
		std::vector<Room*> row;
		for (int n = 0; n < 4; n++)
		{
			if (n != i)
			{
				row.push_back(room_list[room_list.size() - 4 + n]);
			}
			else
			{
				row.push_back(0);
			}
		}
		wing.push_back(row);
	}
	return wing;
}

std::vector<std::vector<Room*>> Labyrinth::merge_wings(const std::vector<std::vector<Room*>>& w1,
	const std::vector<std::vector<Room*>>& w2)
{
	int w1_row = 0, w1_column = 0, w2_row = 0, w2_column = 0;
	pick_room(w1_row, w1_column, w1);
	pick_room(w2_row, w2_column, w2);
	std::vector<std::vector<Room*>> wing;

	//creates a new wing consisting of the two input wings:
	// w1 0
	// 0  w2

	for (int i = 0; i < w1.size() + w2.size(); i++)
	{
		std::vector<Room*> row;
		for (int n = 0; n < w1.size() + w2.size(); n++)
		{
			if (i < w1.size())
			{
				if (n < w1.size())
				{
					row.push_back(w1[i][n]);
				}
				else
				{
					row.push_back(0);
				}
			}
			else
			{
				if (n < w1.size())
				{
					row.push_back(0);
				}
				else
				{
					row.push_back(w2[i - w1.size()][n - w1.size()]);
				}
			}
		}
		wing.push_back(row);
	}
	//connecting the two wings
	//connect the wings
	wing[w1_row][w1.size() + w2_column] = wing[w1.size() + w2_row][w1.size() + w2_column];
	wing[w1_column][w1.size() + w2_row] = wing[w1.size() + w2_column][w1.size() + w2_row];
	wing[w1.size() + w2_row][w1_column] = wing[w1_row][w1_column];
	wing[w1.size() + w2_column][w1_row] = wing[w1_column][w1_row];
	//get rid of the old connections
	wing[w1_row][w1_column] = 0;
	wing[w1_column][w1_row] = 0;
	wing[w1.size() + w2_row][w1.size() + w2_column] = 0;
	wing[w1.size() + w2_column][w1.size() + w2_row] = 0;

	return wing;
}

void Labyrinth::pick_room(int &row, int &column, const std::vector<std::vector<Room*>>& wing)
{
	// Initialize Mersenne Twister pseudo-random number generator
	std::mt19937 gen(rd());
	// Generate pseudo-random numbers
	// uniformly distributed in range (0, wing.size() - 1)
	std::uniform_int_distribution<> dis(0, wing.size() - 1);
	//pick row
	row = dis(gen);
	column = dis(gen);
	while (wing[row][column] == 0)
	{
		column = dis(gen);
	}
}

void Labyrinth::print()
{
	std::cout << "  ";
	for (int i = 0; i < labyrinth.size(); i++)
	{
		if (i < 10)
			std::cout << i << ' ';
		else
			std::cout << i % 10 << ' ';
	}
	std::cout << std::endl;
	for (int i = 0; i < labyrinth.size(); i++)
	{
		std::cout << i % 10 << ' ';
		for (int n = 0; n < labyrinth.size(); n++)
		{
			if (labyrinth[i][n] != 0)
			{
				char symbol = '*';
				if (labyrinth[i][n]->actor != 0)
				{
					if (labyrinth[i][n]->actor->what() == Actor_T::BAT) symbol = '}';
					else if (labyrinth[i][n]->actor->what() == Actor_T::PLAYER) symbol = 'P';
					else if (labyrinth[i][n]->actor->what() == Actor_T::WUMPUS) symbol = 'W';
				}
				if (labyrinth[i][n]->has_hole) symbol = 'O';
				std::cout << symbol << ' ';
			}
			else
			{
				std::cout << "- ";
			}
		}
		std::cout << std::endl;
	}
}

void Labyrinth::prepare_labyrinth()
{
	connect_rooms();
	place_actors();
	place_holes();
}

void Labyrinth::connect_rooms() 
{
	int rr = 0;
	for (int i = 0; i < labyrinth.size(); i++)
	{
		for (int n = 0; n < labyrinth[i].size(); n++)
		{
			if (labyrinth[i][n] != 0)
			{
#ifdef DEBUG
				std::cout << "DEBUG:\n";
				std::cout << rr++ << std::endl << room_list.size() << std::endl;
				std::cout << "   room: " << labyrinth[n][i] << std::endl;
				std::cout << "to room: " << labyrinth[i][n] << std::endl;
				std::cout << "END DEBUG\n";
#endif //DEBUG
				labyrinth[n][i]->connected_rooms.push_back(labyrinth[i][n]);
			}
		}
	}
}

void Labyrinth::place_actors()
{
	// Initialize Mersenne Twister pseudo-random number generator
	std::mt19937 gen(rd());
	// Generate pseudo-random numbers
	// uniformly distributed in range (1, room_list.size() - 1)
	std::uniform_int_distribution<> dis(1, room_list.size() - 1);

	//place player 

	room_list[0]->actor = &(this->player);
	player.position = room_list[0];

	//place wumpus 
	int wump_room = dis(gen);
	room_list[wump_room]->actor = &(this->wump);
	wump.position = room_list[wump_room];

	//place bat
	int bat_room = dis(gen);
	while (room_list[bat_room]->actor != 0)
	{
		bat_room = dis(gen);
	}
	room_list[bat_room]->actor = &(this->bat);
	bat.position = room_list[bat_room];
}

void Labyrinth::place_holes()
{
	// Initialize Mersenne Twister pseudo-random number generator
	std::mt19937 gen(rd());
	// Generate pseudo-random numbers
	// uniformly distributed in range (1, room_list.size() - 1)
	std::uniform_int_distribution<> dis(1, room_list.size() - 1);

	//places holes in rooms that have no actor inside
	for (int i = 0; i < difficulty; i++)
	{
		int rand_room = dis(gen);
		if ((room_list[rand_room]->actor != 0) ||
			room_list[rand_room]->has_hole)
		{
			i--;
		}
		else
		{
			room_list[rand_room]->has_hole = true;
		}
	}
}

Shot_Outcome Player::shoot(std::vector<Room*> shoot_through, int difficulty)
{
	for (auto room : shoot_through)
	{
		if (room->actor != 0 && room->actor->what() == Actor_T::WUMPUS)
			return Shot_Outcome::HIT;
	}
	// Random seed
	std::random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	std::mt19937 gen(rd());
	// Generate pseudo-random numbers
	// uniformly distributed in range (0, difficulty)
	std::uniform_int_distribution<> dis(0, difficulty);
	if (dis(gen) > 0)
		return Shot_Outcome::WAKED_THE_WUMPUS;
	return Shot_Outcome::MISS;
}

HtW_Event Actor::move(Room* room)
{
	if (room->actor != 0)
	{
		if (room->actor->what() == Actor_T::WUMPUS)
			return HtW_Event::GOT_EATEN;
		if (room->actor->what() == Actor_T::BAT)
			return HtW_Event::GRIPPED_BY_BAT;
	}
	else if (room->has_hole)
	{
		return HtW_Event::FELL_IN_HOLE;
	}
	else
	{
		Room* prev_Room = this->position;
		this->position = room;
		prev_Room->actor = 0;
		room->actor = this;
	}
	return HtW_Event::NOTHING;
}

HtW_Event Wumpus::move(Room* room)
{
	if (room->actor != 0)
	{
		if (room->actor->what() == Actor_T::PLAYER)
			return HtW_Event::GOT_EATEN;
	}
	else
	{
		Room* prev_Room = this->position;
		this->position = room;
		prev_Room->actor = 0;
		room->actor = this;
	}
	return HtW_Event::NOTHING;
}

std::vector<std::string> Labyrinth::load_room_names() {
	std::string inFile = "rooms.txt";
	std::ifstream istr(inFile.c_str());

	if (!istr) throw std::runtime_error(
		"N2B::Labyrinth::load_room_names()\nCould not open File");

	std::vector<std::string> room_names;
	while (!istr.eof())
	{
		char line[100];
		istr.getline(line, 100);
		std::string name(line);
		N2B::ltrim(name);
		N2B::rtrim(name);
		room_names.push_back(name);
#ifdef DEBUG
		std::cout << "*" << name << "*\n";
#endif //DEBUG
	}
	return room_names;
}


std::string Labyrinth::get_room_name()
{
	// Initialize Mersenne Twister pseudo-random number generator
	std::mt19937 gen(rd());
	// Generate pseudo-random numbers
	// uniformly distributed in range (0, room_names.size() - 1)
	std::uniform_int_distribution<> dis(0, room_names.size() - 1);

	int rand_num = dis(gen);
	std::string name = room_names[rand_num];
	room_names.erase(room_names.begin() + rand_num);
	return name;
}
