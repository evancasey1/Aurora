#include <iostream>
#include <string>
#include "map.h"
#include "cursorUtilities.h"

Map::Map(int s) 
{
	this->size = s;
	this->player_symbol = '@';
	this->map = new MapSection[size * size];
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

void Map::printMap(int ui_buf_height, int player_row, int player_col, int vision) 
{
	//set position to top of terminal (-padding)
	setAbsoluteCursorPosition(this->VERTICAL_PADDING_MAP, 0);
	std::string padding(this->HORIZONTAL_PADDING_MAP, ' ');
	
	int row_max = player_row + vision + 1;
	int col_max = player_col + vision + 1;

	for (int i = player_row - vision; i < row_max; i++) {
		std::cout << padding;
		for (int j = player_col - vision; j < col_max; j++) {
			if (i == player_row && j == player_col) {
				std::cout << this->player_symbol << " ";
			}
			else {
				std::cout << this->getMapSectionFromIndex(i, j).symbol << " ";
			
			}
		}
		std::cout << "\n";
	}
	resetCommandInputCursorPosition(size, ui_buf_height);
}
