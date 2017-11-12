#include <string>
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
		int attack_power, attack_power_range;
		double speed;
		double crit_chance;
		int allowed_moves = 1;
		int used_moves = 0;

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