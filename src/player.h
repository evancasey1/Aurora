#include <string>
#include "weapon.h"
#include "food.h"
#ifndef PLAYER_H
#define PLAYER_H

class Player 
{
	public:
		//Will contain weapons, supplies, armor, etc
		struct Inventory {
			std::vector<Weapon> weapons;
			std::vector<Food> food;
		};

		Player();
		bool moveSpace(int direction, int map_size, WINDOW *player_window);
		void setRace(std::string r);
		void userCreatePlayer();
		int getRow();
		int getCol();
		void setPosition(int r, int c);
		void printStatus(WINDOW *player_window);
		int computeAttackPower();
		void gainExp(int xp, WINDOW *p_window);
		void printInventory(WINDOW *inv_window, int index);
		void manageInventory(WINDOW *inv_window);

		Inventory inventory;
		int vision;
		int current_health, base_total_health, current_total_health;
		double speed;
		double crit_chance;
		double accuracy;
		int allowed_moves;
		int used_moves;
		double xp_cap_multiplier;
		int level;
		int level_points;
		int current_xp_cap;
		int current_xp;
		int health_mod;
		double level_up_multiplier_health;
		std::string race;
		int row;
		int col;

		int passive_health_regen_counter;   //count of number of moves. Rolls back to 0 when equal to trigger.
		int passive_health_regen_trigger; //number of moves before passive health regeneration happens
		int passive_health_regen_amount;  //amount of health gained on trigger

		Weapon *primary_weapon;
};

#endif