#include <string>
#include "weapon.h"
#include "food.h"
#include "enemy.h"
#include "armor.h"
#ifndef PLAYER_H
#define PLAYER_H

class Player 
{
    public:
        //Will contain weapons, supplies, armor, etc
        struct Inventory {
            std::vector<Weapon> weapons;
            std::vector<Food> food;
            std::vector<Armor> armor;

            int weapon_capacity;
            int food_capacity;
            int weapon_count;
            int food_count;
            int armor_capacity;
            int armor_count;
        };

        Player(std::string race);
        bool moveSpace(int direction, int map_size, WINDOW *player_window, std::vector<Enemy> enemies);
        void setPosition(int r, int c);
        void printStatus(WINDOW *player_window);
        int computeAttackPower(Weapon weapon, WINDOW *alert_window);
        void gainExp(int xp, WINDOW *p_window);
        void gainSouls(int souls_to_gain, WINDOW *p_window);
        void printInventory(WINDOW *inv_window, int index, WINDOW *item_window);
        void manageInventory(WINDOW *inv_window, WINDOW *player_window, WINDOW *alert_win, WINDOW *item_description_window, std::vector<Enemy::Loot> *loot);
        void setPrimaryWeapon(Weapon weapon);
        void setSecondaryWeapon(Weapon weapon);
        void eatFood(Food *food, WINDOW *player_window);
        void levelUp(WINDOW *alert_win);
        void foodEvents();
        void passiveHealthRegeneration(WINDOW *p_win);
        bool pickupLoot(Weapon weapon_item);
        bool pickupLoot(Food food_item);
        bool pickupLoot(Armor armor_item);
        bool isInCombat();
        void setInCombatAbsolute(bool toggle);
        void setInCombatCheck(std::vector<Enemy> enemies);
        void printEquipped(WINDOW *inv_window, WINDOW *item_description_window, int index);
        void manageArmor(WINDOW *inv_window, WINDOW *item_description_window, WINDOW *alert_win);
        void unequipArmor(WINDOW *alert_win, int index);
        bool attack(Enemy *enemy, bool usePrimary, WINDOW *alert_win);
        double getArmorScore();
        void setBleedDamage(int damage, int rounds);
        void takeDamageOverTime(WINDOW *alert_window);

        Inventory inventory;
        std::vector<Food> active_food;
        std::vector<Armor> equipped_armor;
        int vision;
        int current_health, base_total_health, current_total_health;
        double speed;
        double crit_chance;
        double accuracy;
        double base_protection;
        double current_protection;
        double base_evasion;
        double current_evasion;
        double souls_multiplier;
        int base_damage;
        int allowed_moves;
        int used_moves;
        int level;
        int level_points;
        int current_xp_cap;
        int current_xp;
        double level_up_multiplier_xp;
        double level_up_multiplier_health;
        double level_up_multiplier_damage;
        bool is_stunned;
        std::string race;
        int row;
        int col;
        int inventory_index;
        int max_inventory_index;
        int souls;
        int souls_cap;
        bool in_combat;

        Weapon *primary_weapon;
        Weapon *secondary_weapon;

    private:
        int passive_health_regen_counter;   //count of number of moves. Rolls back to 0 when equal to trigger.
        int passive_health_regen_trigger;   //number of moves before passive health regeneration happens
        int passive_health_regen_amount;    //amount of health gained on trigger
        int bleed_damage;
        int bleed_rounds;
};

#endif