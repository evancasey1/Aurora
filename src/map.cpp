#include <iostream>
#include <string>
#include "map.h"
#include "cursorUtilities.h"

Map::Map(int s) 
{
	this->size = s;
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

void Map::printMap(int ui_buf_height) 
{
	//set position to top of terminal (-padding)
	setAbsoluteCursorPosition(this->VERTICAL_PADDING_MAP, 0);
	std::string padding(this->HORIZONTAL_PADDING_MAP, ' ');
	for (int i = 0; i < this->size; i++) {
		std::cout << padding;
		for (int j = 0; j < this->size; j++) {
			std::cout << this->getMapSectionFromIndex(i, j).symbol << " ";
		}
		std::cout << "\n";
	}
	resetCommandInputCursorPosition(size, ui_buf_height);
}
