#include <string>
#include "weapon.h"
#ifndef PLAYER_H
#define PLAYER_H

class Player 
{
	public:
		Player();
		Player (std::string race, std::string name);
		bool moveSpace(int direction, int map_size);
		std::string getName();
		void setName(std::string n);
		std::string getRace();
		void setRace(std::string r);
		void userCreatePlayer();
		int getRow();
		int getCol();
		void setPosition(int r, int c);
		void printStatus(WINDOW *player_window);

		int vision;
		int current_health, total_health;
		double speed;
		double crit_chance;
		double accuracy;
		int allowed_moves;
		int used_moves;

		Weapon *primary_weapon;

		//Will contain weapons, supplies, armor, etc
		struct Equipment {
			std::vector<Weapon> weapons;
		};

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