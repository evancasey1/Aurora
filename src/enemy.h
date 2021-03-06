#include <string>
#include "weapon.h"
#include "food.h"
#include "equipment.h"
#include "armor.h"

#ifndef ENEMY_H
#define ENEMY_H
class Player;
class Map;

class Enemy
{
    public:
        struct Loot {
            std::string dropped_by;
            std::vector<Weapon> weapons;
            std::vector<Food> food;
            std::vector<Armor> armor;

            int row;
            int col;
            long l_id;
            int despawn_counter;
        };
        
        Enemy(std::string en, int rw, int cl, int vi, int sz, int lvl);
        //void seek(int p_row, int p_col);
        void seek(Player player, Map map, std::vector<Enemy> enemies);
        void idle(Map map, std::vector<Enemy> enemies);
        int computeAttackPower(WINDOW *alert_window);
        void deathEvents(std::vector<Enemy::Loot> *loot, WINDOW *win);
        void globalBuff(double mod);
        void globalDebuff(double mod);
        bool isInCombat();
        void setInCombat(bool toggle);
        bool attack(Player *player, WINDOW *alert_win);
        void printStatus(WINDOW *combat_window);
        void setBleedDamage(int damage, int rounds);
        void takeDamageOverTime(WINDOW *alert_window);
        void setPosFromID(int id, int size);
        void setPassive(int turns);

        int row;
        int souls;
        int col;
        int level;
        int vision;
        int current_health;
        int total_health;
        int attack_power;
        int attack_power_range;
        int XP;
        double nightbuff_multiplier;
        double speed;
        double crit_chance;
        double accuracy;
        double base_protection;
        double current_protection;
        double base_evasion;
        double current_evasion;
        bool alert_player;
        char symbol;
        double loot_drop_chance;
        int number_drops_possible;
        double level_up_multiplier_health;
        double level_up_multiplier_damage;
        std::string name;
        int seek_moves;
        int idle_moves;
        bool inCombat;
        double bleed_chance;
        double bleed_resist;
        int bleed_damage;
        int bleed_rounds;
        int passive_turns;
        bool is_stunned;
        bool attacking;

        struct Equipment {
            std::vector<Weapon> weapons;
        };

};

#endif