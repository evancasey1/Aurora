#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
#include "enemy.h"

Enemy::Enemy(std::string e_name, char e_symbol, int p_row, int p_col, int p_vision, int map_size)
{
	this->name = e_name;
	this->symbol = e_symbol;
	this->vision = 5;
	this->total_health = 10;
	this->current_health = this->total_health;
	this->attack_power = 3;
	this->attack_power_range = 1;
	this->crit_chance = 0.1;
	this->speed = 1.5;
	this->alert_player = true;
	this->idle_moves = 1;
	this->seek_moves = 1;
	this->accuracy = 0.5;
	this->XP = 30;
	this->loot_drop_chance = 1;

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
	double accuracy = this->accuracy;
	
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

void Enemy::deathEvents(std::vector<Loot> *loot) {
	//Loot calculations
	double loot_chance_roll  = ((double) rand() / RAND_MAX);
	if (this->loot_drop_chance >= loot_chance_roll) {
		//Generate loot
		//Drop loot on map
		Loot *obj = new Loot();
		obj->row = this->row;
		obj->col = this->col;
		loot->push_back(*obj);
	}
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