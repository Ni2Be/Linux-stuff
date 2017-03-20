#include "HtW_Game.h"

using namespace N2B;

HtW_Game::HtW_Game(int lab_size, int difficulty)
	:	lab(lab_size, difficulty), 
		controller(Controller_state::INVALID) ,
		difficulty(difficulty)
{
	lab.player.bow_range = 3; //starting with range of 3
}

void HtW_Game::start()
{
	this->game_loop();
}

int HtW_Game::game_loop()
{
#ifdef WIN_SYSTEM
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#else
	HANDLE hConsole = N2B::HANDLE::NO_MEANING;
#endif //WIN_SYSTEM
	print_clear_console();
	bool game_running = true;
	std::cout << "You are a brave hero who came to this castle to hunt a\n"
		<< "giant Monster that lived here for hunderds of years.\n"
		<< "The Wumpus!\n"
		<< "There could be other dangerous things, so you should\n"
		<< "better sharpen your senses\n"
		<< "Your bow is ready and so are you.\n";
	print_castle();
	std::cout << "There are three doors at the front of the Castle\n";
	while (game_running)
	{
		//get connected rooms
		std::vector<N2B::Room*> connections = lab.get_room_connections(lab.player.position);

		print_what_is_near(hConsole, connections);
		print_change_color(hConsole, N2B::Color::GREY);
		std::cout << "you see the rooms:\n";
		print_change_color(hConsole, N2B::Color::WHITE);
		print_room_names(connections);


		while (controller == Controller_state::INVALID)
		{
			//ask what to do
			std::cin.clear();
			std::cout << "\nwhat do you want to do?\nshoot (s) / move (m)\n";
			controller = get_user_input();
			HtW_Event event_hadler;

			//Shot
			if (controller == Controller_state::SHOOT)
			{
				//generate shot
				print_clear_console();
				print_bow();
				std::vector<N2B::Room*> shot = build_shot();
				Shot_Outcome shot_outcome = lab.player.shoot(shot, difficulty);

				//what did happen
				if (shot_outcome == Shot_Outcome::HIT)
				{
					print_clear_console();
					print_hit();
					print_change_color(hConsole, N2B::Color::GREEN);
					std::cout << "\nYou hit the Wumpus!\n";
					print_change_color(hConsole, N2B::Color::WHITE);
					controller = Controller_state::WIN;
					break;
				}
				else if (shot_outcome == Shot_Outcome::MISS)
				{
					print_clear_console();
					print_change_color(hConsole, N2B::Color::GREY);
					std::cout << "\nYou missed!\n";
					print_change_color(hConsole, N2B::Color::WHITE);
				}
				else if (shot_outcome == Shot_Outcome::WAKED_THE_WUMPUS)
				{
					std::vector<N2B::Room*> wump_connections = lab.get_room_connections(lab.wump.position);
					event_hadler = lab.wump.move(get_random_room(wump_connections));
					print_clear_console();
					print_change_color(hConsole, N2B::Color::YELLOW);
					std::cout << "\nYou hear a monster moving!\n";
					print_change_color(hConsole, N2B::Color::WHITE);
				}
			}
			//Move
			else if (controller == Controller_state::MOVE)
			{
				int targeted_room = -1;
				while (targeted_room <= 0 || targeted_room > connections.size())
				{
					print_clear_console();
					print_doors();
					std::cout << "\nwhere do you want to move?\n";
					print_room_names(connections);
					std::cin >> targeted_room;
					std::cin.clear();
					if (targeted_room <= 0 || targeted_room > connections.size())
					{
						print_change_color(hConsole, N2B::Color::RED);
						std::cout << "\nnot possible\n";
						print_change_color(hConsole, N2B::Color::WHITE);
					}
					else
					{
						event_hadler = lab.player.move(connections[targeted_room - 1]);
						std::cout << "\nYou moved into " << lab.player.position->name << std::endl;
					}
				}
			}

			//Did somthing happen
			if (event_hadler == HtW_Event::FELL_IN_HOLE)
			{
				print_clear_console();
				print_hole();
				print_change_color(hConsole, N2B::Color::RED);
				std::cout << "\nYou fell in a hole!\n";
				print_change_color(hConsole, N2B::Color::WHITE);
				controller = Controller_state::LOSE;
			}
			else if (event_hadler == HtW_Event::GOT_EATEN)
			{
				print_clear_console();
				print_wumpus();
				print_change_color(hConsole, N2B::Color::RED);
				std::cout << "\nThe Wumpus ate you!\n";
				print_change_color(hConsole, N2B::Color::WHITE);
				controller = Controller_state::LOSE;
			}
			else if (event_hadler == HtW_Event::GRIPPED_BY_BAT)
			{
				//Move the player into a random room
				print_clear_console();
				print_bat();
				print_change_color(hConsole, N2B::Color::YELLOW);
				std::cout << "\nA giant Bat gipped you!\n"
					<< "You beat it until it drops you off\n";
				print_change_color(hConsole, N2B::Color::WHITE);
				Room* fly_to = lab.get_random_room();
				while (fly_to->has_hole || fly_to->actor != 0)
					fly_to = lab.get_random_room();
				lab.player.move(fly_to);
				std::cout << "\nYou moved into " << lab.player.position->name << std::endl;
			}
		}
		//the bat flys in a connected room
		std::vector<N2B::Room*> bat_connections = lab.get_room_connections(lab.bat.position);
		Room* fly_to = get_random_room(bat_connections);
		while (fly_to->actor != 0)
			fly_to = get_random_room(bat_connections);
		lab.bat.move(fly_to);

		//did the player win or lose?
		if (controller == Controller_state::WIN)
		{
			print_change_color(hConsole, N2B::Color::GREEN);
			std::cout << "You WON!\n";
			print_change_color(hConsole, N2B::Color::WHITE);
			break;
		}
		else if (controller == Controller_state::LOSE)
		{
			print_change_color(hConsole, N2B::Color::RED);
			std::cout << "You LOSE!\n";
			print_change_color(hConsole, N2B::Color::WHITE);
			break;
		}
		controller = Controller_state::INVALID;
	}
	return 0;
}

void HtW_Game::print_room_names(std::vector<N2B::Room*> rooms)
{
	for (int i = 0; i < rooms.size(); i++)
	{
		std::cout << rooms[i]->name << " (" << i + 1 << ") ";
		if (i < rooms.size() - 2)
			std::cout << ", ";
		else if (i == rooms.size() - 2)
		{
			std::cout << ", ";
		}
	}
	std::cout << std::endl;
}

std::vector<N2B::Room*> HtW_Game::build_shot()
{
	std::vector<N2B::Room*> shot;
	N2B::Room *arrow_position = lab.player.position;
	std::cout << "\nwhere do you want to shoot?\n";
	for (int i = 0; i < lab.player.bow_range; i++)
	{
		if (i == lab.player.bow_range - 1)
		{
			std::cout << "\nyour arrow can reach "
				<< lab.player.bow_range - i << " more room\n";
		}
		else if (i > 0)
		{
			std::cout << "\nyour arrow can reach "
				<< lab.player.bow_range - i << " more rooms\n";
		}
		std::cout << "\nenter 1, 2 or 3\n"; //TODO
		std::cout << "possible rooms:\n";
		
		std::vector<N2B::Room*> possible_rooms = lab.get_room_connections(arrow_position);
		print_room_names(possible_rooms);

		int targeted_room = 0;
		std::cin >> targeted_room;
		targeted_room -= 1;
		std::cin.clear();
		if (targeted_room < 0 || targeted_room >= possible_rooms.size())
		{
			std::cout << targeted_room << " is no valid target\n";
			i--;
		}
		else
		{
			shot.push_back(possible_rooms[targeted_room]);
			arrow_position = shot.back();
		}
	}
	return shot;
}

HtW_Game::Controller_state HtW_Game::get_user_input()
{
	Controller_state picked;
	char ch;
	std::cin >> ch;
	if (ch == 's')
		picked = Controller_state::SHOOT;
	else if (ch == 'm')
		picked = Controller_state::MOVE;
	else
		picked = Controller_state::INVALID;
	std::cin.clear();
	return picked;
}

Room* HtW_Game::get_random_room(std::vector<N2B::Room*> rooms)
{
	// Random seed
	std::random_device rd;
	// Initialize Mersenne Twister pseudo-random number generator
	std::mt19937 gen(rd());
	// Generate pseudo-random numbers
	// uniformly distributed in range (1, room_list.size() - 1)
	std::uniform_int_distribution<> dis(0, rooms.size() - 1);
	return rooms[dis(gen)];
}

void HtW_Game::print_clear_console()
{
#ifdef WIN_SYSTEM
	system("cls");
#endif //WIN_SYSTEM
}

void HtW_Game::print_change_color(HANDLE hConsole, N2B::Color c)
{
#ifdef WIN_SYSTEM
	switch (c)
	{
	case N2B::Color::BLUE:
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		break;
	case N2B::Color::RED:
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
		break;
	case N2B::Color::GREY:
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
		break;
	case N2B::Color::YELLOW:
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	case N2B::Color::GREEN:
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
		break;
	case N2B::Color::WHITE:
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	default:
		SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	}
#endif //WIN_SYSTEM
}

void HtW_Game::print_what_is_near(HANDLE hConsole, std::vector<N2B::Room*> rooms)
{
#ifdef WIN_SYSTEM
	for (auto connected_rooms : rooms)
	{
		if (connected_rooms->actor != 0)
		{
			if (connected_rooms->actor->what() == N2B::Actor_T::BAT)
			{
				print_change_color(hConsole, N2B::Color::YELLOW);
				std::cout << "You hear a Bat\n";
				print_change_color(hConsole, N2B::Color::WHITE);
			}
			else if (connected_rooms->actor->what() == N2B::Actor_T::WUMPUS)
			{
				print_change_color(hConsole, N2B::Color::GREY);
				std::cout << "You smell the Wumpus\n";
				print_change_color(hConsole, N2B::Color::WHITE);
			}
		}
		if (connected_rooms->has_hole)
		{
			print_change_color(hConsole, N2B::Color::BLUE);
			std::cout << "You feel a breeze\n";
			print_change_color(hConsole, N2B::Color::WHITE);
		}
	}
#else
	for (auto connected_rooms : rooms)
	{
		if (connected_rooms->actor != 0)
		{
			if (connected_rooms->actor->what() == N2B::Actor_T::BAT)
			{
				std::cout << "You hear a Bat\n";
			}
			else if (connected_rooms->actor->what() == N2B::Actor_T::WUMPUS)
			{
				std::cout << "You smell the Wumpus\n";
			}
		}
		if (connected_rooms->has_hole)
		{
			std::cout << "You feel a breeze\n";
		}
	}
#endif //WIN_SYSTEM
}

void HtW_Game::print_castle()
{
	std::cout
		<< "                                  |>>>\n"
		<< "                                  |\n"
		<< "                    |>>>      _  _|_  _         |>>>\n"
		<< "                    |        |;| |;| |;|        |\n"
		<< "                _  _|_  _    \\\\.    .  /    _  _|\n"
		<< "               |;|_|;|_|;|    \\\\:. ,  /    |;|_|;|_|;|\n"
		<< "               \\\\..      /    ||;   . |    \\\\.    .  /\n"
		<< "                \\\\.  ,  /     ||:  .  |     \\\\:  .  /\n"
		<< "                 ||:   |_   _ ||_ . _ | _   _||:   |\n"
		<< "                 ||:  .|||_|;|_|;|_|;|_|;|_|;||:.  |\n"
		<< "                 ||:   ||.    .     .      . ||:  .|\n"
		<< "                 ||: . || .     . .   .  ,   ||:   |       \\,/\n"
		<< "                 ||:   ||:  ,  _______   .   ||: , |            /`\\\n"
		<< "                 ||:   || .   /+++++++\\    . ||:   |\n"
		<< "                 ||:   ||.    |+++++++| .    ||: . |\n"
		<< "              __ ||: . ||: ,  |+++++++|.  . _||_   |\n"
		<< "     ____--`~    '--~~__|.    |+++++__|----~    ~`---,              ___\n"
		<< "-~--~                   ~---__|,--~'                  ~~----_____-~'   `~----~~\n";
}


void HtW_Game::print_wumpus()
{
	std::cout
		<< "	                                              ,--,  ,.-.\n"
		<< "                ,                   \\,       '-,-`,'-.' | ._\n"
		<< "               /|          \ \    ,   |\\         }  )/  / `-,',\n"
		<< "               [ '          |\\  /|   | |        /  \\|  |/`  ,`\n"
		<< "               | |       ,.`  `,` `, | |  _,...(   (      _',\n"
		<< "               \\  \\  __ ,-` `  ,  , `/ |,'      Y     (   \\_L\\\n"
		<< "                \\  \\_\\,``,   ` , ,  /  |         )         _,/\n"
		<< "                 \\  '  `  ,_ _`_,-,<._.<        /         /\n"
		<< "                  ', `>.,`  `  `   ,., |_      |         /\n"
		<< "                    \\/`  `,   `   ,`  | /__,.-`    _,   `\\\n"
		<< "                -,-..\\  _  \\  `  /  ,  / `._) _,-\\`       \\\n"
		<< "                 \\_,,.) /\\    ` /  / ) (-,, ``    ,        |\n"
		<< "                ,` )  | \\_\\       '-`  |  `(               \\\n"
		<< "               /  /```(   , --, ,' \\   |`<`    ,            |\n"
		<< "              /  /_,--`\\   <\\  V /> ,` )<_/)  | \\      _____)\n"
		<< "        ,-, ,`   `   (_,\\ \\    |   /) / __/  /   `----`\n"
		<< "       (-, \\           ) \\ ('_.-._)/ /,`    /\n"
		<< "       | /  `          `/ \\\\ V   V, /`     /\n"
		<< ",--\\(        ,     <_/`\\\\     ||      /\n"
		<< "   (   ,``-     \\/|         \\-A.A-`|     /\n"
		<< "  ,>,_ )_,..(    )\\          -,,_-`  _--`\n"
		<< "(_ \\|`   _,/_  /  \\_            ,--`\n"
		<< "\\( `   <.,../`     `-.._   _,-`\n"
		<< "`                      ```\n";
}

void HtW_Game::print_bat()
{
	std::cout
		<< "	....._      \n"
		<< " `.   ``-.                               .-----.._\n"
		<< "   `,     `-.                          .:      /`\n"
		<< "     :       `\"..                 .. - ``       :\n"
		<< "     /   ...--:::`n            n.`::...       :\n"
		<< "     `:``      .` ::          /  `.     ``---..:.\n"
		<< "       `\\    .`  ._:   .-:   ::    `.     .-``\n"
		<< "         :  :    :_\\\\_/: :  .::      `.  /\n"
		<< "         : /      \\-../:/_.`-`         \\ :\n"
		<< "         :: _.._  q` p ` /`             \\|\n"
		<< "         :-`    ``(_. ..-----hh``````/-._:\n"
		<< "                     `:      ``     /     `\n"
		<< "                     E:            /\n"
		<< "                     :          _/\n"
		<< "                      :    _..-``\n"
		<< "                      l--``\n";
}

void HtW_Game::print_doors()
{
	std::cout
	<< "     ______   	          ______  	      ______\n"
	<< "   ,-' ;  ! `-.        ,-' ;  ! `-.        ,-' ;  ! `-.\n"
	<< "  / :  !  :  . \\     / :  !  :  .  \\     / :  !  :  .  \\\n"
	<< " |_ ;   __:  ;  |    |_ ;   __:  ;  |    |_ ;   __:  ;  |\n"
	<< " )| .  :)(.  !  |    )| .  :)(.  !  |    )| .  :)(.  !  |\n"
	<< " |\"    (##)  _  |    |\"    ( #)  _  |    |\"    ( )  _   |\n"
	<< " |  :  ;`'  (_) (    |  :  ;`'  (_) (    |  :  ;`'  (*) (\n"
	<< " |  :  :  .     |    |  :  :  .     |    |  :  :  .     |\n"
	<< " )_ !  ,  ;  ;  |    )_ !  ,  ;  ;  |    )_ !  ,  ;  ;  |\n"
	<< " || .  .  :  :  |    || .  .  :  :  |    || .  .  :  :  |\n"
	<< " |\" .  |  :   . |    |\" .  |  :  .  |    |\" .  |  :  .  |\n"
	<< " |_____;----.___|    |___;----._____|    |_____;----.___|\n";
}

void HtW_Game::print_hole()
{
	std::cout
		<< ".,,,,,I777777777777777777777777777777777777777..............|\n"
		<< "..,,.?777777777.I777777777+.??77777=.7777777777.............|\n"
		<< "..,.77777777777777,~==7.....????~~,,777777777777,...........|\n"
		<< ".,.7777777777+:~===:==+.....+???~~.777777777777777..........|\n"
		<< ".,777777777777777==,~=+.....??+:~,~++=~,:,,,.777777.........|\n"
		<< ",7I7777777?,,,:~~~=,:~=.....+++~:.++=~,,,.7777777777........|\n"
		<< ".77777777~...,::~~,=,:~,....+++::.++=~,.~777777777777.......|\n"
		<< ".77777777777~.,::::~.:~~....===:,,+=~,.==~:,.+77777777......|\n"
		<< ".777777=:~===~.,:::,~.,:....~~=,.=~~:..~=I7777777777777~....|\n"
		<< ".777777=,:~~=~,.,:,::.,,....:,,,.~~:.++77777777777777777:...|\n"
		<< ".77777777777+:~..,,,.,...........,...+=+++777777777777777I,.|\n"
		<< ".7777777...::~::....................,==++===~7777777777777..|\n"
		<< ".77777777777?:,:,...................~+====~~~::~+777777777..|\n"
		<< ".77777777777777777.................,:~~~~~~:::,.,...777777..|\n"
		<< ".7777777777777777~:............=777777777??+:........,.I77..|\n"
		<< ".777777777777777:::............=~7777777777777777777777777..|\n"
		<< ".77777777777777.,,::....,77=..::~~:77777777777777777777777..|\n"
		<< ".7777777777777?,,,,:~7777777777.::,,7 77777777777777777777..|\n"
		<< ".77777777777II..+77777777777777777~..~ 7777777777777777777..|\n"
		<< ".I777777777777777777777777777777777777=7777777777777777777..|\n"
		<< ".IIIII7777777777777777777777777777777777777777777777777777..|\n";
}

void HtW_Game::print_hit()
{
	std::cout
		<< "	                                              ,--,  ,.-.\n"
		<< "                ,                   \\,       '-,-`,'-.' | ._\n"
		<< "               /|          \ \    ,   |\\         }  )/  / `-,',\n"
		<< "               [ '          |\\  /|   | |        /  \\|  |/`  ,`\n"
		<< "               | |       ,.`  `,` `, | |  _,...(   (      _',\n"
		<< "               \\  \\  __ ,-` `  ,  , `/ |,'      Y     (   \\_L\\\n"
		<< "                \\  \\_\\,``,   ` , ,  /  |         )         _,/\n"
		<< "                 \\  '  `  ,_ _`_,-,<._.<        /         /\n"
		<< "                  ', `>.,`  `  `   ,., |_      |         /      ,,////\n"
		<< "  _.-\"/ ____________ \\/`  `,   `   ,`  | ___,__________,_____////////\n"
		<< "  `'-.\\~~~~~~~~~~~~  -,-..\\  _  \\  `  /~~~~~~~~~~~~~~~~~~~~~~\\\\\\\\\\\\\\\\\n"
		<< "                 \\_,,.) /\\    ` /  / ) (-,, ``    ,        |    \'\'\\\\\\\\\n"
		<< "                ,` )  | \\O\\       'O`  |  `(               \\\n"
		<< "               /  /```(   , --, ,' \\   |`<`    ,            |\n"
		<< "              /  /_,--`\\   <\\  V /> ,` )<_/)  | \\      _____)\n"
		<< "        ,-, ,`   `   (_,\\ \\    |   /) / __/  /   `----`\n"
		<< "       (-, \\           ) \\ ('_.-._)/ /,`    /\n"
		<< "       | /  `          `/ \\\\   _  , /`     /\n"
		<< ",--\\(        ,     <_/`\\\\    (O)      /\n"
		<< "   (   ,``-     \\/|         \\- . -`|     /\n"
		<< "  ,>,_ )_,..(    )\\          -,,_-`  _--`\n"
		<< "(_ \\|`   _,/_  /  \\_            ,--`\n"
		<< "\\( `   <.,../`     `-.._   _,-`\n"
		<< "`                      ```\n";
}

void HtW_Game::print_bow()
{
	std::cout
		<< "          4$$-.                          \n"
		<< "           4   \".\n"
		<< "           4    ^.                                       \n"
		<< "           4     $                                       \n"
		<< "           4     'b                                      \n"
		<< "           4      \"b.\n"
		<< "           4        $                                    \n"
		<< "           4        $r                                   \n"
		<< "           4        $F                                   \n"
		<< "-$b========4========$b====*P=-                           \n"
		<< "           4       *$$F                                  \n"
		<< "           4        $$\"\n"
		<< "           4       .$F                                   \n"
		<< "           4       dP                                    \n"
		<< "           4      F                                      \n"
		<< "           4     @                                       \n"
		<< "           4    .                                        \n"
		<< "           J.                                            \n"
		<< "          '$$     '\n";
}