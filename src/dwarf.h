#include <string>
#ifndef DWARF_H
#define DWARF_H

class Dwarf : public Player
{
	private:
		void modify_attrs();
		//in order of declaration
		//fight -> perc -> hunt -> speed -> const
		static constexpr double mods[5] = {1.6, 1.5, 0.7, 0.8, 1.5};
};

#endif