#include <string>
#include <ncurses.h>
#include <vector>
#include "enemy.h"
#include "player.h"
#ifndef MAP_H
#define MAP_H

//TODO:
//  Terrain generation in a more organized fashion
//      Different terrains will have different symbols, obstacles, enemies, and challenges  
//      Organize like a map, with terrain blocks grouped together and not randomly strewn about.
//  ---
//  Generate obstacles. Will need Enemy A* pathfinding before this can happen.
//  ---
//  Generate buildings. 
//      These structures will be denoted by a grouping of 4 or more concurrent
//      symbols on the map, and when entered will have their own map and enemy generation.

class Map
{
    public:
        struct MapSection
        {
            char symbol;
            //will contain structure information later
        };
        Map();
        Map(int s);
        MapSection getMapSectionFromIndex(int row, int col);
        void printMap(Player *player, int vision, std::vector<Enemy> e, std::vector<Enemy::Loot> loot, WINDOW *win);
        void printPlayerInfo(Player player, WINDOW *win);
        int getSectionID(int row, int col);
        bool isValidMove(int row, int col);
        bool isValidMove(int id);
        int size;
        MapSection *map;
        int player_color;
        int enemy_color;
        int daytime_color;
        int nighttime_color;

    private:
        char player_symbol;
        char loot_symbol;
};

#endif