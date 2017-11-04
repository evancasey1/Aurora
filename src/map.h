#include <string>
#include <ncurses.h>
#include <vector>
#include "enemy.h"
#include "player.h"
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
		void printMap(Player *player, int vision, std::vector<Enemy> e, WINDOW *win);
		void printPlayerInfo(Player player, WINDOW *win);
		int size;
		MapSection *map;
		int player_color;
		int enemy_color;
		int daytime_color;
		int nighttime_color;

	private:
		char player_symbol;
		char enemy_symbol;
};

#endif