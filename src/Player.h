#include <string>

class Player 
{
	public:
		Player (std::string race, std::string name) {
			this->name = name;
			this->race = race;
		};
		void move(int distance, std::string direction);
		void show_equipped();
		void show_inventory();
		std::string get_name() {
			return this->name;
		}
		void set_name(std::string n) {
			this->name = n;
		}
		std::string get_race() {
			return this->race;
		}
		void set_race(std::string r) {
			this->race = r;
		}
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