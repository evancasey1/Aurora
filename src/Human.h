/*
* Human
*/
#include <string>

class Human : public Player
{
	private:
		void modify_attrs();
		//in order of declaration
		//fight -> perc -> hunt -> speed -> const
		static constexpr double mods[5] = {1.1, 0.8, 1.6, 1.1, 1.2};
};