#include <string>
#include <cstdlib>
#include <ctime>
#include "food.h"
#include "equipmenttype.h"

Food::Food() 
{
	extern int item_id_counter;
	this->item_id = item_id_counter++;
	this->equipment_id = static_cast<int>(EquipmentType::Food);
	this->initial_health_gain = 5;     
	this->health_gain_per_trigger = 2; 
	this->turns_until_trigger = 5;
	this->total_triggers = 3; 
	this->trigger_count = 0; 
	this->turn_count = 0;
}

Food::Food(std::string name, int initial_health_gain, int health_gain_per_trigger, int turns_until_trigger, int total_triggers) 
{
	extern int item_id_counter;
	this->item_id = item_id_counter++;
	this->equipment_id = static_cast<int>(EquipmentType::Food);
	this->name = name;
	this->initial_health_gain = initial_health_gain;     
	this->health_gain_per_trigger = health_gain_per_trigger; 
	this->turns_until_trigger = turns_until_trigger;
	this->total_triggers = total_triggers; 
	this->trigger_count = 0; 
	this->turn_count = 0;
}

void Food::printDescription(WINDOW *item_description_window)
{
	wprintw(item_description_window, "FOOD\nHealth Gain: %d\nHeal Over Time: %d:%d:%d", this->initial_health_gain, this->health_gain_per_trigger, this->total_triggers, this->turns_until_trigger);
	wrefresh(item_description_window);
}