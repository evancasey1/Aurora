#include <iostream>
#include <string>
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

