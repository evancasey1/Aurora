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
	wmove(map_window, this->pad_y, this->pad_x);
	char to_add;
	int row_max = player_row + vision + 1;
	int col_max = player_col + vision + 1;
	int row_min = player_row - vision;
	int col_min = player_col - vision;

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
			}
			else {
				to_add = this->getMapSectionFromIndex(i, j).symbol;
			}
			for (auto &e : enemies) {
				if (e.row == i && e.col == j) {
					to_add = this->enemy_symbol;
				}
			}
			waddch(map_window, to_add);
			wprintw(map_window, " ");
		}
		count++;
		wprintw(map_window, "\n");
		wclrtoeol(map_window);
		wrefresh(map_window);
	}
	wmove(map_window, this->pad_y, this->pad_x);
}
