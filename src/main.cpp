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
#include "enemy.h"
#include "player.h"
#include "map.h"

static const int MAP_SIZES[3] = {45, 85, 125};
static const int MAP_VERTICAL_PADDING = 2;
static const int MAP_HORIZONTAL_PADDING = 5;
static const int ENTER_KEY = 10;
static const int SPAWN_TOTAL_DENOM = 100;
static int enemy_spawn_rate = 2; // (enemy_spawn_rate/SPAWN_TOTAL_DENOM) chance to spawn per each 'tick'
static int chosen_map_size;
static int max_enemies = 2;
WINDOW *map_window;
WINDOW *combat_window;

void initiate_combat(Player *player, Enemy *enemy)
{
	wprintw(combat_window, "Combat has started\n");
	wrefresh(combat_window);
}

void enemyEvents(Player *player, Map *map, std::vector<Enemy> *enemies)
{
	int rng;
	int index = 0;
	for (auto &e : *enemies) {
		e.seek(player->getRow(), player->getCol());
		if (e.row == player->getRow() && e.col == player->getCol()) {
			//TODO:
			//	combat function
			//	first need items and enemy
			initiate_combat(player, &e);
			enemies->erase(enemies->begin() + index);
		}
		index++;
	}
	rng = (rand() % SPAWN_TOTAL_DENOM);
	if (rng <= enemy_spawn_rate && enemies->size() < max_enemies) {
		enemies->push_back(*(new Enemy(player->getRow(), player->getCol(), player->vision, map->size)));
	}
}

void mainGameLoop(Player *player, Map *map)
{
	int ch;
	std::vector<Enemy> enemies;	
	map->printMap(player->getRow(), player->getCol(), player->vision, enemies, map_window);

	while (true) {
		ch = getch();
		switch(ch) {
			case KEY_UP: case KEY_DOWN: case KEY_LEFT: case KEY_RIGHT:
			case 'w': case 's': case 'a': case 'd':
				player->moveSpace(ch, map->size);
				enemyEvents(player, map, &enemies);
				map->printMap(player->getRow(), player->getCol(), player->vision, enemies, map_window);
				break;
			case ENTER_KEY:
				//TODO:
				//	open command prompt. will be new window
				break;
			default:
				break;
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
	Player player;
	srand((int)time(0));

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
	curs_set(0);

	int index = userSelectMapSize();
	chosen_map_size = MAP_SIZES[index];
    Map map(chosen_map_size, MAP_VERTICAL_PADDING, MAP_HORIZONTAL_PADDING);

	player.userCreatePlayer();
	player.setPosition((int)chosen_map_size/2, (int)chosen_map_size/2);

	//constants are being put into non-constant ints because 
	//these values will change later but the original values
	//should still be obtainable
	int starty = MAP_VERTICAL_PADDING, startx = MAP_HORIZONTAL_PADDING;
	int height = 50, width = 50;

	map_window = newwin(height, width, starty, startx);
	combat_window = newwin(height, width, starty, width + (startx * 2));

	mainGameLoop(&player, &map);

	return 0;
}