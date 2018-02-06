#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <math.h>
#include "weapon.h"
#include "equipmenttype.h"

Weapon::Weapon() {
	//Random weapon generation
	extern int item_id_counter;
	this->item_id = item_id_counter++;
	this->equipment_id = static_cast<int>(EquipmentType::Weapon);
	this->ENHANCED_CHANCE = 0.20;
	this->LEGENDARY_CHANCE = 0.02;
	this->ENHANCED_MOD = 1.25;
	this->LEGENDARY_MOD = 1.5;

	std::string names[] = {"Sword", "Dagger", "Mace", "Club"};
	int name_index = rand() % 4;
	//Temporary values. Will be dependent on level later
	int attack_pow = 3;
	int attack_pow_range = 2;
	
	std::string w_name = names[name_index];
	double condition_chance = ((double) rand() / RAND_MAX);
	
	this->is_poisonous = false;
	this->poison_chance = 0.0;
	this->crit_chance = 0.15;

	/* Evaluate condition of weapon */
	if (this->LEGENDARY_CHANCE >= condition_chance) {
		this->name = "Legendary " + w_name;
		this->attack_power = (int)((double)attack_pow * this->LEGENDARY_MOD);
		this->attack_power_range = (int)((double)attack_pow_range * this->LEGENDARY_MOD);
	}
	else if (this->ENHANCED_CHANCE >= condition_chance) {
		this->name = "Enhanced " + w_name;
		this->attack_power = (int)((double)attack_pow * this->ENHANCED_MOD);
		this->attack_power_range = (int)((double)attack_pow_range * this->ENHANCED_MOD);
	}
	else {
		this->name = w_name;
		this->attack_power = attack_pow;
		this->attack_power_range = attack_pow_range;
	}
	/* End evaluate */

	if (w_name == "Sword") {
		this->bleed_chance = 0.15;
		//Random value 80 <= X < 100
		this->accuracy = ((rand()%20)+80)/100.0;
		this->attack_power += (rand() % (this->attack_power/4));
	}
	else if (w_name == "Dagger") {
		this->bleed_chance = 0.25;
		//Random value 90 <= X < 100
		this->accuracy = ((rand()%10)+90)/100.0;
		this->attack_power += (rand() % (this->attack_power/2));
	}
	else {
		this->bleed_chance = 0.05;
		//Random value 75 <= X < 100
		this->accuracy = ((rand()%25)+75)/100.0;
		this->attack_power += (rand() % (this->attack_power/3));
	}
}

Weapon::Weapon(int level) {
	//Random weapon generation
	extern int item_id_counter;
	this->item_id = item_id_counter++;
	double level_mod = 1.10;
	this->equipment_id = static_cast<int>(EquipmentType::Weapon);;
	this->ENHANCED_CHANCE = 0.20;
	this->LEGENDARY_CHANCE = 0.02;
	this->ENHANCED_MOD = 1.25;
	this->LEGENDARY_MOD = 1.5;

	std::string names[] = {"Sword", "Dagger", "Mace", "Club"};
	int name_index = rand() % 4;
	//Temporary values. Will be dependent on level later
	int attack_pow = 5;
	int attack_pow_range = 2;
	
	std::string w_name = names[name_index];
	double condition_chance = ((double) rand() / RAND_MAX);
	
	this->is_poisonous = false;
	this->poison_chance = 0.0;
	this->crit_chance = 0.15;

	// Evaluate condition of weapon 
	if (this->LEGENDARY_CHANCE >= condition_chance) {
		this->name = "Legendary " + w_name;
		this->attack_power = (int)((double)attack_pow * this->LEGENDARY_MOD);
		this->attack_power_range = (int)((double)attack_pow_range * this->LEGENDARY_MOD);
	}
	else if (this->ENHANCED_CHANCE >= condition_chance) {
		this->name = "Enhanced " + w_name;
		this->attack_power = (int)((double)attack_pow * this->ENHANCED_MOD);
		this->attack_power_range = (int)((double)attack_pow_range * this->ENHANCED_MOD);
	}
	else {
		this->name = w_name;
		this->attack_power = attack_pow;
		this->attack_power_range = attack_pow_range;
	}
	// End evaluate 

	if (w_name == "Sword") {
		this->bleed_chance = 0.15;
		//Random value 80 <= X < 100
		this->accuracy = ((rand()%20)+80)/100.0;
		this->attack_power += (rand() % (this->attack_power/4));
	}
	else if (w_name == "Dagger") {
		this->bleed_chance = 0.25;
		//Random value 90 <= X < 100
		this->accuracy = ((rand()%10)+90)/100.0;
		this->attack_power += (rand() % (this->attack_power/2));
	}
	else {
		this->bleed_chance = 0.05;
		//Random value 75 <= X < 100
		this->accuracy = ((rand()%25)+75)/100.0;
		this->attack_power += (rand() % (this->attack_power/3));
	}
	this->attack_power = std::ceil(this->attack_power * pow(level_mod, level));
}

Weapon::Weapon(std::string w_name, int attack_pow, int attack_pow_range) {
	extern int item_id_counter;
	this->item_id = item_id_counter++;
	this->equipment_id = static_cast<int>(EquipmentType::Weapon);
	double condition_chance = ((double) rand() / RAND_MAX);
	
	this->is_poisonous = false;
	this->poison_chance = 0.0;
	this->crit_chance = 0.15;

	/* Evaluate condition of weapon */
	if (this->LEGENDARY_CHANCE >= condition_chance) {
		this->name = "Legendary " + w_name;
		this->attack_power = (int)((double)attack_pow * this->LEGENDARY_MOD);
		this->attack_power_range = (int)((double)attack_pow_range * this->LEGENDARY_MOD);
	}
	else if (this->ENHANCED_CHANCE >= condition_chance) {
		this->name = "Enhanced " + w_name;
		this->attack_power = (int)((double)attack_pow * this->ENHANCED_MOD);
		this->attack_power_range = (int)((double)attack_pow_range * this->ENHANCED_MOD);
	}
	else {
		this->name = w_name;
		this->attack_power = attack_pow;
		this->attack_power_range = attack_pow_range;
	}
	/* End evaluate */

	if (w_name == "Sword") {
		this->bleed_chance = 0.15;
		//Random value 80 <= X < 100
		this->accuracy = ((rand()%20)+80)/100.0;
		this->attack_power += (rand() % (this->attack_power/4));
	}
	else if (w_name == "Dagger") {
		this->bleed_chance = 0.25;
		//Random value 90 <= X < 100
		this->accuracy = ((rand()%10)+90)/100.0;
		this->attack_power += (rand() % (this->attack_power/2));
	}
	else {
		this->bleed_chance = 0.05;
		//Random value 75 <= X < 100
		this->accuracy = ((rand()%25)+75)/100.0;
		this->attack_power += (rand() % (this->attack_power/3));
	}
}

void Weapon::printType(WINDOW *win) 
{
	wprintw(win, "WEAPON\n");
	wrefresh(win);
}

void Weapon::printDescription(WINDOW *item_description_window)
{
	wprintw(item_description_window, "Attack: %d - %d\nAccuracy: %.2f\nCrit: %.2f", this->attack_power, this->attack_power + this->attack_power_range, this->accuracy, this->crit_chance);
	wrefresh(item_description_window);
}
