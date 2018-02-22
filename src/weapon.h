#ifndef WEAPON_H
#define WEAPON_H
#include "equipment.h"

class Weapon : public Equipment 
{
    public:
        void printDescription(WINDOW *win);
        void printType(WINDOW *win);
        static Weapon randomWeapon(int level);
        void applyRarity();
        void applyLevel(int level);
        static double getRandInRange(int min, int max);

        int damage, damage_range;
        std::string name;
        double bleed_chance;
        double crit_chance;
        double stun_chance;
        double accuracy;
        int level;
        
};

#endif