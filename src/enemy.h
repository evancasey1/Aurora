#include <string>
#ifndef ENEMY_H
#define ENEMY_H

class Enemy
{
	public:
		Enemy(int rw, int cl, int vi, int sz);
		void seek(int p_row, int p_col);
		void idle();
		int row;
		int col;
		int lifespan;
		int vision;
		bool alert_player = true;
		const char *name;

	private:
		//int health;
		
		//moves will be enemy specific later. temp values
		int seek_moves = 2;
		int idle_moves = 1;
		static const int NUM_ENEMY_TYPES = 7;
		const char *NAMES[NUM_ENEMY_TYPES] = {"Wolf", "Alpha Wolf", "Wight", "Goblin", "Troll", "Orc", "Bear"};
};

#endif