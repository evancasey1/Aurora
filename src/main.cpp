/*
* AURORA
* Author: Evan Casey
* Date: Oct 17, 2017
*/

#include <iostream>
#include <string>
#include <ctype.h>
#include "Player.h"
#include "MapSection.h"
#include "CursorUtilities.h"

static const int MAP_SIZES[3] = {20, 30, 40};
static const int VERTICAL_PADDING = 2;
static const int HORIZONTAL_PADDING = 0; //0 for now. Exists in case I want to try using it later
static int user_interface_buffer_height = getWindowHeight();
static int command_input_cursor_position = 43;
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
			eraseLines(1);
			std::cout << ">> ";
		}
	}
	//clears the prompt output
	eraseLines(5);
	return index;
}

Player userCreatePlayer()
{
	//TODO:
	// 	Name selection
	//	Attribute selection
	//  break into 3 helper functions
	//		get name, get race, get attrs
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
			eraseLines(1);
			std::cout << ">> ";
		}
	}
	input_str = "";
	std::cout << "What is your name?\n>> ";
	while(!input_str.compare("")) {
		std::cin >> input_str;
		for(int i = 0; input_str[i]; i++) {
			if (!isalpha(input_str[i]) && !isblank(input_str[i])) {
				input_str = "";
				std::cout << "Your name must only be alphabetical.\r";
				//move cursor up by 1
				moveCursor(1, 0, 0, 0);
				std::cout << ">> \033[K";
				break;
			}
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

	clearUserInterfaceBuffer(command_input_cursor_position);
	int index = userSelectMapSize();
	chosen_map_size = MAP_SIZES[index];
	MapSection *map = new MapSection[chosen_map_size * chosen_map_size];
	printMap(chosen_map_size, user_interface_buffer_height, map);
	Player player = userCreatePlayer();

	return 0;
}