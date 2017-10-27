#include <string>

class Player 
{
	public:
		Player();
		Player (std::string race, std::string name);
		void moveSpace(int direction);
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