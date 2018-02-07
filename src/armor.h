#include "equipment.h"
#include "armortype.h"
#ifndef ARMOR_H
#define ARMOR_H

class Armor : public Equipment 
{
    public:
        Armor();
        Armor(int level);
        Armor(int level, std::string name, double prot);
        void printDescription(WINDOW *win);
        void printType(WINDOW *win);

        int level;
        double protection;
        ArmorType armor_type;
        std::string name;
};

#endif