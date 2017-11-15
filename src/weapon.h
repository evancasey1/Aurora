#ifndef WEAPON_H
#define WEAPON_H

class Weapon {
	public:
		Weapon(std::string name, int attack_power, int attack_power_range);
		int attack_power, attack_power_range;
		std::string name;
		bool is_poisonous;
		double bleed_chance, poison_chance;
		double piercing_mod, slashing_mod, crushing_mod;

	private:
		const double ENHANCED_CHANCE  	  = 0.35;
		const double LEGENDARY_CHANCE  = 0.15;
		const double ENHANCED_MOD 	      = 1.25;
		const double LEGENDARY_MOD     = 1.5;
};

#endif