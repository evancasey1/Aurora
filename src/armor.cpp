#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <math.h>
#include "armor.h"
#include "armortype.h"
#include "equipmenttype.h"
#include "armorConstants.h"
#include "armorInstance.h"

Armor::Armor() 
{
    extern int item_id_counter;
    this->item_id = item_id_counter++;
    this->armor_type = ArmorType::Default;
    this->equipment_id = static_cast<int>(EquipmentType::Armor);
    this->level = 0;
    this->protection = 0.00;
    this->name = "<NONE>";
}

Armor Armor::randomArmor(int level)
{
    int roll = rand() % 5;
    /*
    * Not a huge fan of using the if statements like this. I'll 
    * revisit this function later.
    */
    if (roll == 0) {
        Helmet h(level);
        return h;
    }
    else if (roll == 1) {
        Chestpiece c(level);
        return c;
    }
    else if (roll == 2) {
        Gloves g(level);
        return g;
    }
    else if (roll == 3) {
        Leggings l(level);
        return l;
    }
    else {
        Boots b(level);
        return b;
    }

}

/*
* Returns double between (min/100) and (max/100)
* I.E. getRandInRange(50, 70) would yield values from 0.50 to 0.70
*/
double Armor::getRandInRange(int min, int max)
{
    return (min + (rand() % static_cast<int>(max - min + 1))) / 100.0;
}

void Armor::applyLevel(int level)
{
    this->protection *= std::pow(ArmorConstants::LEVEL_MOD, level);
}

void Armor::applyRarity()
{
    double condition_chance = ((double) rand() / RAND_MAX);
    if (condition_chance <= ArmorConstants::SUPERIOR_CHANCE) {
        this->name = "Superior " + this->name;
        this->protection *= ArmorConstants::SUPERIOR_MOD;
    }
    else if (condition_chance <= ArmorConstants::ENHANCED_CHANCE) {
        this->name = "Enhanced " + this->name;
        this->protection *= ArmorConstants::ENHANCED_MOD;
    }
}

void Armor::printType(WINDOW *win) 
{
    wprintw(win, "ARMOR\n");
    wrefresh(win);
}

void Armor::printDescription(WINDOW *item_description_window)
{
    wprintw(item_description_window, "%s\nProtection: %.2f", (this->name).c_str(), this->protection);
    wrefresh(item_description_window);
}