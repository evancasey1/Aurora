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
		Map(int s);
		MapSection getMapSectionFromIndex(int row, int col);
		void printMap(int player_row, int player_col, int vision, std::vector<Enemy> e);
		int size;
		MapSection *map;

	private:
		char player_symbol;
		char enemy_symbol;
		int y_pad = 5;
		int x_pad;
};

#endif