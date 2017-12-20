#include <iostream>
#include <string>
#include <ncurses.h>
#include <vector>
#include "player.h"

Player::Player()
{
	this->vision = 6;
	this->primary_weapon = new Weapon("Short Sword", 3, 1);
	Food *start_food = new Food("Meat", 3, 1, 2, 2);
	for (int i = 0; i < 3; i++) {
		this->inventory.food.push_back(*start_food);
	}
	this->inventory.food_capacity = 5;
	this->inventory.weapon_capacity = 8;
	this->inventory.weapon_count = 0;
	this->inventory.food_count = 3;

	//placeholder. For debugging only
	//Player creation will be overhauled later
	this->base_total_health = 5000;
	this->current_total_health = this->base_total_health;
	this->current_health = this->base_total_health;
	this->speed = 2.0;
	this->crit_chance = 0.05;
	this->allowed_moves = 1;
	this->used_moves = 0;
	//Will be an attribute of weapon later, or a calculation between player and weapon
	this->accuracy = 0.9;
	this->xp_cap_multiplier = 1.4;
	this->current_xp_cap = 150;
	this->level = 1;
	this->level_points = 0;
	this->current_xp = 0;
	this->level_up_multiplier_health = 1.2;
	this->health_mod = 0;
	this->passive_health_regen_counter = 0;
	this->passive_health_regen_trigger = 50;
	this->passive_health_regen_amount = 1;
	this->inventory_index = 0;
	this->max_inventory_index = 1;
}

void Player::eatFood(Food *food_item, WINDOW *player_status_window) {
	this->current_health += food_item->initial_health_gain;
	if (this->current_health > this->current_total_health) {
		this->current_health = this->current_total_health;
	}
	this->printStatus(player_status_window);
	this->inventory.food_count--;
	return;
}

void Player::setPrimaryWeapon(Weapon weapon) 
{
	*this->primary_weapon = weapon;
}

//Need to find a more elegant solution and not be repetitive within the loop
//Some sort of generic pointer would be very useful
void Player::printInventory(WINDOW *inv_window, int index, WINDOW *item_description_window) 
{
	wclear(inv_window);
	wclear(item_description_window);
	int counter = 0;

	if (this->inventory_index == 0) {
		std::vector<Weapon>::iterator iter;
		wattron(inv_window, A_BOLD);
		wprintw(inv_window, "WEAPONS ->\n");
		wattroff(inv_window, A_BOLD);
		if (this->inventory.weapons.size() == 0) {
			wprintw(inv_window, "<EMPTY>");
		}
		else {
			wclrtoeol(item_description_window);
			wprintw(item_description_window, "Attack: %d - %d\n", this->inventory.weapons.at(index).attack_power, this->inventory.weapons.at(index).attack_power + this->inventory.weapons.at(index).attack_power_range);
			wrefresh(item_description_window);
			for (iter = this->inventory.weapons.begin(); iter != this->inventory.weapons.end();) {
				if (counter == index) {
					wattron(inv_window, A_STANDOUT);
					wprintw(inv_window, "[%x] %s\n", counter, (iter->name).c_str());
					wattroff(inv_window, A_STANDOUT);
				}
				else {
					wprintw(inv_window, "[%x] %s\n", counter, (iter->name).c_str());
				}
				++iter;
				++counter;
			}
		}
	}
	else if (this->inventory_index == 1) {
		std::vector<Food>::iterator iter;
		wattron(inv_window, A_BOLD);
		wprintw(inv_window, "<- FOOD\n");
		wattroff(inv_window, A_BOLD);
		if (this->inventory.food.size() == 0) {
			wprintw(inv_window, "<EMPTY>");
		}
		else {
			for (iter = this->inventory.food.begin(); iter != this->inventory.food.end();) {
				if (counter == index) {
					wattron(inv_window, A_STANDOUT);
					wprintw(inv_window, "[%x] %s\n", counter, (iter->name).c_str());
					wattroff(inv_window, A_STANDOUT);
				}
				else {
					wprintw(inv_window, "[%x] %s\n", counter, (iter->name).c_str());
				}
				++iter;
				++counter;
			}
		}
	}
	wrefresh(inv_window);
	
}

void Player::manageInventory(WINDOW *inv_window, WINDOW *player_status_window, WINDOW *alert_win)
{
	int ch;
	int index = 0;
	Weapon temp;
	std::vector<Weapon> *weapon_vect;
	WINDOW *item_description_window = newwin(30, 30, 22, 60);
	while (true) {
		//temp = NULL;
		printInventory(inv_window, index, item_description_window);
		ch = getch();
		switch(ch) {
			//player movement
			case KEY_UP:
				if (index > 0) {
					index--;
				}
				break;
			case KEY_DOWN:
				if (this->inventory_index == 0 && index < this->inventory.weapons.size() - 1) {
					index++;
				}
				else if (this->inventory_index == 1 && index < this->inventory.food.size() - 1) {
					index++;
				}
				break;
			case KEY_RIGHT:
				if (this->inventory_index < this->max_inventory_index) {
					this->inventory_index++;
					index = 0;
					wclear(item_description_window);
					wrefresh(item_description_window);
				}
				break;
			case KEY_LEFT:
				if (this->inventory_index > 0) {
					this->inventory_index--;
					wclear(item_description_window);
					wrefresh(item_description_window);
				}
				break;
			case KEY_ENTER: case '\n':
				if (this->inventory_index == 0 && this->inventory.weapons.size() > 0) {
					weapon_vect = &this->inventory.weapons;
					temp = weapon_vect->at(index);
					weapon_vect->at(index) = *this->primary_weapon;
					this->setPrimaryWeapon(temp);
					wprintw(alert_win, "Equipped %s\n", (this->primary_weapon->name).c_str());
					wrefresh(alert_win);
				}
				else if (this->inventory_index == 1 && this->inventory.food.size() > 0) {
					this->eatFood(&this->inventory.food.at(index), player_status_window);
					this->inventory.food.erase(this->inventory.food.begin() + index);
					index = 0;
				}
				break;
			case 'e':
				wclear(inv_window);
				wclear(item_description_window);
				wrefresh(inv_window);
				wrefresh(item_description_window);
				return;
			default:
				break;
		}
	}
}

void Player::userCreatePlayer()
{
	//TODO:
	//	Attribute selection
	int chosen_index = 0;
	std::string options[3] = {"Rogue", "Swordsman", "Warrior"};
	std::string descriptions[3] = {"The Rogue strikes swiftly and relies on shadows for concealment to compensate   for weak constitution.", 
								   "The Swordsman is a balanced fighter,    well suited for bladed weapons and      medium armor.", 
								   "The Warrior is a tanky brute that       specializes in blunt force weapons and  heavy armor."};
	int horiz_pad = (int) ((COLS/2)-10);
	int vert_pad = 3;
	WINDOW *player_description_window = newwin(30, 40, vert_pad + 25, horiz_pad - 10);
	int ch = 0;
	
	attron(A_BOLD);
	mvprintw(vert_pad + 10, horiz_pad, "Select your adventurer:\n");
	attroff(A_BOLD);
	while(ch != KEY_ENTER && ch != '\n') {
		//prints contents of options[]
		//highlights currently selected option
		wclear(player_description_window);
		wprintw(player_description_window, "%s", descriptions[chosen_index].c_str());
		wrefresh(player_description_window);
		for (int i = 0; i < 3; i++) {
			move(vert_pad + i + 11, horiz_pad);
			clrtoeol();
			if (i != chosen_index) {
				addstr(options[i].c_str());
			}
			else {
				attron(A_STANDOUT);
				addstr(options[i].c_str());
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
	free(player_description_window);
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
		this->level_points++;
	}
}

void Player::setPosition(int row, int col)
{
	this->row = row;
	this->col = col;
}

//Will print things like player gold count, health, status, and 
//directional indicators to next obelisk
void Player::printStatus(WINDOW *player_window)
{
	wmove(player_window, 0, 0);
	wclrtoeol(player_window);
	wprintw(player_window, "Health: ");
	//If health is below a certain threshold display it in red
	if (this->current_health <= (int)(0.25 * this->current_total_health)) {
		wattron(player_window, COLOR_PAIR(5));
	}
	wprintw(player_window, "%d/%d\n", this->current_health, this->current_total_health);
	wattroff(player_window, COLOR_PAIR(5));
	wprintw(player_window, "Level: %d        XP: %d/%d\n", this->level, this->current_xp, this->current_xp_cap);
	wrefresh(player_window);
}

int Player::computeAttackPower() {
	int power = this->primary_weapon->attack_power;
	int power_range = this->primary_weapon->attack_power_range;
	double crit_chance = this->primary_weapon->crit_chance;
	double acc = this->primary_weapon->accuracy;

	double chance_to_hit  = ((double) rand() / RAND_MAX);
	double chance_to_crit = ((double) rand() / RAND_MAX);
	if (power_range != 0) {
		power += (rand() % power_range);
	}
	if (crit_chance >= chance_to_crit) {
		power *= 2;
	}
	if(acc >= chance_to_hit) {
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

