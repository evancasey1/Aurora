#ifndef VISITORS_H
#define VISITORS_H
#include <boost/variant.hpp>
#include <ncurses.h>

class Visitors 
{

	public:
		struct output_desc : public boost::static_visitor<>
		{
			WINDOW *win;
			explicit output_desc(WINDOW *item_description_window) : win(item_description_window) {}
			template <typename T>
			void operator()(T t) const { t.printDescription(win); }
		};

		struct output_name : public boost::static_visitor<>
		{
			int counter;
			WINDOW *win;
			output_name(int item_counter, WINDOW *inventory_window) : counter (item_counter), win(inventory_window) {}

			template <typename T>
			void operator()(T t) const { wprintw(win, "> [%d] %s\n", counter, (t.name).c_str()); }
		};
};


#endif