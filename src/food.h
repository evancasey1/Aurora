#ifndef FOOD_H
#define FOOD_H
#include "equipment.h"

class Food : public Equipment 
{
    public:
        Food();
        Food(std::string name, int initial_health_gain, int health_gain_per_trigger, int total_triggers);
        void printDescription(WINDOW *win);
        void printType(WINDOW *win);
        void compareTo(Food food, WINDOW *win){printDescription(win);}; //This looks redundant, but is required to work smoothly with 
                                                                        //the boost visitors. It's a necessary evil.
        
        std::string name;
        int initial_health_gain;     //Health boost right after eating
        int health_gain_per_trigger; //Amount of health gained when health is gained over time
        int total_triggers;          //Number of times health is gained until food wears off
        int trigger_count;           //Keeps track of the above variable. Always initialized to 0
};

#endif