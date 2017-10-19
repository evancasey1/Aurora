#include <iostream>
#include <string>
#include "Player.h"
#include "Human.h"
#include "Dwarf.h"
#include "Elf.h"


Player::Player (std::string race, std::string name) {
	set_name(name);
	set_race(race);
}

std::string Player::get_name() {
	return this->name;
}
void Player::set_name(std::string n) {
	this->name = n;
}
std::string Player::get_race() {
	return this->race;
}
void Player::set_race(std::string r) {
	this->race = r;
}

void Player::move(int distance, std::string direction)
{
	//TODO
	std::cout << "Placeholder function" << std::endl;
}

