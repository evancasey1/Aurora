#ifndef WEAPONINSTANCE_H
#define WEAPONINSTANCE_H
#include "weapon.h"

class Dagger : public Weapon
{
    public:
        Dagger(int level) 
        {
            extern int item_id_counter;
            this->type = "S";
            this->name = "Dagger";
            this->damage = 2;
            this->damage_range = 2;
            this->level = level;
            this->item_id = item_id_counter++;
            is_primary = false;
            applyRarity();
            applyLevel(level);
            this->bleed_chance = Weapon::getRandInRange(30, 70);
            this->crit_chance = Weapon::getRandInRange(0, 15);
            this->stun_chance = 0.00;
            this->accuracy = Weapon::getRandInRange(70, 99);
            this->attack = *(new Attack("Bleed out"));
        }
};

class ShortSword : public Weapon
{
    public:
        ShortSword(int level) 
        {
            extern int item_id_counter;
            this->type = "S";
            this->name = "Short Sword";
            this->damage = 2;
            this->damage_range = 4;
            this->level = level;
            this->item_id = item_id_counter++;
            is_primary = false;
            applyRarity();
            applyLevel(level);
            this->bleed_chance = Weapon::getRandInRange(5, 10);
            this->crit_chance = Weapon::getRandInRange(0, 15);
            this->stun_chance = Weapon::getRandInRange(0, 5);
            this->accuracy = Weapon::getRandInRange(80, 95);
            this->attack = *(new Attack("Slash"));
        }
};

class BlackJack : public Weapon
{
    public:
        BlackJack(int level) 
        {
            extern int item_id_counter;
            this->type = "S";
            this->name = "Blackjack";
            this->damage = 1;
            this->damage_range = 2;
            this->level = level;
            this->item_id = item_id_counter++;
            is_primary = false;
            applyRarity();
            applyLevel(level);
            this->bleed_chance = 0.00;
            this->crit_chance = Weapon::getRandInRange(0, 5);
            this->stun_chance = Weapon::getRandInRange(50, 80);
            this->accuracy = Weapon::getRandInRange(75, 95);
            this->attack = *(new Attack("Smash"));
        }
};

class Sword : public Weapon
{
    public:
        Sword(int level) 
        {
            extern int item_id_counter;
            this->type = "P";
            this->name = "Sword";
            this->damage = 3;
            this->damage_range = 3;
            this->level = level;
            this->item_id = item_id_counter++;
            is_primary = true;
            applyRarity();
            applyLevel(level);
            this->bleed_chance = Weapon::getRandInRange(0, 10);
            this->crit_chance = Weapon::getRandInRange(0, 10);
            this->stun_chance = Weapon::getRandInRange(0, 5);
            this->accuracy = Weapon::getRandInRange(70, 90);
            this->attack = *(new Attack("Slash"));
        }
};

class Mace : public Weapon
{
    public:
        Mace(int level) 
        {
            extern int item_id_counter;
            this->type = "P";
            this->name = "Mace";
            this->damage = 5;
            this->damage_range = 3;
            this->level = level;
            this->item_id = item_id_counter++;
            is_primary = true;
            applyRarity();
            applyLevel(level);
            this->bleed_chance = Weapon::getRandInRange(0, 15);
            this->crit_chance = Weapon::getRandInRange(0, 13);
            this->stun_chance = Weapon::getRandInRange(5, 15);
            this->accuracy = Weapon::getRandInRange(60, 85);
            this->attack = *(new Attack("Pummel"));
        }
};

class Flail : public Weapon
{
    public:
        Flail(int level) 
        {
            extern int item_id_counter;
            this->type = "P";
            this->name = "Flail";
            this->damage = 3;
            this->damage_range = 4;
            this->level = level;
            this->item_id = item_id_counter++;
            is_primary = true;
            applyRarity();
            applyLevel(level);
            this->bleed_chance = Weapon::getRandInRange(5, 25);
            this->crit_chance = Weapon::getRandInRange(3, 15);
            this->stun_chance = Weapon::getRandInRange(15, 30);
            this->accuracy = Weapon::getRandInRange(70, 90);
            this->attack = *(new Attack("Crush"));
        }
};

class Halberd : public Weapon
{
    public:
        Halberd(int level) 
        {
            extern int item_id_counter;
            this->type = "PS";
            this->name = "Halberd";
            this->damage = 6;
            this->damage_range = 3;
            this->level = level;
            this->item_id = item_id_counter++;
            is_primary = true;
            applyRarity();
            applyLevel(level);
            this->bleed_chance = Weapon::getRandInRange(20, 40);
            this->crit_chance = Weapon::getRandInRange(5, 15);
            this->stun_chance = Weapon::getRandInRange(0, 10);
            this->accuracy = Weapon::getRandInRange(80, 95);
            this->attack = *(new Attack("Cleave"));
        }
};

#endif