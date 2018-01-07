#ifndef WEAPON_H
#define WEAPON_H
#include "equipment.h"

class Weapon : public Equipment 
{
	public:
		Weapon();
		Weapon(int level);
		Weapon(std::string name, int attack_power, int attack_power_range);
		int attack_power, attack_power_range;
		std::string name;
		bool is_poisonous;
		double bleed_chance, poison_chance;
		double piercing_mod, slashing_mod, crushing_mod;
		double crit_chance;
		double accuracy;


	private:
		double ENHANCED_CHANCE;
		double LEGENDARY_CHANCE;
		double ENHANCED_MOD;
		double LEGENDARY_MOD;
};

#endif