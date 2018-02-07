#include <ncurses.h>
#include <string>
#ifndef EQUIPMENT_H
#define EQUIPMENT_H

class Equipment
{
    public:
        Equipment(){};
        void printDescription(WINDOW *win);

        int equipment_id;
        int item_id;
        std::string name;
};


#endif