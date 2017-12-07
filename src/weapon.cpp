#include <string>
#include <cstdlib>
#include <ctime>
#include "weapon.h"

Weapon::Weapon() {
	//Random weapon generation
	this->ENHANCED_CHANCE = 0.35;
	this->LEGENDARY_CHANCE = 0.15;
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
		this->piercing_mod = 1.1;
		this->slashing_mod = 1.3;
		this->crushing_mod = 1.0;
		this->accuracy = 0.85;
	}
	else if (w_name == "Dagger") {
		this->bleed_chance = 0.25;
		this->piercing_mod = 1.4;
		this->slashing_mod = 1.1;
		this->crushing_mod = 1.0;
		this->accuracy = 0.95;
	}
	else {
		this->bleed_chance = 0.05;
		this->piercing_mod = 1.0;
		this->slashing_mod = 1.0;
		this->crushing_mod = 1.8;
		this->accuracy = 0.80;
	}
	this->accuracy = 0.93;
}

Weapon::Weapon(std::string w_name, int attack_pow, int attack_pow_range) {
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
		this->piercing_mod = 1.1;
		this->slashing_mod = 1.3;
		this->crushing_mod = 1.0;
		this->accuracy = 0.85;
	}
	else if (w_name == "Dagger") {
		this->bleed_chance = 0.25;
		this->piercing_mod = 1.4;
		this->slashing_mod = 1.1;
		this->crushing_mod = 1.0;
		this->accuracy = 0.95;
	}
	else {
		this->bleed_chance = 0.05;
		this->piercing_mod = 1.0;
		this->slashing_mod = 1.0;
		this->crushing_mod = 1.8;
		this->accuracy = 0.80;
	}
	this->accuracy = 0.94;
}

double Weapon::getAccuracy() 
{
	return (double)this->accuracy;
}
