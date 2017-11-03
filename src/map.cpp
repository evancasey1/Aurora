#include <iostream>
#include <string>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "map.h"

Map::Map(int s, int vp, int hp) 
{
	this->size = s;
	this->player_symbol = '@';
	this->enemy_symbol = 'X';
	this->map = new MapSection[size * size];
	this->pad_y = vp;
	this->pad_x = hp;
	init_pair(4, COLOR_BLACK, COLOR_WHITE); //player
	init_pair(3, COLOR_RED, COLOR_WHITE); //enemy
	init_pair(2, COLOR_WHITE, COLOR_BLACK); //default night
	init_pair(1, COLOR_BLACK, COLOR_WHITE); //default day
	this->player_color = 4;
	this->enemy_color = 3;

	srand((int)time(0));
	int r;
	for (int i = 0; i < s; i++) {
		for (int j = 0; j < s; j++) {
			r = (rand() % 2) + 45;
			(this->map + ((i * this->size) + j))->symbol = r;
		}
	}
}

Map::MapSection Map::getMapSectionFromIndex(int row, int col)
{
	return *(this->map + ((row * this->size) + col));
}

void Map::printMap(int player_row, int player_col, int vision, std::vector<Enemy> enemies, WINDOW *map_window) 
{
	//TODO:
	//	reduce flicker
	
	wmove(map_window, this->pad_y, this->pad_x);
	char to_add;
	int row_max = player_row + vision + 1;
	int col_max = player_col + vision + 1;
	int row_min = player_row - vision;
	int col_min = player_col - vision;
	int color_index = 1; //init to daytime for now

	row_max > this->size ? row_max = this->size : false;
	col_max > this->size ? col_max = this->size : false;
	row_min < 0 ? row_min = 0 : false;
	col_min < 0 ? col_min = 0 : false;

	wprintw(map_window, " X: %d Y: %d\n", player_col, player_row);
	int count = 0;
	for (int i = row_min; i < row_max; i++) {
		for (int j = col_min; j < col_max; j++) {
			if (player_row == i && player_col == j) {
				to_add = this->player_symbol;
				color_index = this->player_color;
			}
			else {
				to_add = this->getMapSectionFromIndex(i, j).symbol;
			}
			for (auto &e : enemies) {
				if (e.row == i && e.col == j) {
					to_add = this->enemy_symbol;
					color_index = this->enemy_color;
					break;
				}
			}
			wattron(map_window, COLOR_PAIR(color_index));
			waddch(map_window, to_add);
			wprintw(map_window, " ");
			wattroff(map_window, COLOR_PAIR(color_index));
			color_index = 1;
		}
		count++;
		wprintw(map_window, "\n");
		if (player_row + vision > this->size - 1) {
			wmove(map_window, player_row + vision, 0);
			wclrtoeol(map_window);
		}
		else if (player_row - vision < 0) {
			wmove(map_window, player_row - vision, 0);
			wclrtoeol(map_window);
		}
		
		wrefresh(map_window);
	}
	wmove(map_window, this->pad_y, this->pad_x);
}
