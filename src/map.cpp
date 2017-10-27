#include <iostream>
#include <string>
#include <ncurses.h>
#include "map.h"

Map::Map(int s) 
{
	this->size = s;
	this->player_symbol = '@';
	this->map = new MapSection[size * size];
	this->x_pad = (int)(COLS/2);
	for (int i = 0; i < s; i++) {
		for (int j = 0; j < s; j++) {
			(this->map + ((i * this->size) + j))->symbol = 'X';
		}
	}
}

Map::MapSection Map::getMapSectionFromIndex(int row, int col)
{
	return *(this->map + ((row * this->size) + col));
}

void Map::printMap(int player_row, int player_col, int vision) 
{
	//set position to top of terminal (-padding)
	int row_max = player_row + vision + 1;
	int col_max = player_col + vision + 1;
	int count = 0;
	for (int i = player_row - vision; i < row_max; i++) {
		move(this->y_pad + count, this->x_pad - vision - 1);
		for (int j = player_col - vision; j < col_max; j++) {
			if (i == player_row && j == player_col) {
				addch(this->player_symbol);
			}
			else {
				addch(this->getMapSectionFromIndex(i, j).symbol);
			}
			printw(" ");
		}
		count++;
		printw("\n");
		refresh();
	}
}
