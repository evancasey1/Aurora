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

static const int MAP_SIZES[3] = {45, 85, 125};
static const int MAP_VERTICAL_PADDING = 2;
static const int COM_VERTICAL_PADDING = 3;
static const int MAP_HORIZONTAL_PADDING = 5;
static const int ENTER_KEY = 10;
static const int SPAWN_TOTAL_DENOM = 100;
static int enemy_spawn_rate = 50; // (enemy_spawn_rate/SPAWN_TOTAL_DENOM) chance to spawn per each 'tick'
static int chosen_map_size;
static int max_enemies = 2;
WINDOW *map_window;
WINDOW *alert_window;

void initiate_combat(Player *player, Enemy *enemy)
{
	//TODO:
	//	actual combat
	wprintw(alert_window, "Combat has started\n");
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
				wprintw(alert_window, "%s has spotted you!\n", e.name);
				wrefresh(alert_window);
			}
		}
		else {
			if (!e.alert_player) {
				//if player gets out of range of enemy sight
				//set up to alert them when they get back into range
				e.alert_player = true;
			}
			e.idle();
		}
		if (e.row == player->getRow() && e.col == player->getCol()) {
			initiate_combat(player, &e);
			enemies->erase(enemies->begin() + index);
		}
		index++;
	}
	rng = (rand() % SPAWN_TOTAL_DENOM);
	if (rng <= enemy_spawn_rate && enemies->size() < max_enemies) {
		enemies->push_back(*(new Enemy(player->getRow(), player->getCol(), player->vision, map->size)));
	}
	wattroff(alert_window, COLOR_PAIR(1));
}

void mainGameLoop(Player *player, Map *map)
{
	int ch;
	std::vector<Enemy> enemies;	
	map->printPlayerInfo(*player, map_window);
	map->printMap(player, player->vision, enemies, map_window);

	while (true) {
		ch = getch();
		switch(ch) {
			//player movement
			case KEY_UP: case KEY_DOWN: case KEY_LEFT: case KEY_RIGHT:
			case 'w': case 's': case 'a': case 'd':
				player->used_moves++;
				player->moveSpace(ch, map->size);
				map->printPlayerInfo(*player, map_window);
				map->printMap(player, player->vision, enemies, map_window);
				break;
			//player skips a single move
			case ' ':
				player->used_moves++;
				map->printPlayerInfo(*player, map_window);
				break;
			//player skips all remaining moves for the turn
			case ENTER_KEY:
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

int userSelectMapSize()
{
	//TODO:
	//	Support for custom map size
	int chosen_index = 0;
	const char *options_sel[] =	{">> Tiny <<", ">> Normal <<", ">> Huge <<"};
	const char *options_idle[] = {"Tiny", "Normal", "Huge"};
	int horiz_pad = (int) ((COLS/2)-10);
	int ch = 0;
	
	mvprintw(MAP_VERTICAL_PADDING-1, horiz_pad, "Select your map size\n");
	while(ch != ENTER_KEY) {
		//prints contents of options[]
		//highlights currently selected option
		for (int i = 0; i < 3; i++) {
			if (i != chosen_index) {
				move(MAP_VERTICAL_PADDING + i, horiz_pad);
				clrtoeol();
				addstr(options_idle[i]);
			}
			else {
				move(MAP_VERTICAL_PADDING + i, horiz_pad);
				clrtoeol();
				addstr(options_sel[i]);			
			}
		}
		
		ch = getch();
		switch(ch) {
			case KEY_UP: case 'w':
				chosen_index <= 0 ? chosen_index = 2 : chosen_index--;
				refresh();
				break;
			case KEY_DOWN: case 's':
				chosen_index >= 2 ? chosen_index = 0 : chosen_index++;
				refresh();
				break;
			default:
				break;
		}
	}
	clear();
	refresh();
	return chosen_index;
}

int main(int argc, char *argv[]) 
{
	//ISSUE:
	//	Rare segementation fault on player movement. 
	//		Not able to reproduce
	//		maybe when 2 enemies are on same tile at same time
	Player player;
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

	int index = userSelectMapSize();
	chosen_map_size = MAP_SIZES[index];
    Map map(chosen_map_size);

	player.userCreatePlayer();
	player.setPosition((int)chosen_map_size/2, (int)chosen_map_size/2);

	int map_height = 20, map_width = 35;
	int com_height = 14, com_width = 50;

	map_window = newwin(map_height, map_width, MAP_VERTICAL_PADDING, MAP_HORIZONTAL_PADDING);
	alert_window = newwin(com_height, com_width, COM_VERTICAL_PADDING, map_width + (MAP_HORIZONTAL_PADDING * 2));
	scrollok(alert_window, true);

	mainGameLoop(&player, &map);

	return 0;
}