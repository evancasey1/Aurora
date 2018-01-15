#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <math.h>
#include "armor.h"
#include "armortype.h"

Armor::Armor(int level) 
{
	double base_prot = 0.05;
	this->armor_type = static_cast<ArmorType>(rand() % 5);
	this->equipment_id = 2;
	this->level = level;
	this->prot = std::ceil(pow(base_prot, level));
	if (armor_type == ArmorType::Boots) {
		this->name = "Boots";
	}
	else if (armor_type == ArmorType::Legs) {
		this->name = "Legs";
	}
	else if (armor_type == ArmorType::Chestpiece) {
		this->name = "Chestpiece";
	}
	else if (armor_type == ArmorType::Gloves) {
		this->name = "Gloves";
	}
	else if (armor_type == ArmorType::Helm) {
		this->name = "Helm";
	}
}

Armor::Armor(int level, std::string name, double prot) 
{
	this->equipment_id = 2;
	this->level = level;
	this->name = name;
	this->prot = prot;
	this->armor_type = static_cast<ArmorType>(rand() % 5);
}