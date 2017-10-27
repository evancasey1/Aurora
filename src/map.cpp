#include <iostream>
#include <string>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include "map.h"

Map::Map(int s) 
{
	this->size = s;
	this->player_symbol = '@';
	this->map = new MapSection[size * size];
	this->x_pad = (int)(COLS/2);
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

void Map::printMap(int player_row, int player_col, int vision) 
{
	int row_max = player_row + vision + 1;
	int col_max = player_col + vision + 1;
	int row_min = player_row - vision;
	int col_min = player_col - vision;
	int startx  = this->x_pad - vision - 3;

	row_max > this->size ? row_max = this->size : false;
	col_max > this->size ? col_max = this->size : false;
	row_min < 0 ? row_min = 0 : false;
	col_min < 0 ? col_min = 0 : false;
	startx < 0 ? startx = 0 : false;
	/*
	printw("col: %d\n", player_col);
	printw("row: %d\n", player_row);
	printw("row_max: %d\n", row_max);
	printw("col_max: %d\n", col_max);
	printw("row_min: %d\n", row_min);
	printw("col_min: %d\n", col_min);
	printw("startx: %d", startx);
	*/
	int count = 0;
	for (int i = row_min; i < row_max; i++) {
		move(this->y_pad + count, startx);
		for (int j = col_min; j < col_max; j++) {
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
		clrtoeol();
		refresh();
	}
}
