#include <string>
#include <ncurses.h>

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
		void printMap(int player_row, int player_col, int vision);
		int size;
		MapSection *map;

	private:
		char player_symbol;
		int y_pad = 5;
		int x_pad;
};