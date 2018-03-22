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
    this->total_triggers = 3; 
    this->trigger_count = 0; 
    this->name = "Food";
}

Food::Food(std::string name, int initial_health_gain, int health_gain_per_trigger, int total_triggers) 
{
    extern int item_id_counter;
    this->item_id = item_id_counter++;
    this->equipment_id = static_cast<int>(EquipmentType::Food);
    this->name = name;
    this->initial_health_gain = initial_health_gain;     
    this->health_gain_per_trigger = health_gain_per_trigger; 
    this->total_triggers = total_triggers; 
    this->trigger_count = 0; 
}

void Food::printType(WINDOW *win) 
{
    wprintw(win, "FOOD\n");
    wrefresh(win);
}

void Food::printDescription(WINDOW *item_description_window)
{
    wprintw(item_description_window, "Initial Health Gain: %d\nHealth per Tick: %d\nTotal Ticks: %d", this->initial_health_gain, this->health_gain_per_trigger, this->total_triggers);
    wrefresh(item_description_window);
}