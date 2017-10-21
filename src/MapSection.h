#include <string>
#include "Structure.h"

class MapSection
{
	public:
		MapSection() 
		{
			this->biome  = "PLACEHOLDER";
			this->symbol = 'P';
		}
		MapSection(std::string biome, char sym) 
		{
			this->biome  = biome;
			this->symbol = sym;
		}
		char getSymbol(){return this->symbol;}
		std::string getBiome(){return this->biome;}

	private:
		//TODO
		//Dynamically size to random value in a range
		Structure structures[5];
		char symbol;
		std::string biome;
};