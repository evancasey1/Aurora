#ifndef FOOD_H
#define FOOD_H
#include "equipment.h"

class Food : public Equipment 
{
	public:
		Food();
		Food(std::string name, int initial_health_gain, int health_gain_per_trigger, int turns_until_trigger, int total_triggers);

		std::string name;
		int initial_health_gain;     //Health boost right after eating
		int health_gain_per_trigger; //Amount of health gained when health is gained over time
		int turns_until_trigger;     //Every X moves health is gained
		int total_triggers;          //Number of times health is gained until food wears off
		int trigger_count;           //Keeps track of the above variable. Always initialized to 0
};

#endif