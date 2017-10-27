/*
* AURORA
* Author: Evan Casey
* Date: Oct 17, 2017
*/
#include <iostream>
#include <string>
#include <ncurses.h>
#include <unistd.h>
#include "player.h"
#include "map.h"

static const int MAP_SIZES[3] = {45, 85, 125};
static int chosen_map_size;
static const int VERTICAL_PADDING = 5;
static const int ENTER_KEY = 10;

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
	printw("\nExiting Aurora...\n");
	refresh();
	endwin();
	exit(0);
}

void mainGameLoop(Player *player, Map *map)
{
	int ch;

	while (true) {
		ch = getch();
		switch(ch) {
			case KEY_UP: case KEY_DOWN: case KEY_LEFT: case KEY_RIGHT:
			case 'w': case 's': case 'a': case 'd':
				player->moveSpace(ch, map->size);
				map->printMap(player->getRow(), player->getCol(), player->vision);
				break;
			case ENTER_KEY:
				//TODO:
				//	open command prompt. will be new window
				break;
			default:
				break;
		}
	}
	endwin();
}

int userSelectMapSize()
{
	//TODO:
	//	Support for custom map size
	int chosen_index = 0;
	const char *options_sel[] =	{">> Tiny <<", ">> Normal <<", ">> Huge <<"};
	const char *options_idle[] = {"Tiny", "Normal", "Huge"};
	int horiz_pad = (int) ((COLS/2)-10);
	int ch = 0;
	
	mvprintw(VERTICAL_PADDING-1, horiz_pad, "Select your map size\n");
	while(ch != ENTER_KEY) {
		//prints contents of options[]
		//highlights currently selected option
		for (int i = 0; i < 3; i++) {
			if (i != chosen_index) {
				move(VERTICAL_PADDING + i, horiz_pad);
				clrtoeol();
				addstr(options_idle[i]);
			}
			else {
				move(VERTICAL_PADDING + i, horiz_pad);
				clrtoeol();
				addstr(options_sel[i]);			
			}
		}
		
		ch = getch();
		switch(ch) {
			case KEY_UP: case 'w':
				chosen_index <= 0 ? chosen_index = 2 : chosen_index--;
				refresh();
				break;
			case KEY_DOWN: case 's':
				chosen_index >= 2 ? chosen_index = 0 : chosen_index++;
				refresh();
				break;
			default:
				break;
		}
	}
	clear();
	refresh();
	return chosen_index;
}

int main(int argc, char *argv[]) 
{
	//WINDOW *main_menu;
	Player player;

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
	curs_set(0);

	int index = userSelectMapSize();
	chosen_map_size = MAP_SIZES[index];
    Map map(chosen_map_size);

	player.userCreatePlayer();
	player.setPosition((int)chosen_map_size/2, (int)chosen_map_size/2);

	map.printMap(player.getRow(), player.getCol(), player.vision);
	mainGameLoop(&player, &map);

	return 0;
}