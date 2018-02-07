#ifndef WEAPON_H
#define WEAPON_H
#include "equipment.h"

class Weapon : public Equipment 
{
	public:
		Weapon();
		Weapon(int level);
		Weapon(std::string name, int attack_power, int attack_power_range);
		void printDescription(WINDOW *win);
		void printType(WINDOW *win);
				
		int attack_power, attack_power_range;
		std::string name;
		bool is_poisonous;
		double bleed_chance, poison_chance;
		double crit_chance;
		double accuracy;

	private:
		void applyAttributes(std::string name);
		void applyStatusBuff(std::string w_name, int ap, int apr);
		double ENHANCED_CHANCE;
		double LEGENDARY_CHANCE;
		double ENHANCED_MOD;
		double LEGENDARY_MOD;
};

#endif