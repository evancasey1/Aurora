#ifndef ARMOR_H
#define ARMOR_H
#include "equipment.h"
#include "armortype.h"

class Armor : public Equipment 
{
	public:
		Armor(int level);
		Armor(int level, std::string name, double prot);
		int level;
		double prot;
		ArmorType armor_type;
		std::string name;
};

#endif