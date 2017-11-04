#include <string>
#ifndef PLAYER_H
#define PLAYER_H

class Player 
{
	public:
		Player();
		Player (std::string race, std::string name);
		void moveSpace(int direction, int map_size);
		void showEquipped();
		void showInventory();
		std::string getName();
		void setName(std::string n);
		std::string getRace();
		void setRace(std::string r);
		void showAttrs();
		void userCreatePlayer();
		int getRow();
		int getCol();
		void setPosition(int r, int c);
		int vision;
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
		double warmth;
		double hunger;
		int row;
		int col;
};

#endif