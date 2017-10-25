#include <string>

class Player 
{
	public:
		Player();
		Player (std::string race, std::string name);
		void move(int distance, std::string direction);
		void showEquipped();
		void showInventory();
		std::string getName();
		void setName(std::string n);
		std::string getRace();
		void setRace(std::string r);
		void showAttrs();
		void userCreatePlayer(int cur_pos, int buf_height);

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
};