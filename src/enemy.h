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
		Enemy(std::string en, char es, int rw, int cl, int vi, int sz);
		void seek(int p_row, int p_col);
		void idle(int m_size);
		int row;
		int col;
		int lifespan;
		int vision;
		int current_health;
		int total_health;
		int attack_power;
		int attack_power_range;
		double speed;
		double crit_chance;
		double accuracy;
		bool alert_player;
		char symbol;
		std::string name;

	private:		
		int seek_moves;
		int idle_moves;
};

#endif