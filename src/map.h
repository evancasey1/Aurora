#include <string>
#include <ncurses.h>
#include "cursorUtilities.h"

class Map
{
	public:
		struct MapSection
		{
			char symbol;
			std::string biome;
			//will contain structure information later
		};
		Map(int size);
		MapSection getMapSectionFromIndex(int row, int col);
		void printMap(int ui_buf_height, int player_row, int player_col, int vision);
		
		MapSection *map;

	private:
		int size;
		char player_symbol;
		int y_pad = 5;
		int x_pad;
};