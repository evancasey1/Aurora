#include <string>
#include "weapon.h"
#include "food.h"
#include "equipment.h"
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
		struct Loot {
			std::string dropped_by;
			std::vector<Weapon> weapons;
			std::vector<Food> food;
			//Equipment, etc later
			int row;
			int col;
			long l_id;
		};
		Enemy(std::string en, char es, int rw, int cl, int vi, int sz, int lvl);
		void seek(int p_row, int p_col);
		void idle(int m_size);
		int computeAttackPower();
		bool isValidMove(std::vector<Enemy> *enems, int p_row, int p_col);
		void deathEvents(std::vector<Enemy::Loot> *loot, WINDOW *win);

		int row;
		int souls;
		int col;
		int lifespan;
		int vision;
		int current_health;
		int total_health;
		int attack_power;
		int attack_power_range;
		int XP;
		double speed;
		double crit_chance;
		double accuracy;
		double base_protection;
		double current_protection;
		double base_evasion;
		double current_evasion;
		bool alert_player;
		char symbol;
		double loot_drop_chance;
		int number_drops_possible;
		double level_up_multiplier_health;
		double level_up_multiplier_damage;
		std::string name;
		int seek_moves;
		int idle_moves;

		struct Equipment {
			std::vector<Weapon> weapons;
		};

};

#endif