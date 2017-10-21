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
static int user_interface_buffer_height = 50;
static int command_input_cursor_position = 40; //it doesnt make very much sense why this can be less than ui_buf (above). TODO: understand this
static int chosen_map_size;

void quitGame()
{
	std::cout << "\nExiting Aurora...\n";
	exit(0);
}

void mainGameLoop(Player *player, MapSection *map[])
{
	std::cout << "\nThis will be the main game loop\n";
}

//sets the cursor position regardless of where the cursor currently is.
//in other words, sets it relative to the terminal indexing
void setAbsoluteCursorPosition(int row, int col) {
	//set absolute position to (0, 0)
	std::cout << "\033[1;0H";
	//move cursor down by <row>
	if (row > 0) {
		std::cout << "\033[" + std::to_string(row) + "B";
	}
	//move cursor right by <col>
	if (col > 0) {
		std::cout << "\033[" + std::to_string(col) + "C";
	}
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
	int index = -1;
	setAbsoluteCursorPosition(command_input_cursor_position, 0);
	std::string input_str;
	std::cout << "(1) Tiny\n(2) Normal\n(3) Huge\n";
	std::cout << "Select your map size:\n>> ";

	while (index == -1) {
		std::cin >> input_str;
		for(int i = 0; input_str[i]; i++) {
			input_str[i] = tolower(input_str[i]);
		}
		if (!input_str.compare("1") || !input_str.compare("tiny")) {
			index = 0;
		}
		else if (!input_str.compare("2") || !input_str.compare("normal")) {
			index = 1;
		}
		else if (!input_str.compare("3") || !input_str.compare("huge")) {
			index = 2;
		}
		else if (!input_str.compare("quit") || !input_str.compare("exit")) {
			quitGame();
		}
		else {
			//Invalid option. Erase to end of line and 
			//move up a row to prevent scrolling of terminal
			std::cout << "\033[1A\033[K";
			std::cout << ">> ";
		}
	}
	//This is sort of messy, but essentially it clears the 
	//5 lines written by this function. This needs to be a 
	//utility function at some point
	for (int i = 0; i < 6; i++) {
		std::cout << "\033[1A\033[K";
	}
	return index;
}

Player userCreatePlayer()
{
	//TODO:
	// 	Name selection
	//	Attribute selection
	setAbsoluteCursorPosition(command_input_cursor_position, 0);

	Player player;
	std::string input_str;
	std::string race;
	std::string name;
	std::cout << "(1) Human\n(2) Dwarf\n(3) Elf\n";
	std::cout << "Select your race:\n>> ";

	//while player race is Undefined (default value) keep prompting
	while (!player.getRace().compare("Undefined")) {
		std::cin >> input_str;
		for(int i = 0; input_str[i]; i++) {
			input_str[i] = tolower(input_str[i]);
		}
		if (!input_str.compare("1") || !input_str.compare("human")
			|| !input_str.compare("2") || !input_str.compare("dwarf")
			|| !input_str.compare("3") || !input_str.compare("elf")) {

			player.setRace(input_str);
		}
		else if (!input_str.compare("quit") || !input_str.compare("exit")) {
			quitGame();
		}
		else {
			//Invalid option. Erase to end of line and 
			//move up a row to prevent scrolling of terminal
			std::cout << "\033[1A\033[K";
			std::cout << ">> ";
		}
	}

	return player;
}

MapSection getMapSectionFromIndex(int row, int col, MapSection map[])
{
	return *(map + ((row * chosen_map_size) + col));
}

void printMap(int size, int ui_buf_height, MapSection map[]) {
	//set position to top of terminal (-padding)
	setAbsoluteCursorPosition(VERTICAL_PADDING, HORIZONTAL_PADDING);
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
	clearUserInterfaceBuffer(command_input_cursor_position);
	
	int index = userSelectMapSize();
	chosen_map_size = MAP_SIZES[index];
	MapSection *map = new MapSection[chosen_map_size * chosen_map_size];
	printMap(chosen_map_size, user_interface_buffer_height, map);

	Player player = userCreatePlayer();
	//clearUserInterfaceBuffer(command_input_cursor_position);
	return 0;
}