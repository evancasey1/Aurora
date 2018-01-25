/*
* AURORA
* Author: Evan Casey
* Date: Oct 17, 2017
*/
#include <iostream>
#include <string>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <math.h>
#include <typeinfo>
#include <boost/variant.hpp>
#include "enemy.h"
#include "map.h"
#include "equipment.h"
#include "equipmenttype.h"
#include "color.h"
#include "visitors.h"

/* Begin globals */
const int MAP_SIZES[3] = {50, 100, 150};
const int MAP_VERTICAL_PADDING = 3;
const int COM_VERTICAL_PADDING = 4;
const int MAP_HORIZONTAL_PADDING = 5;
const int SPAWN_TOTAL_DENOM = 100;
const int NUM_ENEMY_TYPES = 6;
const int SPEED_ROLL = 5;
bool is_daytime = true;
int day_turns = 80;
int night_turns = 30;
int turn_counter = 0;
int enemy_spawn_rate = 25;
unsigned int max_enemies = 50;
int loot_items_per_screen = 5;

/*
const std::string ENEMY_NAMES[] = {"Goblin", "Undead", "Troll", "Orc", "Bear", "Wolf"};
const char ENEMY_SYMBOLS[] = {'G', 'U', 'T', 'O', 'B', 'W'};
*/

const std::string ENEMY_NAMES[] = {"Kobold", "Bear", "Wolf", "Specter", "Undead", "Graverobber"};

std::vector<Enemy::Loot> loot;

WINDOW *map_window;
WINDOW *alert_window;
WINDOW *player_status_window;
WINDOW *inventory_window;
WINDOW *loot_window;
WINDOW *power_status_window;
WINDOW *item_description_window;

const std::string TITLE_TEXT[7] =  {"          :::     :::    ::: :::::::::   ::::::::  :::::::::      :::  ",
									 "       :+: :+:   :+:    :+: :+:    :+: :+:    :+: :+:    :+:   :+: :+: ",
									 "     +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+ +:+    +:+  +:+   +:+ ",
									 "   +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+ +#++:++#:  +#++:++#++: ",
									 "  +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+ +#+    +#+ +#+     +#+  ",
									 " #+#     #+# #+#    #+# #+#    #+# #+#    #+# #+#    #+# #+#     #+#   ",
									 "###     ###  ########  ###    ###  ########  ###    ### ###     ###    "};
/* End Globals */ 

void printSouls(Player *player) 
{
	wclear(power_status_window);
	if (player->souls == player->souls_cap) {
		wattron(power_status_window, Color::YellowBlack);
	}
	wprintw(power_status_window, "SOULS: %d/%d", player->souls, player->souls_cap);
	wrefresh(power_status_window);
	wattroff(power_status_window, Color::YellowBlack);
}

bool checkIfAttackHit(double accuracy, double evasion) 
{
	double chance_to_hit  = ((double) rand() / RAND_MAX);

	if(accuracy >= chance_to_hit + evasion) {
		return true;
	}
	return false;
}

int rollSpeedMod(int n)
{
	return (rand() % n);
}

void fastCombat(Player *player, std::vector<Enemy> *enemies, int enemy_index)
{
	Enemy *enemy = &(enemies->at(enemy_index));

	int player_attack_power = player->computeAttackPower() * (1 - enemy->current_protection);
	int enemy_attack_power =  enemy->computeAttackPower() * (1 - player->current_protection);
	bool player_hit = checkIfAttackHit((player->primary_weapon->accuracy + player->accuracy), enemy->current_evasion);
	bool enemy_hit  = checkIfAttackHit(enemy->accuracy, player->current_evasion);
	int p_speed = player->speed + rollSpeedMod(SPEED_ROLL);
	int e_speed = enemy->speed + rollSpeedMod(SPEED_ROLL);

	if (p_speed > e_speed) {
		if (!player_hit) {
			wprintw(alert_window, "You miss %s.\n", (enemy->name).c_str());
		}
		else {
			enemy->current_health -= player_attack_power;
			wprintw(alert_window, "You hit %s for %d damage.\n", (enemy->name).c_str(), player_attack_power);
		}
		//Enemy turn
		if (enemy->current_health > 0) {
			if (enemy_hit) {
				player->current_health -= enemy_attack_power;
				wprintw(alert_window, "%s hits you for %d damage.\n", (enemy->name).c_str(), enemy_attack_power);
			}
			else {
				wprintw(alert_window, "%s missed.\n", (enemy->name).c_str());
			}
		}
	}
	else {
		if (!enemy_hit) {
			wprintw(alert_window, "%s missed.\n", (enemy->name).c_str());
		}
		else {
			player->current_health -= enemy_attack_power;
			wprintw(alert_window, "%s hits you for %d damage.\n", (enemy->name).c_str(), enemy_attack_power);
		}
		//Player turn
		if (player->current_health > 0) {
			if (!player_hit) {
				wprintw(alert_window, "You miss %s.\n", (enemy->name).c_str());
			}
			else {
				enemy->current_health -= player_attack_power;
				wprintw(alert_window, "You hit %s for %d damage.\n", (enemy->name).c_str(), player_attack_power);
			}	
		}
	}
	if (player->current_health <= 0) {
		wprintw(alert_window, "%s killed you. Game over.\n", (enemy->name).c_str());
		wrefresh(alert_window);
		wrefresh(player_status_window);
		endwin();
		std::cout << "Game over.\n" << std::endl;
		exit(0);
	}
	if (enemy->current_health <= 0) {
		wprintw(alert_window, "You killed %s.\n", (enemy->name).c_str());
		wattroff(alert_window, Color::RedBlack);
		wattron(alert_window, Color::GreenBlack);
		wprintw(alert_window, "You gained %d XP.\n", enemy->XP);
		wattroff(alert_window, Color::GreenBlack);
		wattron(alert_window, Color::RedBlack);
		printSouls(player);
		player->gainExp(enemy->XP, alert_window);
		player->gainSouls(enemy->souls, alert_window);
	}

	player->printStatus(player_status_window);
	wrefresh(alert_window);
}

void printLoot(std::vector<boost::variant<Weapon, Food, Armor>> allLoot, int cursor_index) 
{
	wclear(item_description_window);
	wclear(inventory_window);
	
	for (int i = 0; i < allLoot.size(); i++) {
		if (i == cursor_index) {
			wattron(inventory_window, A_STANDOUT);
		}
		boost::apply_visitor(Visitors::output_name(i, inventory_window), allLoot.at(i));
		if (i == cursor_index) {
			boost::apply_visitor(Visitors::output_desc(item_description_window), allLoot.at(i));
			wattroff(inventory_window, A_STANDOUT);
		}
	}
	wrefresh(inventory_window);
	wrefresh(item_description_window);
}
void manageLoot(Player *player, int loot_row, int loot_col)
{
	//boost::variant<Weapon, Food, Armor> allLootType;
	std::vector<boost::variant<Weapon, Food, Armor>> allLoot;
	int cursor_index = 0;
	int ch;
	bool isPickedup = false;

	for (int i = 0; i < loot.size(); i++) {
		if (loot.at(i).row == loot_row && loot.at(i).col == loot_col) {
			allLoot.insert(allLoot.end(), loot.at(i).weapons.begin(), loot.at(i).weapons.end());
			allLoot.insert(allLoot.end(), loot.at(i).food.begin(), loot.at(i).food.end());
			allLoot.insert(allLoot.end(), loot.at(i).armor.begin(), loot.at(i).armor.end());
		}
	}

	while(true) {
		if (allLoot.size() == 0) {
			wclear(inventory_window);
			wrefresh(inventory_window);
			return;
		}
		printLoot(allLoot, cursor_index);
		ch = getch();
		
		switch(ch) {
			case KEY_UP:
				if (cursor_index > 0) {
					cursor_index--;
				}
				break;
			case KEY_DOWN:
				if (cursor_index < (allLoot.size() - 1)) {
					cursor_index++;
				}
				break;

			case KEY_ENTER: case '\n':
				isPickedup = false;
				
				switch(allLoot.at(cursor_index).which()) {
					case 0:
						isPickedup = player->pickupLoot(boost::get<Weapon>(allLoot.at(cursor_index)));
						break;
					case 1:
						isPickedup = player->pickupLoot(boost::get<Food>(allLoot.at(cursor_index)));
						break;
					default:
						isPickedup = player->pickupLoot(boost::get<Armor>(allLoot.at(cursor_index)));
						break;
				}

				if (isPickedup) {
					allLoot.erase(allLoot.begin() + cursor_index);
					if (cursor_index == allLoot.size()) {
						cursor_index--;
					}
					//TODO: Delete from real loot table too
				}
				else {
					wprintw(alert_window, "Insufficient Capacity for %s\n", boost::apply_visitor(Visitors::get_name(), allLoot.at(cursor_index)));
					wrefresh(alert_window);
				}
				
				wclear(item_description_window);
				wrefresh(item_description_window);
				break;
			case 'l': case 'e':
				wclear(inventory_window);
				wrefresh(inventory_window);
				return;
			default:
				break;
		}
	}
}

/*
manageLoot:
	This function will handle the picking up and dropping of loot from enemies. Picked-up loot should be deleted 
	from the map and added to the player's inventory.
*/
/*
void manageLoot(Player *player, int loot_row, int loot_col) 
{
	int ch;
	std::vector<int> loot_indices; //Valid indices for loot, vect_index tracks this.
	std::vector<Enemy::Loot> loot_at_loc; //All loot objects at the specified row and column
	std::vector<Equipment> equipment_at_loc; //Loot broken down into weapon and food components and stored as Equipment
	std::string obj_name;
	int current_item_index = 0;
	int current_vect_index = 0;
	int current_total_index = 0;
	int combined_size = 0;
	int screen_index = 0;

	//The following loops populate the vectors in order
	//for loot to be managed
	for (int i = 0; i < loot.size(); i++) {
		if (loot.at(i).row == loot_row && loot.at(i).col == loot_col) {
			combined_size = (loot.at(i).weapons.size() + loot.at(i).food.size() + loot.at(i).armor.size()) - 1;
			loot_indices.push_back(combined_size);
			loot_at_loc.push_back(loot.at(i));
		}
	}
	int max_screen_index = (std::ceil(((double)loot_at_loc.size() / (double)loot_items_per_screen)) - 1);

	if (loot_at_loc.size() == 0) {
		return;
	}

	for (int j = 0; j < loot_at_loc.size(); j++) {
		equipment_at_loc.insert(equipment_at_loc.end(), loot_at_loc.at(j).weapons.begin(), loot_at_loc.at(j).weapons.end());
		equipment_at_loc.insert(equipment_at_loc.end(), loot_at_loc.at(j).food.begin(), loot_at_loc.at(j).food.end());
		equipment_at_loc.insert(equipment_at_loc.end(), loot_at_loc.at(j).armor.begin(), loot_at_loc.at(j).armor.end());
	}

	while(true) {
		if (loot_at_loc.size() == 0) {
			return;
		}
		printLoot(current_total_index, screen_index, max_screen_index, &loot_at_loc, item_description_window);
		ch = getch();
		
		switch(ch) {
			case KEY_UP:
				if (current_item_index != 0) {
					current_item_index--;
				}
				else if (current_item_index == 0 && current_vect_index > 0) {
					current_vect_index--;
					current_item_index = loot_indices.at(current_vect_index);
				}

				if (current_total_index != 0) {
					current_total_index--;
				}
				break;
			case KEY_DOWN:
				if (current_item_index == loot_indices.at(current_vect_index) && current_vect_index < (loot_indices.size() - 1)) {
					current_vect_index++;
					current_item_index = 0;
				}
				else if (current_item_index < loot_indices.at(current_vect_index)) {
					current_item_index++;
				}

				if (current_total_index < (equipment_at_loc.size() - 1)) {
					current_total_index++;
				}
				break;
			case KEY_RIGHT:
				if (screen_index < max_screen_index) {
					screen_index++;
				}
				break;
			case KEY_LEFT:
				if (screen_index > 0) {
					screen_index--;
				}
				break;
			
			case KEY_ENTER: case '\n':
				player->pickUpLootAtIndex(&loot, current_total_index, current_vect_index, current_item_index, &equipment_at_loc, &loot_at_loc, &loot_indices, inventory_window, alert_window);
				current_item_index = 0;
				current_vect_index = 0;
				current_total_index = 0;
				screen_index = 0;
				max_screen_index = (std::ceil(((double)loot_at_loc.size() / (double)loot_items_per_screen)) - 1);
				wclear(item_description_window);
				wrefresh(item_description_window);
				break;
			case 'l': case 'e':
				wclear(inventory_window);
				wrefresh(inventory_window);
				return;
			default:
				break;
		}
		
	}
}
*/
void deleteDefeatedEnemies(std::vector<Enemy> *enemies) {
	//loop through enemy vector and delete all dead enemies
	std::vector<Enemy>::iterator iter;
	for (iter = enemies->begin(); iter != enemies->end();) {
		if (iter->current_health <= 0) {
			iter->deathEvents(&loot, alert_window);
			iter = enemies->erase(iter);
		}
		else {
			++iter;
		}
	}
}

void spawnEnemy(std::vector<Enemy> *enemies, Player *player, Map *map) {
	int rng = (rand() % SPAWN_TOTAL_DENOM);
	if (rng <= enemy_spawn_rate && (enemies->size() < max_enemies)) {
		int enemy_index = (rand() % NUM_ENEMY_TYPES);
		std::string e_name = ENEMY_NAMES[enemy_index];
		Enemy tempE(e_name, player->row, player->col, player->vision, map->size, player->level);
		enemies->push_back(tempE);
	}
}

//get the distance between 2 points
double getDistance(int a_x, int a_y, int b_x, int b_y)
{
	int dx = abs(b_x - a_x);
	int dy = abs(b_y - a_y);
	return sqrt(pow(dx, 2) + pow(dy, 2));
}

void enemyEvents(Player *player, Map *map, std::vector<Enemy> *enemies)
{
	int index = 0;
	double distance;
	wattron(alert_window, Color::RedBlack); //turn on enemy color scheme while enemy events are active
	for (auto &e : *enemies) {
		distance = getDistance(player->col, player->row, e.col, e.row);
		if (distance <= (double) e.vision) {
			e.seek(player->row, player->col);
			if (e.alert_player) {
				e.alert_player = false;
				wprintw(alert_window, "%s has spotted you!\n", (e.name).c_str());
				wrefresh(alert_window);
			}
		}
		else {
			if (!e.alert_player) {
				//if player gets out of range of enemy sight
				//set up to alert them when they get back into range
				e.alert_player = true;
			}
			e.idle(map->size);
		}
		if (e.row == player->row && e.col == player->col) {
			fastCombat(player, enemies, index);
		}
		index++;
	}

	deleteDefeatedEnemies(enemies);
	spawnEnemy(enemies, player, map);

	wattroff(alert_window, Color::BlackWhite);
}

void printTitle()
{
	int titleLines = 7;
	int horizontal_pad = 12;
	for (int i = 0; i < titleLines; i++) {
		mvprintw((i + 3), horizontal_pad, TITLE_TEXT[i].c_str());
	}
	refresh();
}

void enemyNightBuff(std::vector<Enemy> *enemies)
{
	for (int i = 0; i < enemies->size(); i++) {
		enemies->at(i).globalBuff(enemies->at(i).nightbuff_multiplier);
	}
}

void enemyDayDebuff(std::vector<Enemy> *enemies)
{
	for (int i = 0; i < enemies->size(); i++) {
		enemies->at(i).globalDebuff(enemies->at(i).nightbuff_multiplier);
	}
}

void mainGameLoop(Player *player, Map *map)
{
	int ch;
	bool passive_turn = false;
	std::vector<Enemy> enemies;	
	map->printPlayerInfo(*player, map_window);
	map->printMap(player, player->vision, enemies, loot, map_window);
	player->printStatus(player_status_window);
	printSouls(player);

	while (true) {
		passive_turn = false;
		ch = getch();
		switch(ch) {
			//player movement
			case KEY_UP: case KEY_DOWN: case KEY_LEFT: case KEY_RIGHT:
				if(player->moveSpace(ch, map->size, player_status_window, enemies)) {
					player->used_moves++;
					map->printPlayerInfo(*player, map_window);
					//only prints if enemies won't also update and force a map print
					if (player->used_moves != player->allowed_moves) {
						map->printMap(player, player->vision, enemies, loot, map_window);
					}
				}
				break;
			case 'e':
				player->manageInventory(inventory_window, player_status_window, alert_window, &loot);
				player->printStatus(player_status_window);
				break;
			case 'l':
				//Temporary code for now. Just to test
				manageLoot(player, player->row, player->col);
				break;
			//player skips a single move
			case ',':
				player->used_moves++;
				map->printPlayerInfo(*player, map_window);
				passive_turn = true;
				break;
			//player skips all remaining moves for the turn
			case '.':
				player->used_moves = player->allowed_moves;
				passive_turn = true;
				break;
			default:
				break;
		}
		if (player->used_moves == player->allowed_moves) {
			turn_counter++;
			if (is_daytime && turn_counter > day_turns) {
				enemyNightBuff(&enemies);
				is_daytime = false;
				turn_counter = 0;
				wprintw(alert_window, "Night is falling.\n");
				wrefresh(alert_window);
			}
			else if (!is_daytime && turn_counter > night_turns) {
				enemyDayDebuff(&enemies);
				is_daytime = true;
				turn_counter = 0;
				wprintw(alert_window, "The sun rises.\n");
				wrefresh(alert_window);
			}
			if (!passive_turn) {
				player->foodEvents();
			}
			enemyEvents(player, map, &enemies);
			player->used_moves = 0;
			map->printPlayerInfo(*player, map_window);
			map->printMap(player, player->vision, enemies, loot, map_window);
		}
	}
	endwin();
}

Player userCreatePlayer()
{
	//TODO:
	//	Attribute selection
	int chosen_index = 0;
	std::string options[3] = {"Rogue", "Swordsman", "Warrior"};
	std::string descriptions[3] = {"The Rogue strikes swiftly and relies on shadows for concealment to compensate   for weak constitution.", 
								   "The Swordsman is a balanced fighter,    well suited for bladed weapons and      medium armor.", 
								   "The Warrior is a tanky brute that       specializes in blunt force weapons and  heavy armor."};
	int horiz_pad = (int) ((COLS/2)-10);
	int vert_pad = 3;
	WINDOW *player_description_window = newwin(30, 40, vert_pad + 25, horiz_pad - 10);
	int ch = 0;
	
	attron(A_BOLD);
	mvprintw(vert_pad + 10, horiz_pad, "Select your adventurer:\n");
	attroff(A_BOLD);
	while(ch != KEY_ENTER && ch != '\n') {
		//prints contents of options[]
		//highlights currently selected option
		wclear(player_description_window);
		wprintw(player_description_window, "%s", descriptions[chosen_index].c_str());
		wrefresh(player_description_window);
		for (int i = 0; i < 3; i++) {
			move(vert_pad + i + 11, horiz_pad);
			clrtoeol();
			if (i != chosen_index) {
				addstr(options[i].c_str());
			}
			else {
				attron(A_STANDOUT);
				addstr(options[i].c_str());
				attroff(A_STANDOUT);	
			}
		}
		
		ch = getch();
		switch(ch) {
			case KEY_UP:
				chosen_index <= 0 ? chosen_index = 2 : chosen_index--;
				refresh();
				break;
			case KEY_DOWN:
				chosen_index >= 2 ? chosen_index = 0 : chosen_index++;
				refresh();
				break;
			default:
				break;
		}
	}
	clear();
	refresh();
	free(player_description_window);
	Player player(options[chosen_index]);
	return player;
}

Map userCreateMap()
{
	//TODO:
	//	Support for custom map size
	int chosen_index = 0;
	const char *options[] = {"Tiny", "Normal", "Huge"};
	int horiz_pad = (int) ((COLS/2)-10);
	int ch = 0;
	attron(A_BOLD);
	printTitle();
	mvprintw(MAP_VERTICAL_PADDING + 10, horiz_pad, "Select your map size:\n");
	attroff(A_BOLD);
	while(ch != KEY_ENTER && ch != '\n') {
		//prints contents of options[]
		//highlights currently selected option
		for (int i = 0; i < 3; i++) {
			move(MAP_VERTICAL_PADDING + i + 11, horiz_pad);
			clrtoeol();
			if (i != chosen_index) {
				addstr(options[i]);
			}
			else {
				attron(A_STANDOUT);
				addstr(options[i]);
				attroff(A_STANDOUT);			
			}
		}
		
		ch = getch();
		switch(ch) {
			case KEY_UP:
				chosen_index <= 0 ? chosen_index = 2 : chosen_index--;
				refresh();
				break;
			case KEY_DOWN:
				chosen_index >= 2 ? chosen_index = 0 : chosen_index++;
				refresh();
				break;
			default:
				break;
		}
	}
	clear();
	refresh();
	Map map(MAP_SIZES[chosen_index]);
	return map;
}

void initiateWindows()
{
	int map_height = 30, map_width = 50;
	int com_height = 14, com_width = 50;
	int ps_height  = 2 , ps_width = map_width + com_width;
	int inv_height = 20, inv_width = 30;
	int inv_row = 22, inv_col = MAP_HORIZONTAL_PADDING;

	//TODO:
	//	have window generation depend on size of terminal, or resize it as necessary
	map_window = newwin(map_height, map_width, MAP_VERTICAL_PADDING, MAP_HORIZONTAL_PADDING);
	alert_window = newwin(com_height, com_width, COM_VERTICAL_PADDING + 1, map_width + (MAP_HORIZONTAL_PADDING * 2));
	player_status_window = newwin(ps_height, ps_width, 1, MAP_HORIZONTAL_PADDING);
	inventory_window = newwin(inv_height, inv_width, inv_row, inv_col);
	loot_window = newwin(inv_height, inv_width, inv_row, inv_col);
	power_status_window = newwin(1, 30, 50, MAP_HORIZONTAL_PADDING);
	item_description_window = newwin(30, 30, 22, 60);
	scrollok(alert_window, true);
	scrollok(inventory_window, true);
	scrollok(loot_window, true);
}

void initiateColorPairs()
{
	// START COLORS //
	init_pair(8, COLOR_YELLOW, COLOR_BLACK);//full souls
	init_pair(7, COLOR_CYAN, COLOR_BLACK);  //Level up
	init_pair(6, COLOR_GREEN, COLOR_BLACK); //XP gain
	init_pair(5, COLOR_RED, COLOR_BLACK);   //enemy alert: MAIN
	init_pair(4, COLOR_BLUE, COLOR_WHITE);  //player: MAP
	init_pair(3, COLOR_RED, COLOR_WHITE);   //enemy: MAP
	init_pair(2, COLOR_WHITE, COLOR_BLACK); //default night: MAP
	init_pair(1, COLOR_BLACK, COLOR_WHITE); //default day: MAP
	// END COLORS //
}

int main(int argc, char *argv[]) 
{
	//Map map;
	srand((int)time(0));

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
	curs_set(0);
	start_color();
	initiateColorPairs();
	initiateWindows();

	Map map = userCreateMap();
	printTitle();

	Player player = userCreatePlayer();
	player.setPosition((int)map.size/2, (int)map.size/2);

	mainGameLoop(&player, &map);

	return 0;
}
