/*
* AURORA
* Author: Evan Casey
* Date: Oct 17, 2017
*/

#include <iostream>
#include <string>
#include "Player.h"
#include "MapSection.h"

static const int MAP_SIZES[3] = {20, 35, 45};
static const int VERTICAL_PADDING = 2;
static const int HORIZONTAL_PADDING = 0; //0 for now. Exists in case I want to try using it later
static int user_interface_buffer_height = 53;
static int command_input_cursor_position = user_interface_buffer_height + 4;
static int chosen_map_size;

void setCursorPosition(int row, int col) {
	row = row < 0 ? 1 : col;
	col = col < 0 ? 1 : col;
	std::cout << "\033[" + std::to_string(row) + ";" + std::to_string(col) + "H";
}

void resetCommandInputCursorPosition(int current_cursor_pos, int ui_buf_height)
{
	//move cursor down the remaining lines to reset buffer
	//for consistent text input area
	std::cout << "\033[" + std::to_string(current_cursor_pos - ui_buf_height) + "N";
}

void clearUserInterfaceBuffer(int height) 
{
	// "\033[2J" clears the screen from top to bottom
	// "\033[<height>;1H" puts the cursor down to row <height> and col 1;
	std::cout << "\033[2J\033[" + std::to_string(height) + ";1H";
}

int userSelectMapSize()
{
	//TODO:
	//	Support for custom map size 
	//setCursorPosition(command_input_cursor_position, 1);
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

void printMap(int size, int ui_buf_height, MapSection map[]) {
	//set position to top of terminal (-padding)
	setCursorPosition(VERTICAL_PADDING, HORIZONTAL_PADDING);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			std::cout << getMapSectionFromIndex(i, j, map).getSymbol() << " ";
		}
		std::cout << "\n";
	}
	resetCommandInputCursorPosition(size, ui_buf_height);
}

int main(int argc, char *argv[]) 
{
	//TODO:
	//	Main game loop

	//this is the clear area that will be the display area for the
	//map and general ui area. Text input area is below it.
	//Will be customizable later.
	clearUserInterfaceBuffer(user_interface_buffer_height);
	
	int index = userSelectMapSize();
	if (index == -1) {
		std::cout << "\nExiting Aurora...\n";
		exit(0);
	}
	chosen_map_size = MAP_SIZES[index];
	MapSection *map = new MapSection[chosen_map_size * chosen_map_size];
	printMap(chosen_map_size, user_interface_buffer_height, map);

	//clearUserInterfaceBuffer(command_input_cursor_position);
	return 0;
}