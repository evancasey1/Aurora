#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <math.h>
#include "armor.h"
#include "armortype.h"
#include "equipmenttype.h"


Armor::Armor() 
{
	extern int item_id_counter;
	this->item_id = item_id_counter++;
	this->armor_type = ArmorType::Default;
	this->equipment_id = static_cast<int>(EquipmentType::Armor);
	this->level = 0;
	this->protection = 0.00;
	this->name = "DEFAULT";
}

Armor::Armor(int level) 
{
	extern int item_id_counter;
	this->item_id = item_id_counter++;
	double base_prot = 0.05;
	this->armor_type = static_cast<ArmorType>(rand() % 5);
	this->equipment_id = static_cast<int>(EquipmentType::Armor);
	this->level = level;
	this->protection = base_prot * pow(base_prot + 1, level);
	//Added protection of up to 10% or 0.10
	this->protection += ((rand() % 10) / 100.0);
	switch(armor_type) {
		case ArmorType::Boots:
			this->name = "Boots";
			break;
		case ArmorType::Legs:
			this->name = "Leggings";
			break;
		case ArmorType::Chestpiece:
			this->name = "Chestpiece";
			break;
		case ArmorType::Gloves:
			this->name = "Gloves";
			break;
		case ArmorType::Helm:
			this->name = "Helmet";
			break;
		default:
			return;
	}
}

Armor::Armor(int level, std::string name, double prot) 
{
	extern int item_id_counter;
	this->item_id = item_id_counter++;
	this->equipment_id = static_cast<int>(EquipmentType::Armor);
	this->level = level;
	this->name = name;
	this->protection = prot;
	this->armor_type = static_cast<ArmorType>(rand() % 5);
}

void Armor::printType(WINDOW *win) 
{
	wprintw(win, "ARMOR\n");
	wrefresh(win);
}

void Armor::printDescription(WINDOW *item_description_window)
{
	wprintw(item_description_window, "Protection: %.2f", this->protection);
	wrefresh(item_description_window);
}