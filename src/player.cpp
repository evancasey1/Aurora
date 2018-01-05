#include <iostream>
#include <string>
#include <ncurses.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include "player.h"
#include "enemy.h"

template<typename Out>
static void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

static std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

Player::Player(std::string p_class)
{
	this->vision = 6;
	this->primary_weapon = new Weapon("Short Sword", 3, 2);
	Food *start_food = new Food("Meat", 3, 1, 2, 2);
	for (int i = 0; i < 3; i++) {
		this->inventory.food.push_back(*start_food);
	}
	this->inventory.food_capacity = 5;
	this->inventory.weapon_capacity = 8;
	this->inventory.weapon_count = 0;
	this->inventory.food_count = 3;

	std::ifstream infile("playerAttributes.txt");
	std::string line;
	std::vector<std::string> elements;
	while (std::getline(infile, line))
	{
		elements = split(line, ' ');
		if (elements.at(0) == p_class) {
			break;
		}
	}
	this->accuracy 					 = std::atof(elements.at(1).substr(4, 4).c_str());
	this->speed   					 = std::atoi(elements.at(2).substr(4, 2).c_str());
	this->base_damage 				 = std::atoi(elements.at(3).substr(4, 2).c_str());
	this->allowed_moves				 = std::atoi(elements.at(4).substr(4, 1).c_str());
	this->base_evasion 				 = std::atof(elements.at(5).substr(4, 4).c_str());
	this->base_protection 		     = std::atof(elements.at(6).substr(5, 4).c_str());
	this->base_total_health 		 = std::atoi(elements.at(7).substr(3, 2).c_str());
	this->level_up_multiplier_health = std::atof(elements.at(8).substr(4, 4).c_str());
	this->crit_chance				 = std::atof(elements.at(8).substr(5, 4).c_str());
	this->level_up_multiplier_damage = std::atof(elements.at(9).substr(5, 4).c_str());

	//placeholder. For debugging only
	//Player creation will be overhauled later
	/*
	this->base_total_health = 5000;
	this->speed = 2.0;
	this->crit_chance = 0.05;
	this->allowed_moves = 1;
	this->accuracy = 0.00;
	this->level_up_multiplier_health = 1.2;
	this->base_damage = 1;
	this->base_protection = 0.00;
	this->base_evasion = 0.00;
	*/
	//These attributes are constant for all player types
	this->current_total_health = this->base_total_health;
	this->current_health = this->base_total_health;
	this->used_moves = 0;
	this->level_up_multiplier_xp = 1.25;
	this->current_xp_cap = 150;
	this->level = 1;
	this->level_points = 0;
	this->current_xp = 0;
	this->passive_health_regen_counter = 0;
	this->passive_health_regen_trigger = 50;
	this->passive_health_regen_amount = 1;
	this->inventory_index = 0;
	this->max_inventory_index = 1;
	this->souls_cap = 25;
	this->souls = 0;
	this->souls_multiplier = 1.15;
	this->current_evasion = this->base_evasion;
	this->current_protection = this->base_protection;
}

int getLootIndex(std::vector<Enemy::Loot> *loot, long loot_id) 
{
	for (int i = 0; i < loot->size(); i++) {
		if (loot->at(i).l_id == loot_id) {
			return i;
		}
	}
	return -1;
}

void Player::pickUpLootAtIndex(std::vector<Enemy::Loot> *loot, int current_total_index, int current_vect_index, int current_item_index, std::vector<Equipment> *equipment_at_loc, std::vector<Enemy::Loot> *loot_at_loc, std::vector<int> *loot_indices, WINDOW *inventory_window, WINDOW *alert_window) 
{
	//	TODO: Delete the values in the true loot vector as well.
	std::vector<Enemy::Loot>::iterator loot_iter;
	std::vector<Equipment>::iterator equipment_iter;

	loot_iter = loot_at_loc->begin();
	equipment_iter = equipment_at_loc->begin();
	while (std::distance(loot_at_loc->begin(), loot_iter) < current_vect_index) {
		loot_iter++;
	}
	while (std::distance(equipment_at_loc->begin(), equipment_iter) < current_total_index) {
		equipment_iter++;
	}
	int real_loot_index = getLootIndex(loot, loot_iter->l_id);
	if (real_loot_index == -1) {
		//ERROR, but this should never ever happen
		return;
	}

	//Need to reduce repetition here in the future, but it is fine for a rough draft
	/*
	CASE IF WEAPON
	*/
	if (equipment_at_loc->at(current_total_index).equipment_id == 0) {
		if (this->inventory.weapon_count == this->inventory.weapon_capacity) {
			wprintw(alert_window, "Insufficient space in weapon pouch\n");
			wrefresh(alert_window);
		}
		else {
			this->inventory.weapons.push_back(loot_at_loc->at(current_vect_index).weapons.at(current_item_index));
			this->inventory.weapon_count++;
			loot_iter->weapons.erase(loot_iter->weapons.begin() + current_item_index);
			loot->at(real_loot_index).weapons.erase(loot->at(real_loot_index).weapons.begin() + current_item_index);

			loot_indices->at(current_vect_index)--;
			equipment_at_loc->erase(equipment_iter);
		}
	}
	/*
	CASE IF FOOD
	*/
	else if (equipment_at_loc->at(current_total_index).equipment_id == 1) {
		if (this->inventory.food_count == this->inventory.food_capacity) {
			wprintw(alert_window, "Insufficient space in food reserves\n");
			wrefresh(alert_window);
		}
		else {
			this->inventory.food.push_back(loot_at_loc->at(current_vect_index).food.at(current_item_index));
			this->inventory.food_count++;
			loot_iter->food.erase(loot_iter->food.begin() + current_item_index);
			loot->at(real_loot_index).food.erase(loot->at(real_loot_index).food.begin() + current_item_index);
			
			loot_indices->at(current_vect_index)--;
			equipment_at_loc->erase(equipment_iter);
		}
	}

	if (loot_indices->at(current_vect_index) == -1) {
		loot_indices->erase(loot_indices->begin() + current_vect_index);
	}

	if (loot->at(real_loot_index).weapons.size() == 0 && loot->at(real_loot_index).food.size() == 0) {
		loot->erase(loot->begin() + real_loot_index);
	}
	
	if (loot_iter->food.size() == 0 && loot_iter->weapons.size() == 0) {
		loot_at_loc->erase(loot_iter);
	}

	if (loot_at_loc->size() == 0) {
		wclear(inventory_window);
		wrefresh(inventory_window);
		return;
	}
}

void Player::foodEvents() 
{
	bool deleted;
	std::vector<Food>::iterator food_iter;
	for (food_iter = this->active_food.begin(); food_iter != this->active_food.end();) {
		deleted = false;
		food_iter->turn_count++;
		if (food_iter->turn_count % food_iter->turns_until_trigger) {
			food_iter->trigger_count++;
			this->current_health += food_iter->health_gain_per_trigger;
			if (food_iter->trigger_count == food_iter->total_triggers) {
				food_iter = this->active_food.erase(food_iter);
				deleted = true;
			}
		}
		if (!deleted) {
			food_iter++;
		}
	}
}

void Player::eatFood(Food *food_item, WINDOW *player_status_window) 
{
	this->active_food.push_back(*food_item);
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
			wprintw(item_description_window, "Attack: %d - %d\nAccuracy: %.2f\nCrit: %.2f", this->inventory.weapons.at(index).attack_power, this->inventory.weapons.at(index).attack_power + this->inventory.weapons.at(index).attack_power_range, this->inventory.weapons.at(index).accuracy, this->inventory.weapons.at(index).crit_chance);
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
			wclrtoeol(item_description_window);
			wprintw(item_description_window, "Health Gain: %d\nHeal Over Time: %d:%d:%d", this->inventory.food.at(index).initial_health_gain, this->inventory.food.at(index).health_gain_per_trigger, this->inventory.food.at(index).total_triggers, this->inventory.food.at(index).turns_until_trigger);
			wrefresh(item_description_window);
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

void Player::gainSouls(int souls_to_gain, WINDOW *alert_win)
{
	this->souls += souls_to_gain;
	if (this->souls > souls_cap) {
		this->souls = souls_cap;
	}
	wattroff(alert_win, COLOR_PAIR(5));
	wattron(alert_win, COLOR_PAIR(6));
	wprintw(alert_win, "Acquired %d souls.\n", souls_to_gain);
	wrefresh(alert_win);
	wattroff(alert_win, COLOR_PAIR(6));
	wattron(alert_win, COLOR_PAIR(5));
}

void Player::levelUp(WINDOW *alert_win)
{
	this->current_xp -= this->current_xp_cap;
	this->level++;
	this->current_xp_cap *= this->level_up_multiplier_xp;
	wattroff(alert_win, COLOR_PAIR(5));
	wattron(alert_win, COLOR_PAIR(7));
	wprintw(alert_win, "Level up! %d -> %d\n", this->level - 1, this->level);
	wrefresh(alert_win);
	wattroff(alert_win, COLOR_PAIR(7));
	wattron(alert_win, COLOR_PAIR(5));
	this->base_total_health = std::ceil(this->base_total_health * this->level_up_multiplier_health);
	this->base_damage = std::ceil(this->base_damage * this->level_up_multiplier_damage);
	this->current_total_health = this->base_total_health;
	this->level_points++;
}

void Player::gainExp(int xp, WINDOW *alert_win) 
{
	this->current_xp += xp;
	if (this->current_xp >= this->current_xp_cap) {
		this->levelUp(alert_win);
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
	int power = this->primary_weapon->attack_power + this->base_damage;
	int power_range = this->primary_weapon->attack_power_range;
	double crit_chance = this->primary_weapon->crit_chance;

	double chance_to_crit = ((double) rand() / RAND_MAX);
	if (power_range != 0) {
		power += (rand() % power_range);
	}
	if (crit_chance >= chance_to_crit) {
		power *= 2;
	}
	if (this->souls == this->souls_cap) {
		power *= this->souls_multiplier;
	}
	return (int)power;
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

