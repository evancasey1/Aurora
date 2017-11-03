#include <string>
#include <ncurses.h>
#include <vector>
#include "enemy.h"
#ifndef MAP_H
#define MAP_H

class Map
{
	public:
		struct MapSection
		{
			char symbol;
			std::string biome;
			//will contain structure information later
		};
		Map(int s, int vp, int hp);
		MapSection getMapSectionFromIndex(int row, int col);
		void printMap(int player_row, int player_col, int vision, std::vector<Enemy> e, WINDOW *win);
		int size;
		MapSection *map;
		int player_color;
		int enemy_color;
		int daytime_color;
		int nighttime_color;

	private:
		char player_symbol;
		char enemy_symbol;
		int pad_y;
		int pad_x;
};

#endif