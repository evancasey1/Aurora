#include <string>
#include <cstdlib>
#include <ctime>
#include "food.h"

Food::Food() {
	this->initial_health_gain = 5;     
	this->health_gain_per_trigger = 2; 
	this->turns_until_trigger = 5;
	this->total_triggers = 3; 
	this->trigger_count = 0; 
}