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

			int weapon_capacity;
			int food_capacity;
			int weapon_count;
			int food_count;
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
		void gainSouls(int souls_to_gain, WINDOW *p_window);
		void printInventory(WINDOW *inv_window, int index, WINDOW *item_window);
		void manageInventory(WINDOW *inv_window, WINDOW *player_window, WINDOW *alert_win);
		void setPrimaryWeapon(Weapon weapon);
		void eatFood(Food *food, WINDOW *player_window);

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
		int inventory_index;
		int max_inventory_index;
		int souls;
		int souls_cap;

		int passive_health_regen_counter;   //count of number of moves. Rolls back to 0 when equal to trigger.
		int passive_health_regen_trigger;   //number of moves before passive health regeneration happens
		int passive_health_regen_amount;    //amount of health gained on trigger

		Weapon *primary_weapon;
};

#endif