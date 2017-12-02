#include <iostream>
#include <string>
#include <ncurses.h>
#include <vector>
#include "player.h"

Player::Player()
{
	setName("Adventurer");
	setRace("Undefined");
	this->vision = 6;
	this->primary_weapon = new Weapon("Short Sword", 3, 1);

	//placeholder. For debugging only
	//Player creation will be overhauled later
	this->base_total_health = 50;
	this->current_total_health = this->base_total_health;
	this->current_health = this->base_total_health;
	this->speed = 2.0;
	this->crit_chance = 0.05;
	this->allowed_moves = 1;
	this->used_moves = 0;
	//Will be an attribute of weapon later
	this->accuracy = 0.9;
	this->xp_cap_multiplier = 1.4;
	this->current_xp_cap = 150;
	this->level = 1;
	this->current_xp = 0;
	this->level_up_multiplier_health = 1.2;
	this->health_mod = 0;
	this->passive_health_regen_counter = 0;
	this->passive_health_regen_trigger = 10;
	this->passive_health_regen_amount = 1;
}
Player::Player(std::string race, std::string name) 
{
	setName(name);
	setRace(race);
}

void Player::printInventory(WINDOW *inv_window)
{
	wclear(inv_window);
	wprintw(inv_window, "WEAPONS:\n");
	int counter = 0;
	std::vector<Weapon>::iterator iter;
	for (iter = this->inventory.weapons.begin(); iter != this->inventory.weapons.end();) {
		wprintw(inv_window, "[%x] %s\n", counter, (iter->name).c_str());
		++iter;
		++counter;
	}
	counter = 0;
	wrefresh(inv_window);
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
	const char *options[] = {"Human", "Elf", "Dwarf"};
	int horiz_pad = (int) ((COLS/2)-10);
	int vert_pad = 3;
	int ch = 0;
	
	attron(A_BOLD);
	mvprintw(vert_pad + 10, horiz_pad, "Select your adventurer:\n");
	attroff(A_BOLD);
	while(ch != KEY_ENTER && ch != '\n') {
		//prints contents of options[]
		//highlights currently selected option
		for (int i = 0; i < 3; i++) {
			move(vert_pad + i + 11, horiz_pad);
			clrtoeol();
			if (i != chosen_index) {
				addstr(options[i]);
			}
			else {
				attron(A_STANDOUT);
				addstr(options[i]);
				attroff(A_STANDOUT);	
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
	this->race = options[chosen_index];
	clear();
	refresh();
}

void Player::gainExp(int xp, WINDOW *alert_win) 
{
	this->current_xp += xp;
	if (this->current_xp >= this->current_xp_cap) {
		this->current_xp -= this->current_xp_cap;
		this->level++;
		this->current_xp_cap *= this->xp_cap_multiplier;
		wattroff(alert_win, COLOR_PAIR(5));
		wattron(alert_win, COLOR_PAIR(7));
		wprintw(alert_win, "Level up! %d -> %d\n", this->level - 1, this->level);
		wrefresh(alert_win);
		wattroff(alert_win, COLOR_PAIR(7));
		wattron(alert_win, COLOR_PAIR(5));
		this->base_total_health *= this->level_up_multiplier_health;
		this->current_total_health = this->base_total_health + this->health_mod;
	}
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
	wmove(player_window, 0, 0);
	wclrtoeol(player_window);
	wprintw(player_window, "Health: %d/%d \nLevel: %d        XP: %d/%d\n", this->current_health, this->current_total_health, this->level, this->current_xp, this->current_xp_cap);
	wrefresh(player_window);
}

int Player::computeAttackPower() {
	int power = this->primary_weapon->attack_power;
	int power_range = this->primary_weapon->attack_power_range;
	double crit_chance = this->primary_weapon->crit_chance;
	double accuracy = this->primary_weapon->accuracy;
	
	double chance_to_hit  = ((double) rand() / RAND_MAX);
	double chance_to_crit = ((double) rand() / RAND_MAX);
	if (power_range != 0) {
		power += (rand() % power_range);
	}
	if (crit_chance >= chance_to_crit) {
		power *= 2;
	}
	if(accuracy >= chance_to_hit) {
		return power;
	}
	return 0;
}

//Returns true if the movement was valid, false otherwise
bool Player::moveSpace(int direction, int map_size, WINDOW *player_window)
{
	switch (direction) {
		case KEY_UP:
			if (this->row == 0) return false;
			this->row--;
			break;
		case KEY_DOWN:
			if (this->row == map_size-1) return false;
			this->row++;
			break;
		case KEY_LEFT:
			if (this->col == 0) return false;
			this->col--;
			break;
		case KEY_RIGHT:
			if (this->col == map_size-1) return false;
			this->col++;
			break;
		default:
			break;
	}
	//Only passively regenerate on active moves (no skips)
	//This promotes active gameplay
	//Also only do if health is not capped
	if (this->current_health < this->current_total_health) {
		this->passive_health_regen_counter++;
		if (this->passive_health_regen_counter == this->passive_health_regen_trigger) {
			this->passive_health_regen_counter = 0;
			this->current_health += this->passive_health_regen_amount;
		}
		this->printStatus(player_window);
	}
	
	return true;
}

