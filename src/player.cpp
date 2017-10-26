#include <iostream>
#include <string>
#include <ncurses.h>
#include "cursorUtilities.h"
#include "player.h"
#include "human.h"
#include "dwarf.h"
#include "elf.h"

Player::Player()
{
	setName("Adventurer");
	setRace("Undefined");
	this->vision = 5;
}
Player::Player(std::string race, std::string name) 
{
	setName(name);
	setRace(race);
}

void Player::userCreatePlayer(int command_input_cursor_position, int buf_height)
{
	//TODO:
	// 	Name selection
	//	Attribute selection
	//  break into 3 helper functions
	//		get name, get race, get attrs
	std::string input_str;
	std::string race;
	std::string name;
	printw("Select your race");
	printw("(1) Human\n(2) Dwarf\n(3) Elf\n");
	refresh();
}

void Player::setPosition(int row, int col)
{
	this->row = row;
	this->col = col;
}

int Player::getRow()
{
	return this->row;
}

int Player::getCol()
{
	return this->col;
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

void Player::move(int direction)
{
	//TODO
	std::cout << "Direction: " << direction << std::endl;
}

