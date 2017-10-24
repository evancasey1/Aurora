#include <iostream>
#include <string>
#include "CursorUtilities.h"
#include "Player.h"
#include "Human.h"
#include "Dwarf.h"
#include "Elf.h"

Player::Player()
{
	setName("Adventurer");
	setRace("Undefined");
}
Player::Player(std::string race, std::string name) 
{
	setName(name);
	setRace(race);
}

void Player::userCreatePlayer(int command_input_cursor_position)
{
	//TODO:
	// 	Name selection
	//	Attribute selection
	//  break into 3 helper functions
	//		get name, get race, get attrs
	setAbsoluteCursorPosition(command_input_cursor_position, 0);

	std::string input_str;
	std::string race;
	std::string name;
	std::cout << "(1) Human\n(2) Dwarf\n(3) Elf\n";
	std::cout << "Select your race:\n>> ";

	//while player race is Undefined (default value) keep prompting
	while (!this->getRace().compare("Undefined")) {
		std::cin >> input_str;
		for(int i = 0; input_str[i]; i++) {
			input_str[i] = tolower(input_str[i]);
		}
		if (!input_str.compare("1") || !input_str.compare("human")
			|| !input_str.compare("2") || !input_str.compare("dwarf")
			|| !input_str.compare("3") || !input_str.compare("elf")) {

			this->setRace(input_str);
		}
		else if (!input_str.compare("quit") || !input_str.compare("exit")) {
			exit(1);
		}
		else {
			//Invalid option. Erase to end of line and 
			//move up a row to prevent scrolling of terminal
			eraseLines(1);
			std::cout << ">> ";
		}
	}
	input_str = "";
	std::cout << "What is your name?\n>> ";
	while(!input_str.compare("")) {
		std::cin >> input_str;
		for(int i = 0; input_str[i]; i++) {
			if (!isalpha(input_str[i]) && !isblank(input_str[i])) {
				input_str = "";
				std::cout << "Your name must only be alphabetical.\r";
				//move cursor up by 1
				moveCursor(1, 0, 0, 0);
				std::cout << ">> \033[K";
				break;
			}
		}
	}
	this->setName(input_str);
}

std::string Player::getName() 
{
	return this->name;
}
void Player::setName(std::string n) 
{
	this->name = n;
}
std::string Player::getRace() 
{
	return this->race;
}
void Player::setRace(std::string r) 
{
	this->race = r;
}

void Player::move(int distance, std::string direction)
{
	//TODO
	std::cout << "Placeholder function" << std::endl;
}

