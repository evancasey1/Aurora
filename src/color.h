#ifndef COLOR_H
#define COLOR_H
#include <ncurses.h>

/*
* Convention 'ForegroundBackground'
*	I.E. WhiteBlack -> White text color with black highlight
*/
enum class Color : int {
    BlackWhite = COLOR_PAIR(1),
    WhiteBlack = COLOR_PAIR(2),
    RedWhite = COLOR_PAIR(3),
    BlueWhite = COLOR_PAIR(4),
    RedBlack = COLOR_PAIR(5),
    GreenBlack = COLOR_PAIR(6),
    CyanBlack = COLOR_PAIR(7),
    YellowBlack = COLOR_PAIR(8),
    MagentaBlack = COLOR_PAIR(9)
};

#endif