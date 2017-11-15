#include <string>
#include <cstdlib>
#include <ctime>
#include "weapon.h"

Weapon::Weapon(std::string w_name, int attack_pow, int attack_pow_range) {
	double condition_chance = ((double) rand() / RAND_MAX);
	
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
	if (w_name == "Sword" || w_name == "Dagger") {
		this->bleed_effect = true;
		if (w_name == "Dagger") {
			this->poison_effect = true;
		}
	}
}