#ifndef ARMOR_H
#define ARMOR_H
#include "equipment.h"
#include "armortype.h"

class Armor : public Equipment 
{
    public:
        Armor(); // needs to exist to create default armor
        Armor(int level);
        void printDescription(WINDOW *win);
        void printType(WINDOW *win);
        static Armor randomArmor(int level);
        static double getRandInRange(int min, int max);
        void applyRarity();
        void applyLevel(int level);

        int level;
        double protection;
        ArmorType armor_type;
        std::string name;
};

#endif