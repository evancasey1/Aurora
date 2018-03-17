#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <math.h>
#include <queue>
#include "enemy.h"
#include "color.h"
#include "player.h"
#include "map.h"

extern bool checkIfAttackHit(double acc, double eva);
extern void slowCombat(Player *player, Enemy *enemy, Map map, std::vector<Enemy> enemies);

Enemy::Enemy(std::string e_name, int p_row, int p_col, int p_vision, int map_size, int e_level)
{
    this->name = e_name;
    this->level = e_level;
    std::ifstream infile("enemyAttributes.csv");
    std::string element;

    while(infile.good()) {
        getline(infile, element, ',');
        if (element == e_name) {
            getline(infile, element, ',');
            this->accuracy = std::atof(element.c_str()); 
            getline(infile, element, ',');
            this->crit_chance = std::atof(element.c_str());
            getline(infile, element, ',');
            this->attack_power = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->attack_power_range = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->base_protection = std::atof(element.c_str());
            getline(infile, element, ',');
            this->total_health = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->souls = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->XP = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->idle_moves = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->seek_moves = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->number_drops_possible = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->loot_drop_chance = std::atof(element.c_str());
            getline(infile, element, ',');
            this->vision = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->speed = std::atof(element.c_str());
            getline(infile, element, ',');
            this->base_evasion = std::atof(element.c_str());
            getline(infile, element, ',');
            this->level_up_multiplier_health = std::atof(element.c_str());
            getline(infile, element, ',');
            this->level_up_multiplier_damage = std::atof(element.c_str());
            getline(infile, element, ',');
            this->bleed_resist = std::atof(element.c_str());
            getline(infile, element, '\n');
            this->symbol = element[0];
            infile.close();
            break;
        }
        else {
            getline(infile, element);
        }
    }

    this->current_evasion = this->base_evasion;
    this->current_protection = this->base_protection;
    this->alert_player = true;
    this->nightbuff_multiplier = 1.15;
    this->inCombat = false;
    this->bleed_chance = 0.05;
    this->bleed_damage = 0;
    this->bleed_rounds = 0;
    this->is_stunned = false;

    this->total_health = std::ceil(this->total_health * std::pow(this->level_up_multiplier_health, e_level));
    this->attack_power = std::ceil(this->attack_power * std::pow(this->level_up_multiplier_damage, e_level));
    this->attack_power_range = std::ceil(this->attack_power_range * std::pow(this->level_up_multiplier_damage, e_level));
    this->current_health = this->total_health;


    /*
    *Set enemy coordinates on map
    */
    int e_row = 0;
    int e_col = 0;
    int range = (p_vision * 2);
    int choice = (rand() % 2); //0 or 1
    int op = (rand() % 2);
    switch (choice) {
        case 0:
            if (!op) {
                e_row = p_row + p_vision;
            }
            else {
                e_row = p_row - p_vision;
            }
            e_col = (rand() % range) + p_col;
            break;
        case 1:
            if (!op) {
                e_col = p_col + p_vision;
            }
            else {
                e_col = p_col - p_vision;
            }
            e_row = (rand() % range) + p_row;
            break;
        default:
            break;
    }

    e_row > (map_size - 1) ? e_row = (map_size - 1) : false;
    e_row < 0 ? e_row = 0 : false;
    e_col > (map_size - 1) ? e_col = (map_size - 1) : false;
    e_col < 0 ? e_col = 0 : false;

    this->row = e_row;
    this->col = e_col;
}

int Enemy::computeAttackPower(WINDOW *alert_window) 
{
    int power = this->attack_power;
    int power_range = this->attack_power_range;
    double crit_chance = this->crit_chance;
    
    double crit_roll = ((double) rand() / RAND_MAX);
    if (power_range != 0) {
        power += (rand() % power_range);
    }
    if (crit_chance >= crit_roll) {
        power *= 2;
        wprintw(alert_window, "CRITICAL HIT\n");
        wrefresh(alert_window);
    }

    return (int)power;
}

/*
* During daytime, strengthens all enemies by $mod
*
*/
void Enemy::globalBuff(double mod) 
{
    this->attack_power = (int)(this->attack_power * mod);
    this->attack_power_range = (int)(this->attack_power_range * mod);
    this->speed = (int)(speed * mod);
    this->crit_chance *= mod;
    this->accuracy *= mod;
    this->current_protection *= mod;
    this->current_evasion *= mod;
    this->number_drops_possible *= 2;
}

/*
* Reverses effect of globalBuff by $mod
*
*/
void Enemy::globalDebuff(double mod) 
{
    this->attack_power = (int)(this->attack_power / mod);
    this->attack_power_range = (int)(this->attack_power_range / mod);
    this->speed = (int)(speed / mod);
    this->crit_chance /= mod;
    this->accuracy /= mod;
    this->current_protection = this->base_protection;
    this->current_evasion = this->base_evasion;
    this->number_drops_possible /= 2;
}

/*
* Handles loot dropping when enemies die.
*
*
*/
void Enemy::deathEvents(std::vector<Loot> *loot, WINDOW *alert_win) 
{
    static long unique_loot_id = 0;
    double loot_chance_roll;
    bool dropped = false;
    Loot loot_obj;
    for (int i = 0; i < this->number_drops_possible; i++) {
        loot_chance_roll  = ((double) rand() / RAND_MAX);
        //loot_chance_roll = 0.00;
        if (this->loot_drop_chance >= loot_chance_roll) {
            //Generate loot
            //Drop loot on map
            dropped = true;
            loot_obj.row = this->row;
            loot_obj.col = this->col;
            loot_obj.dropped_by = this->name;
            
            if (this->name == "Wolf") {
                //Food(std::string name, int initial_health_gain, int health_gain_per_trigger, int turns_until_trigger, int total_triggers);
                loot_obj.food.push_back(Food("Wolf Meat", 4, 1, 5, 3));
            }
            else if (this->name == "Bear") {
                loot_obj.food.push_back(Food("Bear Meat", 6, 2, 4, 2));
            }
            else {
                if ((rand() % 2) == 0) {
                    Weapon weapon = Weapon::randomWeapon(this->level);
                    loot_obj.weapons.push_back(weapon);
                }
                else {
                    Armor armor = Armor::randomArmor(this->level);
                    loot_obj.armor.push_back(armor);
                }
            }
        }
    }
    if (dropped) {
        wattroff(alert_win, Color::RedBlack);
        wattron(alert_win, Color::GreenBlack);
        wprintw(alert_win, "%s dropped loot.\n", (this->name).c_str());
        wattroff(alert_win, Color::GreenBlack);
        wattron(alert_win, Color::RedBlack);

        loot_obj.l_id = unique_loot_id;
        loot_obj.despawn_counter = 0;
        unique_loot_id++;
        loot->push_back(loot_obj);
    }
    wrefresh(alert_win);
}

void Enemy::setBleedDamage(int damage, int rounds) 
{
    this->bleed_damage += damage;
    this->bleed_rounds += rounds;
}

void Enemy::takeDamageOverTime(WINDOW *alert_window)
{
    if (this->bleed_rounds > 0) {
        wattron(alert_window, Color::RedBlack);
        wprintw(alert_window, "%s bleeds for %d damage.\n", (this->name).c_str(), this->bleed_damage);
        wattroff(alert_window, Color::RedBlack);
        wrefresh(alert_window);
        this->bleed_rounds--;
        this->current_health -= this->bleed_damage;
    }
    else {
        this->bleed_damage = 0;
    }
}

/*
*Attacks player. Returns true if player is killed, false otherwise
*
*/
bool Enemy::attack(Player *player, WINDOW *alert_window)
{
    bool attack_hit = checkIfAttackHit(this->accuracy, player->current_evasion);
    this->takeDamageOverTime(alert_window);
    if (this->current_health <= 0 || this->is_stunned) {
        if (this->is_stunned) {
            this->is_stunned = false;
        }
        return false;
    }
    
    wattron(alert_window, Color::RedBlack);
    if (attack_hit) {
        int dmg = this->computeAttackPower(alert_window) * (1 - player->current_protection);
        double bleed_roll = ((double) rand() / RAND_MAX);
        if (bleed_roll <= this->bleed_chance) {
            wprintw(alert_window, "You are bleeding.\n");
            player->setBleedDamage(1, 3); //placeholder. Will be based on enemy attacks when implemented
        }
        player->current_health -= dmg;
        wprintw(alert_window, "%s hits you for %d damage.\n", (this->name).c_str(), dmg);
        wrefresh(alert_window);
        wattroff(alert_window, Color::RedBlack);
    }
    else {
        wprintw(alert_window, "%s missed.\n", (this->name).c_str());
        wrefresh(alert_window);
        wattroff(alert_window, Color::RedBlack);
    }
    return player->current_health <= 0;
}

void Enemy::printStatus(WINDOW *combat_window)
{
    wclear(combat_window);
    int max_bars = 20;
    double ratio = (double)this->current_health / (double)this->total_health;
    int nbars = (int) (ratio * max_bars);
    
    wprintw(combat_window, "%s [lvl %d]\n", (this->name).c_str(), this->level);
    if (ratio > 0.75) {
        wattron(combat_window, Color::GreenBlack);
    }
    else if (ratio > 0.30) {
        wattron(combat_window, Color::YellowBlack);
    }
    else {
        wattron(combat_window, Color::RedBlack);
    }

    wattron(combat_window, A_ALTCHARSET);
    for (int i = 0; i < nbars; i++) {
        waddch(combat_window, ACS_CKBOARD);
    }
    wattrset(combat_window, A_NORMAL);

    wprintw(combat_window, "\n\nHP: %d/%d\nPROT: %.0f%%\nBLD RESIST: %.0f%%\n", this->current_health, this->total_health, this->current_protection * 100, this->bleed_resist * 100);
    if (this->bleed_rounds > 0) {
        wattron(combat_window, Color::RedBlack);
        wprintw(combat_window, "BLD %d pts/%d rds", this->bleed_damage, this->bleed_rounds);
        wattroff(combat_window, Color::RedBlack);
    }
    wrefresh(combat_window);
}

bool Enemy::isInCombat() 
{
    return this->inCombat;
}

void Enemy::setInCombat(bool toggle)
{
    this->inCombat = toggle;
}

/*
* Movement pattern when enemy does not spot player
*/
void Enemy::idle(int map_size)
{
    //move a random direction or stay still
    int move;
    for (int i = 0; i < this->idle_moves; i++) {
        move = (rand() % 6);
        switch (move) {
            case 0:
                this->row++;
                break;
            case 1:
                this->row--;
                break;
            case 2:
                this->col++;
                break;
            case 3:
                this->col--;
                break;
            default:
                break;
        }
        //bounding logic
        if (this->row >= map_size) this->row--;
        else if (this->row < 0) this->row++;

        if (this->col >= map_size) this->col--;
        else if (this->col < 0) this->col++;
    }
}

void Enemy::setPosFromID(int id, int size) 
{
    this->row = std::floor(id / size);
    this->col = id % size;
}

void Enemy::seek(Player player, Map map, std::vector<Enemy> enemies)
{
    std::vector<int> trail(map.size * map.size, -1);
    std::queue<int> search_queue;
    int p_row = player.row;
    int p_col = player.col;
    int mods[] = {1, -1, map.size, -map.size}; //right, left, down, up
    int e_node = map.getSectionID(this->row, this->col);
    int p_node = map.getSectionID(p_row, p_col);
    int current;
    bool found = false;
    
    search_queue.push(e_node); //push player tile to queue
    trail.at(e_node) = e_node;
    //endwin();

    while(!search_queue.empty() && !found) {
        current = search_queue.front();
        search_queue.pop();
        for (int i = 0; i < 4; i++) {
            if (map.isValidMove(current + mods[i]) && trail.at(current + mods[i]) == -1) {
                search_queue.push(current + mods[i]);
                trail.at(current + mods[i]) = current;
                if ((current + mods[i]) == p_node) {
                    found = true;
                    break;
                }
            }
        }
    }

    if (found) {
        std::vector<int> path;
        int index = p_node;
        while(trail[index] != e_node) {
            index = trail[index];
            path.push_back(index);
        }
        std::reverse(path.begin(), path.end());
        index = 0;
        while (index < this->seek_moves && index < static_cast<int>(path.size())) {
            setPosFromID(path.at(index), map.size);
            index++;
        }
        if (index < this->seek_moves) {
            slowCombat(&player, this, map, enemies);
        }

    }
    else {
        this->idle(map.size);
    }
}


/*
* Movement pattern when enemy spots player
*/
/*
void Enemy::seek(int p_row, int p_col)
{
    //Not too happy with this randomization solution, but since enemy
    //movement is going to be overhauled soon anyway it'll do
    int choice;
    for (int i = 0; i < this->seek_moves; i++){ 
        choice = rand() % 2;
        if (choice) {
            if (p_row > this->row) {
                this->row++;
            }
            else if (p_row < this->row) {
                this->row--;
            }
            else if (p_col > this->col) {
                this->col++;
            }
            else if (p_col < this->col) {
                this->col--;
            }
        }
        else {
            if (p_col > this->col) {
                this->col++;
            }
            else if (p_col < this->col) {
                this->col--;
            }
            else if (p_row > this->row) {
                this->row++;
            }
            else if (p_row < this->row) {
                this->row--;
            }
        }
        
    }
}
*/