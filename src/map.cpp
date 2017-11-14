#include <iostream>
#include <string>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "map.h"
#include "player.h"

Map::Map(int s) 
{
	this->size = s;
	this->player_symbol = '@';
	this->map = new MapSection[size * size];
	
	this->player_color = 4;
	this->enemy_color = 3;
	this->nighttime_color = 2;
	this->daytime_color = 1;

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

void Map::printPlayerInfo(Player player, WINDOW *map_window)
{
	wmove(map_window, 0, 0);
	wprintw(map_window, "X: %d Y: %d --- Moves: %d/%d\n", player.getCol(), abs(player.getRow() - (this->size - 1)), player.used_moves, player.allowed_moves);
	wrefresh(map_window);
}

void Map::printMap(Player *player, int vision, std::vector<Enemy> enemies, WINDOW *map_window) 
{
	//TODO:
	//	reduce flicker

	wmove(map_window, 1, 0);
	char to_add;
	int row_max = player->getRow() + vision + 1;
	int col_max = player->getCol() + vision + 1;
	int row_min = player->getRow() - vision;
	int col_min = player->getCol() - vision;
	int color_index = this->daytime_color; //init to daytime for now

	row_max > this->size ? row_max = this->size : false;
	col_max > this->size ? col_max = this->size : false;
	row_min < 0 ? row_min = 0 : false;
	col_min < 0 ? col_min = 0 : false;

	int count = 0;
	for (int i = row_min; i < row_max; i++) {
		for (int j = col_min; j < col_max; j++) {
			if (player->getRow() == i && player->getCol() == j) {
				to_add = this->player_symbol;
				color_index = this->player_color;
			}
			else {
				to_add = this->getMapSectionFromIndex(i, j).symbol;
			}
			for (auto &e : enemies) {
				if (e.row == i && e.col == j) {
					to_add = e.symbol;
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
		if (player->getRow() + vision > this->size - 1) {
			wmove(map_window, player->getRow() + vision, 0);
			wclrtoeol(map_window);
		}
		else if (player->getRow() - vision < 0) {
			wmove(map_window, player->getRow() - vision, 0);
			wclrtoeol(map_window);
		}
		
		wrefresh(map_window);
	}
}
