#include <iostream>
#include <string>
#include <ncurses.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <boost/variant.hpp>
#include "visitors.h"
#include "player.h"
#include "enemy.h"
#include "color.h"
#include "equipmenttype.h"
#include "weaponInstance.h"

extern bool checkIfAttackHit(double acc, double eva);

Player::Player(std::string p_class)
{
    for (int j = 0; j < 5; j++) {
        Armor defArm;
        equipped_armor.push_back(defArm);
    }
    this->vision = 6;
    this->primary_weapon = new Sword(1);
    this->secondary_weapon = new Dagger(1);

    this->inventory.food_capacity = 5;
    this->inventory.weapon_capacity = 3;
    this->inventory.armor_capacity = 3;
    this->inventory.food_count = 0;
    this->inventory.armor_count = 0;
    this->inventory.weapon_count = 0;

    this->race = p_class;
    std::string element;
    std::ifstream infile("playerAttributes.csv");
    while(infile.good()) {
        getline(infile, element, ',');
        if (element == p_class) {
            getline(infile, element, ',');
            this->accuracy = std::atof(element.c_str()); 
            getline(infile, element, ',');
            this->speed = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->base_damage = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->allowed_moves = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->base_evasion = std::atof(element.c_str());
            getline(infile, element, ',');
            this->base_protection = std::atof(element.c_str());
            getline(infile, element, ',');
            this->base_total_health = std::atoi(element.c_str());
            getline(infile, element, ',');
            this->level_up_multiplier_health = std::atof(element.c_str());
            getline(infile, element, ',');
            this->crit_chance = std::atof(element.c_str());
            getline(infile, element, '\n');
            this->level_up_multiplier_damage = std::atof(element.c_str());
            infile.close();
            break;
        }
        else {
            getline(infile, element);
        }
    }

    //placeholder. For debugging only
    this->base_total_health = 5000;
    
    //These attributes are constant for all player types
    this->current_total_health = this->base_total_health;
    this->current_health = this->base_total_health;
    this->used_moves = 0;
    this->level_up_multiplier_xp = 1.25;
    this->current_xp_cap = 150;
    this->level = 1;
    this->level_points = 0;
    this->current_xp = 0;
    this->passive_health_regen_counter = 0;
    this->passive_health_regen_trigger = 50;
    this->passive_health_regen_amount = 1;
    this->inventory_index = 0;
    this->max_inventory_index = 2;
    this->souls_cap = 25;
    this->souls = 0;
    this->souls_multiplier = 1.15;
    this->current_evasion = this->base_evasion;
    this->current_protection = this->base_protection;
    this->inCombat = false;
}

int getLootIndex(std::vector<Enemy::Loot> *loot, long loot_id) 
{
    for (int i = 0; i < loot->size(); i++) {
        if (loot->at(i).l_id == loot_id) {
            return i;
        }
    }
    return -1;
}

/*
*foodEvents()
*   Goes through the list of the food items that the player has eaten,
*   and for each item checks if the item should trigger health gain (and if so, gives health to player),
*   increments the given counters, and deletes the food item if it should expire.
*/
void Player::foodEvents() 
{
    bool deleted;
    std::vector<Food>::iterator food_iter;
    for (food_iter = this->active_food.begin(); food_iter != this->active_food.end();) {
        deleted = false;
        food_iter->turn_count++;
        if (food_iter->turn_count % food_iter->turns_until_trigger) {
            food_iter->trigger_count++;
            this->current_health += food_iter->health_gain_per_trigger;
            if (food_iter->trigger_count == food_iter->total_triggers) {
                food_iter = this->active_food.erase(food_iter);
                deleted = true;
            }
        }
        if (!deleted) {
            food_iter++;
        }
    }
}

/*
*eatFood(Food, WINDOW*)
*   Gets passed a food item, adds it to the player's active_food vector, 
*   and gives initial health boost to player.
*/
void Player::eatFood(Food *food_item, WINDOW *player_status_window) 
{
    this->active_food.push_back(*food_item);
    this->current_health += food_item->initial_health_gain;
    if (this->current_health > this->current_total_health) {
        this->current_health = this->current_total_health;
    }
    this->printStatus(player_status_window);
    this->inventory.food_count--;
    return;
}

void Player::setPrimaryWeapon(Weapon weapon) 
{
    *this->primary_weapon = weapon;
}

void Player::setSecondaryWeapon(Weapon weapon)
{
    *this->secondary_weapon = weapon;
}

/*
*printInventory(WINDOW*, int, WINDOW*)
*   Prints player's inventory in a categorical manner according to equipment.
*   Simply goes down the corresponding list (Weapon, Food, Armor, etc), prints a counter
*   and the item's name and description
*/
void Player::printInventory(WINDOW *inv_window, int index, WINDOW *item_description_window) 
{
    wclear(inv_window);
    wclear(item_description_window);
    int counter = 0;
    std::vector<boost::variant<Weapon, Food, Armor>> equipment;
    std::vector<Equipment>::iterator iter;
    boost::variant<Weapon, Food, Armor> current_equipped;

    wattron(inv_window, A_BOLD);
    
    switch(this->inventory_index) {
        case static_cast<int>(EquipmentType::Weapon):
            equipment.insert(equipment.end(), this->inventory.weapons.begin(), this->inventory.weapons.end());
            wprintw(inv_window, "WEAPONS [%d/%d] ->\n", this->inventory.weapon_count, this->inventory.weapon_capacity);
            current_equipped = *this->primary_weapon;
            break;
        case static_cast<int>(EquipmentType::Food):
            equipment.insert(equipment.end(), this->inventory.food.begin(), this->inventory.food.end());
            wprintw(inv_window, "<- FOOD [%d/%d]\n", this->inventory.food_count, this->inventory.food_capacity);
            current_equipped = *(new Food());
            break;
        case static_cast<int>(EquipmentType::Armor):
            equipment.insert(equipment.end(), this->inventory.armor.begin(), this->inventory.armor.end());
            wprintw(inv_window, "<- ARMOR [%d/%d] ->\n", this->inventory.armor_count, this->inventory.armor_capacity);
            if (this->inventory.armor.size() > 0) {
                current_equipped = equipped_armor.at(static_cast<int>(this->inventory.armor.at(index).armor_type));
            }
            break;
        default:
            return;
    }
    
    wattroff(inv_window, A_BOLD);

    if (equipment.size() == 0) {
        wprintw(inv_window, "<EMPTY>");
    }
    else {
        for (int i = 0; i < equipment.size(); i++) {
            if (counter == index) {
                wattron(inv_window, A_STANDOUT);
                boost::apply_visitor(Visitors::compare_to(current_equipped, item_description_window), equipment.at(i));
            }
            boost::apply_visitor(Visitors::output_list_name(counter, inv_window), equipment.at(i));
            if (counter == index) {
                wattroff(inv_window, A_STANDOUT);
            }
            counter++;
        }
    }
    wrefresh(inv_window);
}

/*
*findValidLootID(std::vector<Enemy::Loot>)
*   Returns a valid (unused at time of creation) loot id for use when the player drops 
*   an item and it is added to the loot table.
*/
int findValidLootID(std::vector<Enemy::Loot> loot)
{
    std::vector<int> used_ids;
    int new_id = 0;
    for (int i = 0; i < loot.size(); i++) {
        used_ids.push_back(loot.at(i).l_id);
    }
    while(true) {
        if (!(std::find(used_ids.begin(), used_ids.end(), new_id) != used_ids.end())) {
            return new_id;
        }
        new_id++;
    }
}

/*
*manageInventory(WINDOW*, WINDOW*, WINDOW*, std::vector<Enemy::Loot>*)
*   Main inventory management system of player.
*   Player can switch between windows for each type of equipment (Armor, Weapon, Food, ...) using left and right arrow keys
*   <ENTER>: allows player to equip Weapon/Armor or eat Food
*   <d>: allows player to drop an item, will be put into world as a standard loot object 
*   <KEY_UP>/<KEY_DOWN>: Allows player to navigate menus
*/
void Player::manageInventory(WINDOW *inv_window, WINDOW *player_status_window, WINDOW *alert_win, WINDOW *item_description_window, std::vector<Enemy::Loot> *loot)
{
    int ch;
    int index = 0;
    Weapon wtemp;
    Armor atemp;
    Enemy::Loot loot_obj;
    bool valid_drop = false;
    std::vector<Weapon> *weapon_vect;
    //WINDOW *item_description_window = newwin(30, 30, 22, 60);
    while (true) {
        //temp = NULL;
        printInventory(inv_window, index, item_description_window);
        ch = getch();
        switch(ch) {
            //player movement
            case KEY_UP:
                if (index > 0) {
                    index--;
                }
                break;
            case KEY_DOWN:
                if (this->inventory_index == static_cast<int>(EquipmentType::Weapon) && index < this->inventory.weapons.size() - 1) {
                    index++;
                }
                else if (this->inventory_index == static_cast<int>(EquipmentType::Food) && index < this->inventory.food.size() - 1) {
                    index++;
                }
                else if (this->inventory_index == static_cast<int>(EquipmentType::Armor) && index < this->inventory.armor.size() - 1) {
                    index++;
                }
                break;
            case KEY_RIGHT:
                if (this->inventory_index < this->max_inventory_index) {
                    this->inventory_index++;
                    index = 0;
                    wclear(item_description_window);
                    wrefresh(item_description_window);
                }
                break;
            case KEY_LEFT:
                if (this->inventory_index > 0) {
                    this->inventory_index--;
                    index = 0;
                    wclear(item_description_window);
                    wrefresh(item_description_window);
                }
                break;
            case KEY_ENTER: case '\n':
                if (this->inventory_index == static_cast<int>(EquipmentType::Weapon) && this->inventory.weapons.size() > 0) {
                    weapon_vect = &this->inventory.weapons;
                    wtemp = weapon_vect->at(index);

                    if (wtemp.type == "P") {
                        weapon_vect->at(index) = *this->primary_weapon;
                        this->setPrimaryWeapon(wtemp);
                    }
                    else {
                        weapon_vect->at(index) = *this->secondary_weapon;
                        this->setSecondaryWeapon(wtemp);
                    }
                    
                    wprintw(alert_win, "Equipped %s\n", (wtemp.name).c_str());
                    wrefresh(alert_win);
                    index = 0;
                }
                else if (this->inventory_index == static_cast<int>(EquipmentType::Food) && this->inventory.food.size() > 0) {
                    this->eatFood(&this->inventory.food.at(index), player_status_window);
                    this->inventory.food.erase(this->inventory.food.begin() + index);
                    index = 0;
                }
                else if (this->inventory_index == static_cast<int>(EquipmentType::Armor) && this->inventory.armor.size() > 0) {
                    atemp = this->inventory.armor.at(index);
                    this->inventory.armor.erase(this->inventory.armor.begin() + index);
                    if (equipped_armor.at(static_cast<int>(atemp.armor_type)).armor_type != ArmorType::Default) {
                        this->inventory.armor.push_back(equipped_armor.at(static_cast<int>(atemp.armor_type)));
                    }
                    else {
                        this->inventory.armor_count--;
                    }
                    equipped_armor.at(static_cast<int>(atemp.armor_type)) = atemp;
                    wprintw(alert_win, "Equipped %s\n", (atemp.name).c_str());
                    wrefresh(alert_win);  
                    wclear(item_description_window);
                    wrefresh(item_description_window); 
                    index = 0; 
                }
                break;
            case 'd':
                loot_obj.food.clear();
                loot_obj.weapons.clear();
                loot_obj.armor.clear();

                /*
                TODO: reduce repetition here with a boost static visitor
                */
                if (this->inventory_index == static_cast<int>(EquipmentType::Weapon) && this->inventory.weapons.size() > 0) {
                    loot_obj.weapons.push_back(this->inventory.weapons.at(index));
                    this->inventory.weapons.erase(this->inventory.weapons.begin() + index);
                    this->inventory.weapon_count--;
                    valid_drop = true;
                }
                else if (this->inventory_index == static_cast<int>(EquipmentType::Food) && this->inventory.food.size() > 0) {
                    loot_obj.food.push_back(this->inventory.food.at(index));
                    this->inventory.food.erase(this->inventory.food.begin() + index);   
                    this->inventory.food_count--;
                    valid_drop = true;
                }
                else if (this->inventory_index == static_cast<int>(EquipmentType::Armor) && this->inventory.armor.size() > 0) {
                    loot_obj.armor.push_back(this->inventory.armor.at(index));
                    this->inventory.armor.erase(this->inventory.armor.begin() + index); 
                    this->inventory.armor_count--;
                    valid_drop = true;
                }

                if (valid_drop) {
                    loot_obj.l_id = findValidLootID(*loot);
                    loot_obj.row = this->row;
                    loot_obj.col = this->col;
                    loot_obj.dropped_by = this->race;
                    loot_obj.despawn_counter = 0;
                    loot->push_back(loot_obj);
                    index = 0;
                    valid_drop = false;
                }
                break;
            case 'e': case 'l': case 'a':
                wclear(inv_window);
                wclear(item_description_window);
                wrefresh(inv_window);
                wrefresh(item_description_window);
                return;
            default:
                break;
        }
    }
    wclear(item_description_window);
    wrefresh(item_description_window);
}

void Player::printEquipped(WINDOW *inv_window, WINDOW *item_description_window, int index)
{
    wclear(inv_window);
    wclear(item_description_window);
    std::string armor_names[] = {"[BOOT]", "[LEGS]", "[CHST]", "[GLVS]", "[HELM]"};
    wattron(inv_window, A_BOLD);
    wprintw(inv_window, "EQUIPPED ARMOR\n");
    wattroff(inv_window, A_BOLD);

    for (int i = 0; i < this->equipped_armor.size(); i++) {
        if (i == index) {
            wattron(inv_window, A_STANDOUT);
            this->equipped_armor.at(i).printDescription(item_description_window);
        }
        wprintw(inv_window, " %s %s\n", armor_names[i].c_str(), this->equipped_armor.at(i).name.c_str());
        wattroff(inv_window, A_STANDOUT);
    }

    wattron(inv_window, A_BOLD);
    wprintw(inv_window, "\nEQUIPPED WEAPONS\n");
    wattroff(inv_window, A_BOLD);

    wprintw(inv_window, "[PRIM] %s\n", (this->primary_weapon->name).c_str());
    wprintw(inv_window, "[SCND] %s", (this->secondary_weapon->name).c_str());

    wrefresh(inv_window);
    wrefresh(item_description_window);
}

void Player::unequipArmor(WINDOW *alert_win, int index)
{
    if (this->equipped_armor.at(index).name != "<NONE>") {
        if ((this->inventory.armor_count < this->inventory.armor_capacity)) {
            Armor defaultArmor;
            wprintw(alert_win, "Unequipped %s.\n", this->equipped_armor.at(index).name.c_str());
            wrefresh(alert_win);
            this->inventory.armor.push_back(this->equipped_armor.at(index));
            this->equipped_armor.at(index) = defaultArmor;
            this->inventory.armor_count++;
        }
        else {
            wprintw(alert_win, "No room in inventory for %s.\n", this->equipped_armor.at(index).name.c_str());
            wrefresh(alert_win);
        }
    }
}

/*
*manageArmor(WINDOW*, WINDOW*, WINDOW*)
*When 'a' is pressed in the main game loop, currently equipped armor is displayed using this function.
*The player can navigate the equipped armor and unequip it and check statistics.
*/
void Player::manageArmor(WINDOW *inv_window, WINDOW *item_description_window, WINDOW *alert_win)
{
    int ch;
    int index = 0;

    while (true) {
        printEquipped(inv_window, item_description_window, index);
        ch = getch();
        switch(ch) {
            case KEY_UP:
                if (index > 0) {
                    index--;
                }
                break;
            case KEY_DOWN:
                if (index < (this->equipped_armor.size() - 1)) {
                    index++;
                }
                break;
            case KEY_ENTER: case '\n':
                this->unequipArmor(alert_win, index);
                break;
            case 'a': case 'e':
                wclear(inv_window);
                wclear(item_description_window);
                wrefresh(inv_window);
                wrefresh(item_description_window);
                return;
            default:
                break;
        }
    }
}

void Player::gainSouls(int souls_to_gain, WINDOW *alert_win)
{
    this->souls += souls_to_gain;
    if (this->souls > souls_cap) {
        this->souls = souls_cap;
    }
    wattroff(alert_win, Color::RedBlack);
    wattron(alert_win, Color::GreenBlack);
    wprintw(alert_win, "Acquired %d souls.\n", souls_to_gain);
    wrefresh(alert_win);
    wattroff(alert_win, Color::GreenBlack);
    wattron(alert_win, Color::RedBlack);
}

bool Player::pickupLoot(Weapon weapon_item) 
{
    if (this->inventory.weapon_count < this->inventory.weapon_capacity) {
        this->inventory.weapons.push_back(weapon_item);
        this->inventory.weapon_count++;
        return true;
    }
    return false;
}

bool Player::pickupLoot(Food food_item) 
{
    if (this->inventory.food_count < this->inventory.food_capacity) {
        this->inventory.food.push_back(food_item);
        this->inventory.food_count++;
        return true;
    }
    return false;
}

bool Player::pickupLoot(Armor armor_item) 
{
    if (this->inventory.armor_count < this->inventory.armor_capacity) {
        this->inventory.armor.push_back(armor_item);
        this->inventory.armor_count++;
        return true;
    }
    return false;
}

void Player::levelUp(WINDOW *alert_win)
{
    this->current_xp -= this->current_xp_cap;
    this->level++;
    this->current_xp_cap *= this->level_up_multiplier_xp;

    wattroff(alert_win, Color::RedBlack);
    wattron(alert_win, Color::CyanBlack);
    wprintw(alert_win, "Level up! %d -> %d\n", this->level - 1, this->level);
    wrefresh(alert_win);
    wattroff(alert_win, Color::CyanBlack);
    wattron(alert_win, Color::RedBlack);
    
    this->base_total_health = std::ceil(this->base_total_health * this->level_up_multiplier_health);
    this->base_damage = std::ceil(this->base_damage * this->level_up_multiplier_damage);
    this->current_total_health = this->base_total_health;
    this->level_points++;
}

void Player::gainExp(int xp, WINDOW *alert_win) 
{
    this->current_xp += xp;
    if (this->current_xp >= this->current_xp_cap) {
        this->levelUp(alert_win);
    }
}

void Player::setPosition(int row, int col)
{
    this->row = row;
    this->col = col;
}

void Player::setInCombatAbsolute(bool toggle)
{
    this->inCombat = toggle;
}

void Player::setInCombatCheck(std::vector<Enemy> enemies)
{
    for (Enemy e : enemies){
        if (e.isInCombat()) {
            this->inCombat = true;
            return;
        }
    }
    this->inCombat = false;
}

bool Player::isInCombat()
{
    return this->inCombat;
}

//Will print things like player gold count, health, status, and 
//directional indicators to next obelisk
void Player::printStatus(WINDOW *player_window)
{
    wmove(player_window, 0, 0);
    wclrtoeol(player_window);
    double ratio = (double)this->current_health / (double)this->current_total_health;

    if (ratio > 0.75) {
        wattron(player_window, Color::GreenBlack);
    }
    else if (ratio > 0.30) {
        wattron(player_window, Color::YellowBlack);
    }
    else {
        wattron(player_window, Color::RedBlack);
    }

    wprintw(player_window, "Health: ");
    //If health is below a certain threshold display it in red
    wprintw(player_window, "%d/%d\n", this->current_health, this->current_total_health);
    wattrset(player_window, A_NORMAL);

    wprintw(player_window, "Level: %d        XP: %d/%d\n", this->level, this->current_xp, this->current_xp_cap);
    if (this->souls == this->souls_cap) {
        wattron(player_window, Color::YellowBlack);
    }
    wprintw(player_window, "SOULS: %d/%d", this->souls, this->souls_cap);
    wattroff(player_window, Color::YellowBlack);
    wrefresh(player_window);
}

int Player::computeAttackPower() {
    int power = this->primary_weapon->damage + this->base_damage;
    int power_range = this->primary_weapon->damage_range;
    double crit_chance = this->primary_weapon->crit_chance;

    double chance_to_crit = ((double) rand() / RAND_MAX);
    if (power_range != 0) {
        power += (rand() % power_range);
    }
    if (crit_chance >= chance_to_crit) {
        power *= 2;
    }
    if (this->souls == this->souls_cap) {
        power *= this->souls_multiplier;
    }
    return (int)power;
}

/*
*Attacks enemy. Returns true if enemy is killed, false otherwise
*
*/
bool Player::attack(Enemy *enemy, WINDOW *alert_window)
{
    bool attack_hit = checkIfAttackHit((this->primary_weapon->accuracy + this->accuracy), enemy->current_evasion);
    
    wattron(alert_window, Color::MagentaBlack);
    if (attack_hit) {
        int dmg = this->computeAttackPower() * (1 - enemy->current_protection);
        enemy->current_health -= dmg;
        wprintw(alert_window, "You hit %s for %d damage.\n", (enemy->name).c_str(), dmg);
        wrefresh(alert_window);
        wattroff(alert_window, Color::MagentaBlack);
    }
    else {
        wprintw(alert_window, "You miss %s.\n", (enemy->name).c_str());
        wrefresh(alert_window);
        wattroff(alert_window, Color::MagentaBlack);
    }
    return enemy->current_health <= 0;
}

//Only passively regenerate on active moves (no skips)
//This promotes active gameplay
//Also only do if health is not capped
void Player::passiveHealthRegeneration(WINDOW *player_window)
{
    if (this->current_health < this->current_total_health) {
        this->passive_health_regen_counter++;
        if (this->passive_health_regen_counter == this->passive_health_regen_trigger) {
            this->passive_health_regen_counter = 0;
            this->current_health += this->passive_health_regen_amount;
        }
        this->printStatus(player_window);
    }
}

//Returns true if the movement was valid, false otherwise
bool Player::moveSpace(int direction, int map_size, WINDOW *player_window, std::vector<Enemy> enemies)
{
    switch (direction) {
        case KEY_UP:
            if (this->row == 0) return false;
            this->row--;
            break;
        case KEY_DOWN:
            if (this->row == map_size-1) return false;
            this->row++;
            break;
        case KEY_LEFT:
            if (this->col == 0) return false;
            this->col--;
            break;
        case KEY_RIGHT:
            if (this->col == map_size-1) return false;
            this->col++;
            break;
        default:
            break;
    }
    
    passiveHealthRegeneration(player_window);
    return true;
}

