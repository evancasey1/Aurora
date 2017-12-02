#include <string>
#include "weapon.h"
#ifndef PLAYER_H
#define PLAYER_H

class Player 
{
	public:
		//Will contain weapons, supplies, armor, etc
		struct Inventory {
			std::vector<Weapon> weapons;
		};

		Player();
		Player (std::string race, std::string name);
		bool moveSpace(int direction, int map_size, WINDOW *player_window);
		std::string getName();
		void setName(std::string n);
		std::string getRace();
		void setRace(std::string r);
		void userCreatePlayer();
		int getRow();
		int getCol();
		void setPosition(int r, int c);
		void printStatus(WINDOW *player_window);
		int computeAttackPower();
		void gainExp(int xp, WINDOW *p_window);
		void printInventory(WINDOW *inv_window);

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
		int current_xp_cap;
		int current_xp;
		int health_mod;
		double level_up_multiplier_health;

		int passive_health_regen_counter;   //count of number of moves. Rolls back to 0 when equal to trigger.
		int passive_health_regen_trigger; //number of moves before passive health regeneration happens
		int passive_health_regen_amount;  //amount of health gained on trigger

		Weapon *primary_weapon;

	private:
		std::string name;
		std::string race;
		struct Attributes 
		{
			double fighting;
			double perception;
			double hunting;
			double speed;
			double constitution;
		};
		int row;
		int col;
};

#endif