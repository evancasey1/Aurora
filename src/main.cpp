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
#include "enemy.h"
#include "map.h"
#include "equipment.h"

/* Begin globals */
const int MAP_SIZES[3] = {50, 100, 150};
const int MAP_VERTICAL_PADDING = 3;
const int COM_VERTICAL_PADDING = 4;
const int MAP_HORIZONTAL_PADDING = 5;
const int SPAWN_TOTAL_DENOM = 100;
const int NUM_ENEMY_TYPES = 6;
bool is_daytime = true;
int day_turns = 70;
int night_turns = 30;
int turn_counter = 0;
int enemy_spawn_rate = 25;
unsigned int max_enemies = 50;

const std::string ENEMY_NAMES[] = {"Goblin", "Undead", "Troll", "Orc", "Bear", "Wolf"};
const char ENEMY_SYMBOLS[] = {'G', 'U', 'T', 'O', 'B', 'W'};

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
		wattron(power_status_window, COLOR_PAIR(8));
	}
	wprintw(power_status_window, "SOULS: %d/%d", player->souls, player->souls_cap);
	wrefresh(power_status_window);
	wattroff(power_status_window, COLOR_PAIR(8));
}

void initiate_combat(Player *player, std::vector<Enemy> *enemies, int enemy_index)
{
	//TODO:
	//	actual turn-based combat
	int player_attack_power;
	int enemy_attack_power;
	Enemy *enemy = &(enemies->at(enemy_index));
	player_attack_power = player->computeAttackPower();
	enemy_attack_power  = enemy->computeAttackPower();
	if (player->speed > enemy->speed) {
		//player attacks first
		enemy->current_health -= player_attack_power;
		wprintw(alert_window, "You hit %s for %d damage.\n", (enemy->name).c_str(), player_attack_power);
		if (enemy->current_health > 0) {
			player->current_health -= enemy_attack_power;
			wprintw(alert_window, "%s hits you for %d damage.\n", (enemy->name).c_str(), enemy_attack_power);
		}
		else {
			//enemy died
			wprintw(alert_window, "You killed %s.\n", (enemy->name).c_str());
			wattroff(alert_window, COLOR_PAIR(5));
			wattron(alert_window, COLOR_PAIR(6));
			wprintw(alert_window, "You gained %d XP.\n", enemy->XP);
			wattroff(alert_window, COLOR_PAIR(6));
			wattron(alert_window, COLOR_PAIR(5));
			printSouls(player);
			player->gainExp(enemy->XP, alert_window);
			player->gainSouls(enemy->souls, alert_window);
		}
	}
	else {
		//enemy attacks first
		player->current_health -= enemy_attack_power;
		wprintw(alert_window, "%s hits you for %d damage.\n", (enemy->name).c_str(), enemy_attack_power);
		if (player->current_health > 0) {
			enemy->current_health -= player_attack_power;
			wprintw(alert_window, "You hit %s for %d damage.\n", (enemy->name).c_str(), player_attack_power);
		}
	}
	if (player->current_health <= 0) {
		//player died
		wprintw(alert_window, "%s killed you. Game over.\n", (enemy->name).c_str());
		wrefresh(alert_window);
		wrefresh(player_status_window);
		usleep(2500000);
		endwin();
		std::cout << "Game over.\n" << std::endl;
		exit(0);
	}
	player->printStatus(player_status_window);
	wrefresh(alert_window);
}

void printLoot(int item_index, std::vector<Enemy::Loot> *loot_at_loc, WINDOW *item_description_window) 
{
	wclear(item_description_window);
	wclear(inventory_window);
	int counter = 0;

	std::vector<Enemy::Loot>::iterator loot_iter;
	std::vector<Weapon>::iterator weapon_iter;
	std::vector<Food>::iterator food_iter;

	if (loot_at_loc->size() == 0) {
		wprintw(inventory_window, "<EMPTY>");
	}
	else {
		for (loot_iter = loot_at_loc->begin(); loot_iter != loot_at_loc->end(); loot_iter++) {
			wattron(inventory_window, A_BOLD);
			wprintw(inventory_window, "%s\n", (loot_iter->dropped_by).c_str());
			wattroff(inventory_window, A_BOLD);
			
			for (weapon_iter = loot_iter->weapons.begin(); weapon_iter != loot_iter->weapons.end(); weapon_iter++) {
				if (counter == item_index) {
					wattron(inventory_window, A_STANDOUT);
				}
				wprintw(inventory_window, "> [%d] %s\n", counter, (weapon_iter->name).c_str());
				if (counter == item_index) {
					wprintw(item_description_window, "%s\nAttack: %d - %d\nAccuracy: %.2f\nCrit: %.2f", (weapon_iter->name).c_str(), weapon_iter->attack_power, weapon_iter->attack_power + weapon_iter->attack_power_range, weapon_iter->accuracy, weapon_iter->crit_chance);
					wattroff(inventory_window, A_STANDOUT);
				}
				counter++;
			}
			for (food_iter = loot_iter->food.begin(); food_iter != loot_iter->food.end(); food_iter++) {
				if (counter == item_index) {
					wattron(inventory_window, A_STANDOUT);
				}
				wprintw(inventory_window, "> [%d] %s\n", counter, (food_iter->name).c_str());
				if (counter == item_index) {
					wprintw(item_description_window, "%s\nHealth Gain: %d\nHeal Over Time: %d:%d:%d", (food_iter->name).c_str(), food_iter->initial_health_gain, food_iter->health_gain_per_trigger, food_iter->total_triggers, food_iter->turns_until_trigger);
					wattroff(inventory_window, A_STANDOUT);
				}
				counter++;
			}
		}
	}
	wrefresh(item_description_window);
	wrefresh(inventory_window);
}

void pickUpLootAtIndex(Player *player, int current_total_index, int current_vect_index, int current_item_index, std::vector<Equipment> *equipment_at_loc, std::vector<Enemy::Loot> *loot_at_loc, std::vector<int> *loot_indices) 
{
	//	TODO: Delete the values in the true loot vector as well.
	std::vector<Enemy::Loot>::iterator loot_iter;
	std::vector<Equipment>::iterator equipment_iter;

	loot_iter = loot_at_loc->begin();
	equipment_iter = equipment_at_loc->begin();
	while (std::distance(loot_at_loc->begin(), loot_iter) < current_vect_index) {
		loot_iter++;
	}
	while (std::distance(equipment_at_loc->begin(), equipment_iter) < current_total_index) {
		equipment_iter++;
	}

	/*
	CASE IF WEAPON
	*/
	if (equipment_at_loc->at(current_total_index).equipment_id == 0) {
		if (player->inventory.weapon_count == player->inventory.weapon_capacity) {
			wprintw(alert_window, "Insufficient space in weapon pouch\n");
			wrefresh(alert_window);
		}
		else {
			player->inventory.weapons.push_back(loot_at_loc->at(current_vect_index).weapons.at(current_item_index));
			player->inventory.weapon_count++;
			loot_iter->weapons.erase(loot_iter->weapons.begin() + current_item_index);
			if (loot_iter->weapons.size() == 0) {
				loot_at_loc->erase(loot_iter);
			}
			loot_indices->at(current_vect_index)--;
			equipment_at_loc->erase(equipment_iter);
		}
	}
	/*
	CASE IF FOOD
	*/
	else if (equipment_at_loc->at(current_total_index).equipment_id == 1) {
		if (player->inventory.food_count == player->inventory.food_capacity) {
			wprintw(alert_window, "Insufficient space in food reserves\n");
			wrefresh(alert_window);
		}
		else {
			player->inventory.food.push_back(loot_at_loc->at(current_vect_index).food.at(current_item_index));
			player->inventory.food_count++;
			loot_iter->food.erase(loot_iter->food.begin() + current_item_index);
			if (loot_iter->food.size() == 0) {
				loot_at_loc->erase(loot_iter);
			}
			loot_indices->at(current_vect_index)--;
			equipment_at_loc->erase(equipment_iter);
		}
	}

	if (loot_indices->at(current_vect_index) == -1) {
		loot_indices->erase(loot_indices->begin() + current_vect_index);
	}
	current_item_index = 0;
	current_vect_index = 0;
	current_total_index = 0;

	if (loot_at_loc->size() == 0) {
		wclear(inventory_window);
		wrefresh(inventory_window);
		return;
	}
}

//There is probably a more efficient way to handle deletion. 
//Will worry about that later, after I nail down a way to do it at all
//Will be really important to clean up this function later for readability 
//	Have to break it down in a smaller, more modular fashion
/*
manageLoot:
	This function will handle the picking up and dropping of loot from enemies. Picked-up loot should be deleted 
	from the map and added to the player's inventory.
*/
void manageLoot(Player *player, int loot_row, int loot_col) 
{
	int ch;
	std::vector<int> loot_indices;
	std::vector<Enemy::Loot> loot_at_loc;
	std::vector<Equipment> equipment_at_loc;
	std::string obj_name;
	int current_item_index = 0;
	int current_vect_index = 0;
	int current_total_index = 0;
	int combined_size = 0;

	for (int i = 0; i < loot.size(); i++) {
		if (loot.at(i).row == loot_row && loot.at(i).col == loot_col) {
			combined_size = (loot.at(i).weapons.size() + loot.at(i).food.size()) - 1;
			loot_indices.push_back(combined_size);
			loot_at_loc.push_back(loot.at(i));
		}
	}
	for (int j = 0; j < loot_at_loc.size(); j++) {
		for (int w = 0; w < loot_at_loc.at(j).weapons.size(); w++) {
			equipment_at_loc.push_back(loot_at_loc.at(j).weapons.at(w));
		}
		for (int f = 0; f < loot_at_loc.at(j).food.size(); f++) {
			equipment_at_loc.push_back(loot_at_loc.at(j).food.at(f));
		}
	}

	if (loot_at_loc.size() == 0) {
		return;
	}

	while(true) {
		printLoot(current_total_index, &loot_at_loc, item_description_window);
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
			case KEY_ENTER: case '\n':
				pickUpLootAtIndex(player, current_total_index, current_vect_index, current_item_index, &equipment_at_loc, &loot_at_loc, &loot_indices);
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
		char e_symbol = ENEMY_SYMBOLS[enemy_index];
		enemies->push_back(*(new Enemy(e_name, e_symbol, player->row, player->col, player->vision, map->size)));
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
	wattron(alert_window, COLOR_PAIR(5)); //turn on enemy color scheme while enemy events are active
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
			initiate_combat(player, enemies, index);
		}
		index++;
	}

	deleteDefeatedEnemies(enemies);
	spawnEnemy(enemies, player, map);

	wattroff(alert_window, COLOR_PAIR(1));
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

void mainGameLoop(Player *player, Map *map)
{
	int ch;
	bool passive_turn = false;
	//int loot_size;
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
				if(player->moveSpace(ch, map->size, player_status_window)) {
					player->used_moves++;
					map->printPlayerInfo(*player, map_window);
					//only prints if enemies won't also update and force a map print
					if (player->used_moves != player->allowed_moves) {
						map->printMap(player, player->vision, enemies, loot, map_window);
					}
				}
				break;
			case 'e':
				player->manageInventory(inventory_window, player_status_window, alert_window);
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
				is_daytime = false;
				turn_counter = 0;
				wprintw(alert_window, "Night is falling.\n");
				wrefresh(alert_window);
			}
			else if (!is_daytime && turn_counter > night_turns) {
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

Map* userCreateMap()
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
	return new Map(MAP_SIZES[chosen_index]);
}

int main(int argc, char *argv[]) 
{
	Player player;
	//Map map;
	srand((int)time(0));

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
	curs_set(0);
	start_color();

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

	Map *map = userCreateMap();

	printTitle();
	player.userCreatePlayer();
	player.setPosition((int)map->size/2, (int)map->size/2);

	int map_height = 30, map_width = 50;
	int com_height = 14, com_width = 50;
	int ps_height  = 2 , ps_width = map_width + com_width;
	int inv_height = 20, inv_width = 30;
	int inv_row = 22, inv_col = MAP_HORIZONTAL_PADDING;

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

	mainGameLoop(&player, map);

	return 0;
}
