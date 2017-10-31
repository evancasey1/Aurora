#include <string>
#ifndef ENEMY_H
#define ENEMY_H

class Enemy
{
	public:
		Enemy(int rw, int cl, int vi, int sz);
		void seek(int p_row, int p_col);
		int row;
		int col;
		int lifespan = 10;
		const char *name;

	private:
		//int health;
		//int vision = 5;
		int moves = 2;
		static const int NUM_ENEMY_TYPES = 7;
		const char *NAMES[NUM_ENEMY_TYPES] = {"Wolf", "Alpha Wolf", "Wight", "Goblin"
							    			  "Troll", "Orc", "Bear"};
};

#endif