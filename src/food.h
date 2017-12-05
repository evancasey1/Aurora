#ifndef FOOD_H
#define FOOD_H

class Food {
	public:
		Food();

		int initial_health_gain;     //Health boost right after eating
		int health_gain_per_trigger; //Amount of health gained when health is gained over time
		int turns_until_trigger;     //Every X moves health is gained
		int total_triggers;          //Number of times health is gained until food wears off
		int trigger_count;           //Keeps track of the above variable
};

#endif