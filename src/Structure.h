#include <string>

class Structure 
{
	public:
		Structure() 
		{
			this->name = "PLACEHOLDER_STRUCTURE_NAME";
		};
		std::string name;
		void show_loot();
		char symbol;

	private:
		//TODO
		//Add loot table
		double warm_up_player();
};