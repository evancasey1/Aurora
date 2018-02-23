#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <math.h>
#include "weapon.h"
#include "weaponConstants.h"
#include "weaponInstance.h"

Weapon Weapon::randomWeapon(int level)
{
    int roll = rand() % 5;
    /*
    * Not a huge fan of using the if statements like this. I'll 
    * revisit this function later.
    */
    if (roll == 0) {
        Dagger d(level);
        return d;
    }
    else if (roll == 1) {
        Sword s(level);
        return s;
    }
    else if (roll == 2) {
        Mace m(level);
        return m;
    }
    else if (roll == 3) {
        ShortSword ss(level);
        return ss;
    }
    else {
        BlackJack b(level);
        return b;
    }

}

/*
* Returns double between (min/100) and (max/100)
* I.E. getRandInRange(50, 70) would yield values from 0.50 to 0.70
*/
double Weapon::getRandInRange(int min, int max)
{
    return (min + (rand() % static_cast<int>(max - min + 1))) / 100.0;
}

void Weapon::applyLevel(int level)
{
    this->damage *= std::pow(WeaponConstants::LEVEL_MOD, level);
    this->damage_range *= std::pow(WeaponConstants::LEVEL_MOD, level);
}

void Weapon::applyRarity()
{
    double condition_chance = ((double) rand() / RAND_MAX);
    if (condition_chance <= WeaponConstants::SUPERIOR_CHANCE) {
        this->name = "Superior " + this->name;
        this->damage *= WeaponConstants::SUPERIOR_MOD;
        this->damage_range *= WeaponConstants::SUPERIOR_MOD; 
    }
    else if (condition_chance <= WeaponConstants::ENHANCED_CHANCE) {
        this->name = "Enhanced " + this->name;
        this->damage *= WeaponConstants::ENHANCED_MOD;
        this->damage_range *= WeaponConstants::ENHANCED_MOD; 
    }
}

void Weapon::printDescription(WINDOW *win) 
{
    wprintw(win, "%s\nAttack: %d - %d\nAccuracy: %.2f\nCrit: %.2f\nBleed: %.2f\nStun: %.2f", (this->name).c_str(), this->damage, this->damage + this->damage_range, this->accuracy, this->crit_chance, this->bleed_chance, this->stun_chance);
    wrefresh(win);
}
void Weapon::printType(WINDOW *win)
{
    wprintw(win, "WEAPON\n");
    wrefresh(win);
}
