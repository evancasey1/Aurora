#include <string>
#ifndef ENEMY_H
#define ENEMY_H


//TODO:
//	this will be the parent class to each subclass of enemies.
//	Because enemies will have unique abilities/equipment/loot/stats
//	and to be able to be more flexible, this change will be necessary down the line.
//  ---
//	Enemies will all have an A* pathfinding algorithm implemented instead of the
//	current "dumb" algorithm that will introduce problems when obstacles are implemented
//	and currently do have issues when 2 or more enemies are in close proximity
class Enemy
{
	public:
		Enemy(int rw, int cl, int vi, int sz);
		void seek(int p_row, int p_col);
		void idle(int m_size);
		int row;
		int col;
		int lifespan;
		int vision;
		bool alert_player = true;
		const char *name;

	private:
		//int health;
		
		//moves will be enemy specific later. temp values
		int seek_moves = 1;
		int idle_moves = 1;
		static const int NUM_ENEMY_TYPES = 7;
		const char *NAMES[NUM_ENEMY_TYPES] = {"Wolf", "Alpha Wolf", "Wight", "Goblin", "Troll", "Orc", "Bear"};
};

#endif