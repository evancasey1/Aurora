#include <string>
#include "Structure.h"

class MapSection
{
	public:
		MapSection(std::string biome);
		static std::string biome;

	private:
		Structure structures[5];
};