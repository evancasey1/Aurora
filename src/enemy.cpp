#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
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

Enemy::Enemy(std::string e_name, char e_symbol, int p_row, int p_col, int p_vision, int map_size)
{
	bool validEnemy = false;
	this->name = e_name;
	this->symbol = e_symbol;
	/* ENEMY ATTRIBUTES */
	std::ifstream infile("enemyAttributes.txt");
	std::string line;
	std::vector<std::string> elements;
	while (std::getline(infile, line))
	{
		elements = split(line, ' ');
		if (elements.at(0) == e_name) {
			validEnemy = true;
			break;
		}
	}

	if (validEnemy) {
		//I am not sold on the "Magic number" aspect of this solution, but it is far better
		//than the alternative of having an individual class per each enemy or a separate conditional
		//block for each.
		this->accuracy 				= std::atof(elements.at(1).substr(4, 4).c_str());
		this->crit_chance 			= std::atof(elements.at(2).substr(5, 4).c_str());
		this->attack_power 			= std::atoi(elements.at(3).substr(4, 2).c_str());
		this->base_protection 		= std::atof(elements.at(4).substr(5, 4).c_str());
		this->total_health 			= std::atoi(elements.at(5).substr(3, 3).c_str());
		this->souls 				= std::atoi(elements.at(6).substr(6, 2).c_str());
		this->XP 					= std::atoi(elements.at(7).substr(3, 3).c_str());
		this->idle_moves 			= std::atoi(elements.at(8).substr(5, 1).c_str());
		this->seek_moves 			= std::atoi(elements.at(9).substr(5, 1).c_str());
		this->number_drops_possible = std::atoi(elements.at(10).substr(6, 1).c_str());
		this->loot_drop_chance 		= std::atof(elements.at(11).substr(9, 4).c_str());
		this->vision 				= std::atoi(elements.at(12).substr(4, 2).c_str());
		this->speed 				= std::atof(elements.at(13).substr(4, 5).c_str());
		this->base_evasion			= std::atof(elements.at(14).substr(4, 4).c_str());
		this->attack_power_range = 2;
	}
	else {
		this->accuracy = 0.5;
		this->crit_chance = 0.1;
		this->attack_power = 3;
		this->base_protection = 0.00;
		this->total_health = 10;
		this->souls = 1;
		this->XP = 30;
		this->idle_moves = 2;
		this->seek_moves = 2;
		this->number_drops_possible = 2;
		this->loot_drop_chance = 0.6;
		this->vision = 5;
		this->speed = 1.5;
		this->base_evasion = 0.0;
		this->attack_power_range = 2;
	}

	this->current_evasion = this->base_evasion;
	this->current_protection = this->base_protection;
	this->current_health = this->total_health;
	this->alert_player = true;

	int e_row = 0;
	int e_col = 0;
	int range = (p_vision * 2);
	int choice = (rand() % 2); //0 or 1
	int op = (rand() % 2);
	switch (choice) {
		case 0:
			if (!op) {
				e_row = p_row + p_vision;
			}
			else {
				e_row = p_row - p_vision;
			}
			e_col = (rand() % range) + p_col;
			break;
		case 1:
			if (!op) {
				e_col = p_col + p_vision;
			}
			else {
				e_col = p_col - p_vision;
			}
			e_row = (rand() % range) + p_row;
			break;
		default:
			break;
	}

	e_row > (map_size - 1) ? e_row = (map_size - 1) : false;
	e_row < 0 ? e_row = 0 : false;
	e_col > (map_size - 1) ? e_col = (map_size - 1) : false;
	e_col < 0 ? e_col = 0 : false;

	this->row = e_row;
	this->col = e_col;
}

int Enemy::computeAttackPower() {
	int power = this->attack_power;
	int power_range = this->attack_power_range;
	double crit_chance = this->crit_chance;
	
	double chance_to_crit = ((double) rand() / RAND_MAX);
	if (power_range != 0) {
		power += (rand() % power_range);
	}
	if (crit_chance >= chance_to_crit) {
		power *= 2;
	}

	return (int)power;
}

void Enemy::deathEvents(std::vector<Loot> *loot, WINDOW *alert_win) {
	//Loot calculations
	double loot_chance_roll;
	bool dropped = false;
	static long unique_loot_id = 0;
	Loot *obj = new Loot();
	for (int i = 0; i < this->number_drops_possible; i++) {
		loot_chance_roll  = ((double) rand() / RAND_MAX);
		if (this->loot_drop_chance >= loot_chance_roll) {
			//Generate loot
			//Drop loot on map
			dropped = true;
			obj->row = this->row;
			obj->col = this->col;
			obj->dropped_by = this->name;
			
			if (this->name == "Wolf") {
				//Food(std::string name, int initial_health_gain, int health_gain_per_trigger, int turns_until_trigger, int total_triggers);
				obj->food.push_back(*(new Food("Wolf Meat", 4, 1, 5, 3)));
			}
			else if (this->name == "Bear") {
				obj->food.push_back(*(new Food("Bear Meat", 6, 2, 4, 2)));
			}
			else {
				obj->weapons.push_back(*(new Weapon()));
			}
		}
	}
	if (dropped) {
		wattroff(alert_win, COLOR_PAIR(5));
		wattron(alert_win, COLOR_PAIR(6));
		wprintw(alert_win, "%s dropped loot.\n", (this->name).c_str());
		wattroff(alert_win, COLOR_PAIR(6));
		wattron(alert_win, COLOR_PAIR(5));
		obj->l_id = unique_loot_id;
		unique_loot_id++;
		loot->push_back(*obj);
	}
	wrefresh(alert_win);
}

bool Enemy::isValidMove(std::vector<Enemy> *enemies, int p_row, int p_col) {
	std::vector<Enemy>::iterator iter;
	for (iter = enemies->begin(); iter != enemies->end();) {
		if (iter->row == this->row && iter->col == this->col) {
			return false;
		}
		++iter;
	}
	if (this->row == p_row && this->col == p_col) {
		return false;
	}

	return true;
}

void Enemy::idle(int map_size)
{
	//move a random direction or stay still
	int move;
	for (int i = 0; i < this->idle_moves; i++) {
		move = (rand() % 6);
		switch (move) {
			case 0:
				this->row++;
				break;
			case 1:
				this->row--;
				break;
			case 2:
				this->col++;
				break;
			case 3:
				this->col--;
				break;
			default:
				break;
		}
		//bounding logic
		if (this->row >= map_size) this->row--;
		else if (this->row < 0) this->row++;

		if (this->col >= map_size) this->col--;
		else if (this->col < 0) this->col++;
	}
}

void Enemy::seek(int p_row, int p_col)
{
	for (int i = 0; i < this->seek_moves; i++){	
		if (p_row > this->row) {
			this->row++;
		}
		else if (p_row < this->row) {
			this->row--;
		}
		else if (p_col > this->col) {
			this->col++;
		}
		else if (p_col < this->col) {
			this->col--;
		}
	}
}