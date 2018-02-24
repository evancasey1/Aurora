#ifndef WEAPON_H
#define WEAPON_H
#include "equipment.h"

class Weapon : public Equipment 
{
    public:
        void printDescription(WINDOW *win);
        void printType(WINDOW *win);
        void applyRarity();
        void applyLevel(int level);
        void compareTo(Weapon w, WINDOW *idw);
        static double getRandInRange(int min, int max);
        static Weapon randomWeapon(int level);


        int damage, damage_range;
        std::string name;
        double bleed_chance;
        double crit_chance;
        double stun_chance;
        double accuracy;
        int level;
        bool is_primary;
        
};

#endif