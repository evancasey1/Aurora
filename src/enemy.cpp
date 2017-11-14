#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <string>
#include "enemy.h"

Enemy::Enemy(int p_row, int p_col, int p_vision, int map_size)
{
	//TODO
	//  cant spawn on another enemy
	//  weight name (enemy type) percent chance
	int index = (rand() % this->NUM_ENEMY_TYPES);
	this->name = NAMES[index];
	this->symbol = SYMBOLS[index];
	this->vision = 5;
	this->total_health = 10;
	this->current_health = this->total_health;
	this->attack_power = 2;
	this->attack_power_range = 1;
	this->crit_chance = 0.1;
	this->speed = 1.5;
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
	//Basic spawn logic
	//e_row = (rand() % map_size);
	//e_col = (rand() % map_size);

	e_row > (map_size - 1) ? e_row = (map_size - 1) : false;
	e_row < 0 ? e_row = 0 : false;
	e_col > (map_size - 1) ? e_col = (map_size - 1) : false;
	e_col < 0 ? e_col = 0 : false;

	this->row = e_row;
	this->col = e_col;
	//printw("Row: %d, Col: %d - ", e_row, e_col);
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