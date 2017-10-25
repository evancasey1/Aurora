#include <string>
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
		void printMap(int ui_buf_height);

		MapSection *map;

	private:
		int size;
		const int VERTICAL_PADDING_MAP = 5;
		const int HORIZONTAL_PADDING_MAP = 10;
};