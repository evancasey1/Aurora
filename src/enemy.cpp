#include <cstdlib>
#include <ctime>
#include <iostream>
#include "enemy.h"

Enemy::Enemy(int p_row, int p_col, int p_vision, int map_size)
{
	//TODO:
	//	only valid spawns
	//		outside vision
	//		inside map
	int spawnx = 0;
	int spawny = 0;
	this->vision = 5;
	srand((int)time(0));
	spawnx = (rand() % p_row) + p_vision;
	spawny = (rand() % p_col) + p_vision;
	
	this->row = spawny;
	this->col = spawnx;
}