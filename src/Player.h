#include <string>

class Player 
{
	public:
		Player (std::string race, std::string name);
		void move(int distance, std::string direction);
		void show_equipped();
		void show_inventory();
		std::string get_name();
		void set_name(std::string n);
		std::string get_race();
		void set_race(std::string r);
		void show_attrs();

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

};