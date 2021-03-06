#ifndef VISITORS_H
#define VISITORS_H
#include <boost/variant.hpp>
#include <ncurses.h>
#include <vector>
#include "player.h"
#include "equipmenttype.h"

class Visitors 
{

    public:
        struct output_desc : public boost::static_visitor<>
        {
            WINDOW *win;
            explicit output_desc(WINDOW *item_description_window) : win(item_description_window) {}
            
            template <typename T>
            void operator()(T t) const 
            { 
                t.printType(win); 
                t.printDescription(win); 
            }
        };

        struct output_inv_desc : public boost::static_visitor<>
        {
            WINDOW *win;
            explicit output_inv_desc(WINDOW *item_description_window) : win(item_description_window) {}
            
            template <typename T>
            void operator()(T t) const 
            { 
                t.printDescription(win); 
            }
        };

        struct output_list_name : public boost::static_visitor<>
        {
            int counter;
            WINDOW *win;
            explicit output_list_name(int item_counter, WINDOW *inventory_window) : counter(item_counter), win(inventory_window) {}

            template <typename T>
            void operator()(T t) const 
            { 
                wprintw(win, "> [%d] %s\n", counter, (t.name).c_str()); 
            }
        };

        struct compare_to : public boost::static_visitor<>
        {
            boost::variant<Weapon, Armor, Food> item;
            WINDOW *win;
            explicit compare_to(boost::variant<Weapon, Armor, Food> equipped, WINDOW *item_description_window) : item(equipped), win(item_description_window) {}

            template <typename T>
            void operator()(T t) const 
            {
                T raw_item = boost::get<T>(item);
                t.compareTo(raw_item, win);
            }
        };

        struct output_name : public boost::static_visitor<>
        {
            WINDOW *win;
            explicit output_name(WINDOW *window) : win(window) {}

            template <typename T>
            void operator()(T t) const 
            { 
                wprintw(win, "%s\n", (t.name).c_str()); 
            }
        };

        struct get_name : public boost::static_visitor<const char*>
        {
            template <typename T>
            const char* operator()(T t) const 
            {
                return (t.name).c_str(); 
            }
        };

        struct pick_up : public boost::static_visitor<bool>
        {
            Player *p;
            explicit pick_up(Player *player) : p(player) {}
            template <typename T>
            bool operator()(T t) const 
            { 
                return p->pickupLoot(t); 
            }
        };

        struct get_item_id : public boost::static_visitor<int>
        {
            template <typename T>
            int operator()(T t) const 
            { 
                return t.item_id; 
            }
        };


};


#endif