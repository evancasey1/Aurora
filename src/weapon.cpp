#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <math.h>
#include "weapon.h"
#include "weaponConstants.h"
#include "weaponInstance.h"
#include "color.h"

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

/*
* Violating a lot of DRY principles here, will revisit
*/
void Weapon::compareTo(Weapon weapon, WINDOW *win)
{
    wprintw(win, "%s [%s]\nLevel: ", (this->name).c_str(), (this->type).c_str());
    if (this->level < weapon.level) {
        wattron(win, Color::RedBlack);
    }
    else if (this->level > weapon.level) {
        wattron(win, Color::GreenBlack);
    }
    wprintw(win, "%d\n", this->level);
    wattrset(win, A_NORMAL);
    wprintw(win, "Attack: ");

    if (this->damage + this->damage_range < weapon.damage + weapon.damage_range) {
        wattron(win, Color::RedBlack);
    }
    else if (this->damage + this->damage_range > weapon.damage + weapon.damage_range) {
        wattron(win, Color::GreenBlack);
    }
    wprintw(win, "%d\n", this->damage + this->damage_range);
    wattrset(win, A_NORMAL);
    wprintw(win, "Accuracy: ");

    if (this->accuracy < weapon.accuracy) {
        wattron(win, Color::RedBlack);
    }
    else if (this->accuracy > weapon.accuracy) {
        wattron(win, Color::GreenBlack);
    }
    wprintw(win, "%.0f%%\n", this->accuracy * 100);
    wattrset(win, A_NORMAL);
    wprintw(win, "Crit: ");

    if (this->crit_chance < weapon.crit_chance) {
        wattron(win, Color::RedBlack);
    }
    else if (this->crit_chance > weapon.crit_chance) {
        wattron(win, Color::GreenBlack);
    }
    wprintw(win, "%.0f%%\n", this->crit_chance * 100);
    wattrset(win, A_NORMAL);
    wprintw(win, "Bleed: ");

    if (this->bleed_chance < weapon.bleed_chance) {
        wattron(win, Color::RedBlack);
    }
    else if (this->bleed_chance > weapon.bleed_chance) {
        wattron(win, Color::GreenBlack);
    }
    wprintw(win, "%.0f%%\n", this->bleed_chance * 100);
    wattrset(win, A_NORMAL);
    wprintw(win, "Stun: ");

    if (this->stun_chance < weapon.stun_chance) {
        wattron(win, Color::RedBlack);
    }
    else if (this->stun_chance > weapon.stun_chance) {
        wattron(win, Color::GreenBlack);
    }
    wprintw(win, "%.0f%%\n", this->stun_chance * 100);
    wattrset(win, A_NORMAL);

    mvwprintw(win, 9, 0, "Currently Equipped:\n");
    weapon.printDescription(win);
}

void Weapon::printDescription(WINDOW *win) 
{
    wprintw(win, "%s [%s]\nLevel: %d\nAttack: %d - %d\nAccuracy: %.0f%%\nCrit: %.0f%%\nBleed: %.0f%%\nStun: %.0f%%", (this->name).c_str(), (this->type).c_str(), this->level, this->damage, this->damage + this->damage_range, this->accuracy * 100, this->crit_chance * 100, this->bleed_chance * 100, this->stun_chance * 100);
    wrefresh(win);
}
void Weapon::printType(WINDOW *win)
{
    wprintw(win, "WEAPON\n");
    wrefresh(win);
}
