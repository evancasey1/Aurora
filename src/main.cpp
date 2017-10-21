/*
* AURORA
* Author: Evan Casey
* Date: Oct 17, 2017
*/

#include <iostream>
#include <string>
#include "Player.h"
#include "MapSection.h"

static const int MAP_SIZES[3] = {20, 35, 50};
static int chosen_map_size = 0;

int userSelectMapSize()
{
	//TODO:
	//	Support for custom map size 
	std::string input_str;
	std::cout << "(1) Tiny\n(2) Normal\n(3) Epic\n";
	std::cout << "Select your map size:\n>> "; 

	while (true) {
		std::cin >> input_str;
		for(int i = 0; input_str[i]; i++) {
			input_str[i] = tolower(input_str[i]);
		}
		if (!input_str.compare("1") || !input_str.compare("tiny")) {
			return 0;
		}
		else if (!input_str.compare("2") || !input_str.compare("normal")) {
			return 1;
		}
		else if (!input_str.compare("3") || !input_str.compare("epic")) {
			return 2;
		}
		else if (!input_str.compare("quit") || !input_str.compare("exit")) {
			return -1;
		}
		else {
			std::cout << "Please enter a valid option.\n>> ";
		}
	}
}

MapSection getMapSectionFromIndex(int row, int col, MapSection map[])
{
	return *(map + ((row * chosen_map_size) + col));
}

void printMap(int size, MapSection map[]) {

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << getMapSectionFromIndex(i, j, map).getSymbol() << " ";
		}
		std::cout << "\n";
	}
}

int main(int argc, char *argv[]) 
{
	//TODO:
	//	Main game loop

	int index = userSelectMapSize();
	if (index == -1) {
		std::cout << "\nExiting Aurora...\n";
		exit(0);
	}
	chosen_map_size = MAP_SIZES[index];
	MapSection *map = new MapSection[chosen_map_size * chosen_map_size];
	printMap(chosen_map_size, map);

	return 0;
}