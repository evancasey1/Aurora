/*
* AURORA
* Author: Evan Casey
* Date: Oct 17, 2017
*/

#include <iostream>
#include <string>
#include "Player.h"
#include "MapSection.h"

static const int MAP_SIZE_SMALL  = 25;
//static const int MAP_SIZE_MEDIUM = 50;
//static const int MAP_SIZE_LARGE  = 75;
//static const int MAP_SIZES[3] = {MAP_SIZE_SMALL, MAP_SIZE_MEDIUM, MAP_SIZE_LARGE};
int chosen_map_size = 0;

MapSection getMapSectionFromIndex(int row, int col, MapSection map[])
{
	return *(map + ((row * chosen_map_size) + col));
}

void print_map(int size, MapSection map[]) {

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << getMapSectionFromIndex(i, j, map).getSymbol() << " ";
		}
		std::cout << "\n";
	}
}

int main(int argc, char *argv[]) 
{
	//TODO
	//Dynamic map size

	chosen_map_size = MAP_SIZE_SMALL;
	MapSection map[MAP_SIZE_SMALL * MAP_SIZE_SMALL];
	print_map(chosen_map_size, map);

	return 0;
}