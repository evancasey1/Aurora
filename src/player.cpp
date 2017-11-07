#include <iostream>
#include <string>
#include <ncurses.h>
#include "player.h"
#include "human.h"
#include "dwarf.h"
#include "elf.h"

Player::Player()
{
	setName("Adventurer");
	setRace("Undefined");
	this->vision = 6;

	//placeholder. For debugging only
	//Player creation will be overhauled later
	this->total_health = 15;
	this->current_health = 15;
}
Player::Player(std::string race, std::string name) 
{
	setName(name);
	setRace(race);
}

void Player::userCreatePlayer()
{
	//TODO:
	// 	Name selection
	//	Attribute selection
	//  break into 3 helper functions
	//		get name, get race, get attrs
	//	make this menu selection a reusable function
	int chosen_index = 0;
	const char *options_sel[] =	{">> Human <<", ">> Elf <<", ">> Dwarf <<"};
	const char *options_idle[] = {"Human", "Elf", "Dwarf"};
	int horiz_pad = (int) ((COLS/2)-10);
	int vert_pad = 3;
	int ch = 0;
	
	mvprintw(vert_pad-1, horiz_pad, "Select your map size\n");
	while(ch != KEY_ENTER && ch != '\n') {
		//prints contents of options[]
		//highlights currently selected option
		for (int i = 0; i < 3; i++) {
			if (i != chosen_index) {
				move(vert_pad + i, horiz_pad);
				clrtoeol();
				addstr(options_idle[i]);
			}
			else {
				move(vert_pad + i, horiz_pad);
				clrtoeol();
				addstr(options_sel[i]);			
			}
		}
		
		ch = getch();
		switch(ch) {
			case KEY_UP:
				chosen_index <= 0 ? chosen_index = 2 : chosen_index--;
				refresh();
				break;
			case KEY_DOWN:
				chosen_index >= 2 ? chosen_index = 0 : chosen_index++;
				refresh();
				break;
			default:
				break;
		}
	}
	this->race = options_idle[chosen_index];
	clear();
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

//Will print things like player gold count, health, status, and 
//directional indicators to next obelisk
void Player::printStatus(WINDOW *player_window)
{
	wprintw(player_window, "Health: %d/%d", this->current_health, this->total_health);
	wrefresh(player_window);
}

void Player::moveSpace(int direction, int map_size)
{
	//TODO
	switch (direction) {
		case KEY_UP:
			if (this->row == 0) break;
			this->row--;
			break;
		case KEY_DOWN:
			if (this->row == map_size-1) break;
			this->row++;
			break;
		case KEY_LEFT:
			if (this->col == 0) break;
			this->col--;
			break;
		case KEY_RIGHT:
			if (this->col == map_size-1) break;
			this->col++;
			break;
		default:
			break;
	}
}

