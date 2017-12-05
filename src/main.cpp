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
#include "enemy.h"
#include "map.h"
//#include "wolf.h"

/* Begin globals */
const int MAP_SIZES[3] = {50, 100, 150};
const int MAP_VERTICAL_PADDING = 3;
const int COM_VERTICAL_PADDING = 4;
const int MAP_HORIZONTAL_PADDING = 5;
const int SPAWN_TOTAL_DENOM = 100;
const int NUM_ENEMY_TYPES = 2;
int enemy_spawn_rate = 25;
unsigned int max_enemies = 50;
/*
const std::string ENEMY_NAMES[] = {"Wolf", "Undead", "Goblin", "Troll", "Orc", "Bear"};
const char ENEMY_SYMBOLS[] = {'W', 'U', 'G', 'T', 'O', 'B'};
*/
//For testing:
const std::string ENEMY_NAMES[] = {"Wolf", "Goblin"};
const char ENEMY_SYMBOLS[] = {'W', 'G'};

std::vector<Enemy::Loot> loot;

WINDOW *map_window;
WINDOW *alert_window;
WINDOW *player_status_window;
WINDOW *inventory_window;


const std::string TITLE_TEXT[7] =  {"          :::     :::    ::: :::::::::   ::::::::  :::::::::      :::  ",
									 "       :+: :+:   :+:    :+: :+:    :+: :+:    :+: :+:    :+:   :+: :+: ",
									 "     +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+ +:+    +:+  +:+   +:+ ",
									 "   +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+ +#++:++#:  +#++:++#++: ",
									 "  +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+ +#+    +#+ +#+     +#+  ",
									 " #+#     #+# #+#    #+# #+#    #+# #+#    #+# #+#    #+# #+#     #+#   ",
									 "###     ###  ########  ###    ###  ########  ###    ### ###     ###    "};
/* End Globals */ 

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
			player->gainExp(enemy->XP, alert_window);
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
	int loot_size;
	std::vector<Enemy> enemies;	
	map->printPlayerInfo(*player, map_window);
	map->printMap(player, player->vision, enemies, loot, map_window);
	player->printStatus(player_status_window);

	while (true) {
		ch = getch();
		switch(ch) {
			//player movement
			case KEY_UP: case KEY_DOWN: case KEY_LEFT: case KEY_RIGHT:
				wclear(inventory_window);
				wrefresh(inventory_window);
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
				player->printInventory(inventory_window);
				break;
			case 'l':
				//Temporary code for now. Just to test
				loot_size = loot.size();
				for (int k = 0; k < loot_size; k++) {
					if (loot.at(k).row == player->row && loot.at(k).col == player->col && loot.at(k).dropped_by != "Wolf") {
						player->inventory.weapons.push_back(loot.at(k).weapons.at(0));
						loot.erase(loot.begin() + k);
						k--;
						loot_size--;
					}
				}	
				break;
			//player skips a single move
			case ',':
				player->used_moves++;
				map->printPlayerInfo(*player, map_window);
				break;
			//player skips all remaining moves for the turn
			case '.':
				player->used_moves = player->allowed_moves;
				break;
			default:
				break;
		}
		if (player->used_moves == player->allowed_moves) {
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
	//ISSUE:
	//	Rare segementation fault on player movement. 
	//		Not able to reproduce
	//		Seems to happen more at the edges of the map. 
	//		Could be an issue with enemy spawns
	//TODO:
	//	At some point put map creation into the map class. Not vital, but would
	//  make more sense for the organization and flow of the program
	//	---
	//	Add equipment class (food, supplies)
	//	---
	//	Add new window for combat. Yet to be determined how this will be laid out.
	//  ---
	//	Figure out what to do if 2 different enemies reach the player at the same time.
	//	---
	//	Implement enemy party system (I.E. A troop of 3 goblins with varying statlines)
	//	---
	//	Player attributes for greater customization over game experience
	//	---
	
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
	int inv_height = 30, inv_width = 50;
	int inv_row = 22, inv_col = MAP_HORIZONTAL_PADDING;

	map_window = newwin(map_height, map_width, MAP_VERTICAL_PADDING, MAP_HORIZONTAL_PADDING);
	alert_window = newwin(com_height, com_width, COM_VERTICAL_PADDING + 1, map_width + (MAP_HORIZONTAL_PADDING * 2));
	player_status_window = newwin(ps_height, ps_width, 1, MAP_HORIZONTAL_PADDING);
	inventory_window = newwin(inv_height, inv_width, inv_row, inv_col);
	scrollok(alert_window, true);
	scrollok(inventory_window, true);

	mainGameLoop(&player, map);

	return 0;
}
