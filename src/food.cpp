#include <string>
#include <cstdlib>
#include <ctime>
#include "food.h"

Food::Food() {
	this->equipment_id = 1;
	this->initial_health_gain = 5;     
	this->health_gain_per_trigger = 2; 
	this->turns_until_trigger = 5;
	this->total_triggers = 3; 
	this->trigger_count = 0; 
}

Food::Food(std::string name, int initial_health_gain, int health_gain_per_trigger, int turns_until_trigger, int total_triggers) {
	this->equipment_id = 1;
	this->name = name;
	this->initial_health_gain = initial_health_gain;     
	this->health_gain_per_trigger = health_gain_per_trigger; 
	this->turns_until_trigger = turns_until_trigger;
	this->total_triggers = total_triggers; 
	this->trigger_count = 0; 
}
