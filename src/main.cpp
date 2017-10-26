/*
* AURORA
* Author: Evan Casey
* Date: Oct 17, 2017
*/

#include <iostream>
#include <string>
#include <ctype.h>
#include <ncurses.h>
#include <unistd.h>
#include "player.h"
#include "map.h"
#include "cursorUtilities.h"

static const int MAP_SIZES[3] = {45, 85, 125};
//static const int VERTICAL_PADDING = 2;
//static const int HORIZONTAL_PADDING = 5;
static int user_interface_buffer_height  = 50;
//static int command_input_cursor_position = 43;
static int chosen_map_size;

//from tldp.org
//tutorial on ncurses windows
WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 			* for the vertical and horizontal
					 			* lines			        */
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

//from tldp.org
//tutorial on ncurses windows
void destroy_win(WINDOW *local_win)
{	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	wrefresh(local_win);
	delwin(local_win);
}

void quitGame()
{
	std::cout << "\nExiting Aurora...\n";
	exit(0);
}

void mainGameLoop(Player *player, Map *map)
{
	//TODO:
	//  implement ncurses
	usleep(3000000);
	endwin();
	return;
}

int userSelectMapSize()
{
	//TODO:
	//	Support for custom map size
	int index = -1;
	std::string input_str;
	printw("Select your map size\n");
	printw("Tiny\nNormal\nHuge\n");
	refresh();

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
	}
	//clears the prompt output
	eraseLines(5);
	return index;
}

int main(int argc, char *argv[]) 
{
	//WINDOW *main_menu;
	Player player;

	initscr();
	cbreak();

	int index = userSelectMapSize();
	chosen_map_size = MAP_SIZES[index];
	Map map(chosen_map_size);

	//player.userCreatePlayer(command_input_cursor_position, user_interface_buffer_height);
	player.setPosition((int)chosen_map_size/2, (int)chosen_map_size/2);

	map.printMap(user_interface_buffer_height, player.getRow(), player.getCol(), player.vision);
	mainGameLoop(&player, &map);

	return 0;
}