/*
* Elf
*/
#include <string>

class Elf : public Player
{
	private:
		void modify_attrs();
		//in order of declaration
		//fight -> perc -> hunt -> speed -> const
		static constexpr double mods[5] = {0.9, 1.8, 1.3, 1.2, 0.75};
};