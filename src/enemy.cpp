#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include "enemy.h"

Enemy::Enemy(int p_row, int p_col, int p_vision, int map_size)
{
	//TODO
	//  cant spawn on another enemy
	//  fix spawn range
	int e_row = 0;
	int e_col = 0;
	int range = (p_vision * 2) + 1;
	srand((int)time(0));
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