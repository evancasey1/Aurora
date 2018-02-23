#ifndef ARMORINSTANCE_H
#define ARMORINSTANCE_H
#include "armor.h"

/*
* The reason there needs to be a class for each armor type is
* that in the future they will each have unique attributes 
* that encompass more than variable protection. This is a preemptive
* refactoring to make that future shift easier.
*/

class Helmet : public Armor
{
    public:
        Helmet(int level) 
        {
            extern int item_id_counter;
            this->item_id = item_id_counter++;
            this->armor_type = ArmorType::Helmet;
            this->name = "Helmet";
            this->equipment_id = static_cast<int>(EquipmentType::Armor);
            this->level = level;
            this->protection = getRandInRange(5, 10);
            applyRarity();
            applyLevel(level);
        }
};

class Chestpiece : public Armor
{
    public:
        Chestpiece(int level) 
        {
            extern int item_id_counter;
            this->item_id = item_id_counter++;
            this->armor_type = ArmorType::Chestpiece;
            this->name = "Chestpiece";
            this->equipment_id = static_cast<int>(EquipmentType::Armor);
            this->level = level;
            this->protection = getRandInRange(10, 20);
            applyRarity();
            applyLevel(level);
        }
};

class Gloves : public Armor
{
    public:
        Gloves(int level) 
        {
            extern int item_id_counter;
            this->item_id = item_id_counter++;
            this->armor_type = ArmorType::Gloves;
            this->name = "Gloves";
            this->equipment_id = static_cast<int>(EquipmentType::Armor);
            this->level = level;
            this->protection = getRandInRange(5, 10);
            applyRarity();
            applyLevel(level);
        }
};

class Leggings : public Armor
{
    public:
        Leggings(int level) 
        {
            extern int item_id_counter;
            this->item_id = item_id_counter++;
            this->armor_type = ArmorType::Leggings;
            this->name = "Leggings";
            this->equipment_id = static_cast<int>(EquipmentType::Armor);
            this->level = level;
            this->protection = getRandInRange(10, 20);
            applyRarity();
            applyLevel(level);
        }
};

class Boots : public Armor
{
    public:
        Boots(int level) 
        {
            extern int item_id_counter;
            this->item_id = item_id_counter++;
            this->armor_type = ArmorType::Boots;
            this->name = "Boots";
            this->equipment_id = static_cast<int>(EquipmentType::Armor);
            this->level = level;
            this->protection = getRandInRange(5, 10);
            applyRarity();
            applyLevel(level);
        }
};

#endif