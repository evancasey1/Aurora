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

/* Begin globals */
const int MAP_SIZES[3] = {45, 85, 125};
const int MAP_VERTICAL_PADDING = 3;
const int COM_VERTICAL_PADDING = 4;
const int MAP_HORIZONTAL_PADDING = 5;
const int SPAWN_TOTAL_DENOM = 100;
const int NUM_ENEMY_TYPES = 6;
static int enemy_spawn_rate = 100; // (enemy_spawn_rate/SPAWN_TOTAL_DENOM) chance to spawn per each 'tick'
//static int chosen_map_size;
int max_enemies = 50;
const std::string ENEMY_NAMES[] = {"Wolf", "Undead", "Goblin", "Troll", "Orc", "Bear"};
const char ENEMY_SYMBOLS[] = {'W', 'U', 'G', 'T', 'O', 'B'};
WINDOW *map_window;
WINDOW *alert_window;
WINDOW *player_status_window;
/* End Globals */ 

int computeAttackPower(int base_power, int power_range, double accuracy, double crit_chance) {
	int power = base_power;
	double chance_to_hit  = ((double) rand() / RAND_MAX);
	double chance_to_crit = ((double) rand() / RAND_MAX);
	if (power_range != 0) {
		power += (rand() % power_range);
	}
	if (crit_chance >= chance_to_crit) {
		power *= 2;
	}
	if(accuracy >= chance_to_hit) {
		return power;
	}
	return 0;
}

void initiate_combat(Player *player, std::vector<Enemy> *enemies, int enemy_index)
{
	//TODO:
	//	actual turn-based combat
	int player_attack_power;
	int enemy_attack_power;
	Enemy *enemy = &(enemies->at(enemy_index));
	player_attack_power = computeAttackPower(player->attack_power, player->attack_power_range, player->accuracy, player->crit_chance);
	enemy_attack_power  = computeAttackPower(enemy->attack_power, enemy->attack_power_range, enemy->accuracy, enemy->crit_chance);
	if (player->speed > enemy->speed) {
		//player attacks first
		enemy->current_health -= player_attack_power;
		wprintw(alert_window, "You hit %s for %d damage.\n", (enemy->name).c_str(), player_attack_power);
		if (enemy->current_health > 0) {
			player->current_health -= enemy_attack_power;
			wprintw(alert_window, "%s hits you for %d damage.\n", (enemy->name).c_str(), enemy_attack_power);
		}
		else {
			wprintw(alert_window, "You killed %s.\n", (enemy->name).c_str());
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
		usleep(3500000);
		endwin();
		std::cout << "Game over.\n" << std::endl;
		exit(0);
	}
	player->printStatus(player_status_window);
	wrefresh(player_status_window);
	wrefresh(alert_window);

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
	int rng;
	int index = 0;
	double distance;
	wattron(alert_window, COLOR_PAIR(5)); //turn on enemy color scheme while enemy events are active
	for (auto &e : *enemies) {
		distance = getDistance(player->getCol(), player->getRow(), e.col, e.row);
		if (distance <= (double) e.vision) {
			e.seek(player->getRow(), player->getCol());
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
		if (e.row == player->getRow() && e.col == player->getCol()) {
			initiate_combat(player, enemies, index);
		}
		index++;
	}
	//loop through enemy vector and delete all dead enemies
	std::vector<Enemy>::iterator iter;
	for (iter = enemies->begin(); iter != enemies->end();) {
		if (iter->current_health <= 0) {
			iter = enemies->erase(iter);
		}
		else {
			++iter;
		}
	}

	//Chance to spawn an enemy
	rng = (rand() % SPAWN_TOTAL_DENOM);
	if (rng <= enemy_spawn_rate && enemies->size() < max_enemies) {
		int enemy_index = (rand() % NUM_ENEMY_TYPES);
		std::string e_name = ENEMY_NAMES[enemy_index];
		char e_symbol = ENEMY_SYMBOLS[enemy_index];
		enemies->push_back(*(new Enemy(e_name, e_symbol, player->getRow(), player->getCol(), player->vision, map->size)));
	}

	wattroff(alert_window, COLOR_PAIR(1));
}

void mainGameLoop(Player *player, Map *map)
{
	int ch;
	std::vector<Enemy> enemies;	
	map->printPlayerInfo(*player, map_window);
	map->printMap(player, player->vision, enemies, map_window);
	player->printStatus(player_status_window);

	while (true) {
		ch = getch();
		switch(ch) {
			//player movement
			case KEY_UP: case KEY_DOWN: case KEY_LEFT: case KEY_RIGHT:
				if(player->moveSpace(ch, map->size)) {
					player->used_moves++;
					map->printPlayerInfo(*player, map_window);
					//only prints if enemies won't also update and force a map print
					if (player->used_moves != player->allowed_moves) {
						map->printMap(player, player->vision, enemies, map_window);
					}
				}
				break;
			//player skips a single move
			case ' ':
				player->used_moves++;
				map->printPlayerInfo(*player, map_window);
				break;
			//player skips all remaining moves for the turn
			case '\n': case KEY_ENTER:
				player->used_moves = player->allowed_moves;
				break;
			default:
				break;
		}
		if (player->used_moves == player->allowed_moves) {
			enemyEvents(player, map, &enemies);
			player->used_moves = 0;
			map->printPlayerInfo(*player, map_window);
			map->printMap(player, player->vision, enemies, map_window);
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
	mvprintw(MAP_VERTICAL_PADDING-1, horiz_pad, "Select your map size:\n");
	attroff(A_BOLD);
	while(ch != KEY_ENTER && ch != '\n') {
		//prints contents of options[]
		//highlights currently selected option
		for (int i = 0; i < 3; i++) {
			move(MAP_VERTICAL_PADDING + i, horiz_pad);
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
	//	Add weapon class to be implemented by Player and Enemy both
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
	init_pair(5, COLOR_RED, COLOR_BLACK); //enemy alert: MAIN
	init_pair(4, COLOR_BLUE, COLOR_WHITE); //player: MAP
	init_pair(3, COLOR_RED, COLOR_WHITE); //enemy: MAP
	init_pair(2, COLOR_WHITE, COLOR_BLACK); //default night: MAP
	init_pair(1, COLOR_BLACK, COLOR_WHITE); //default day: MAP
	// END COLORS //

	Map *map = userCreateMap();

	player.userCreatePlayer();
	player.setPosition((int)map->size/2, (int)map->size/2);

	int map_height = 30, map_width = 50;
	int com_height = 14, com_width = 50;
	int ps_height  = 1 , ps_width = map_width + com_width;

	map_window = newwin(map_height, map_width, MAP_VERTICAL_PADDING, MAP_HORIZONTAL_PADDING);
	alert_window = newwin(com_height, com_width, COM_VERTICAL_PADDING, map_width + (MAP_HORIZONTAL_PADDING * 2));
	player_status_window = newwin(ps_height, ps_width, 1, MAP_HORIZONTAL_PADDING);
	scrollok(alert_window, true);

	mainGameLoop(&player, map);

	return 0;
}