#include <iostream>
#include <string>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "map.h"
#include "player.h"

Map::Map(int s) 
{
    this->size = s;
    this->player_symbol = '@';
    this->loot_symbol = '*';
    this->map = new MapSection[size * size];
    
    this->player_color = 4;
    this->enemy_color = 3;
    this->nighttime_color = 2;
    this->daytime_color = 1;

    double r;
    for (int i = 0; i < s; i++) {
        for (int j = 0; j < s; j++) {
            r = (rand() % 2) + 45;
            (this->map + ((i * this->size) + j))->symbol = r;
            r = ((double) rand() / RAND_MAX);
            if (r < .05) {
                (this->map + ((i * this->size) + j))->symbol = '#';
            }
            
        }
    }
}

Map::MapSection Map::getMapSectionFromIndex(int row, int col)
{
    return *(this->map + ((row * this->size) + col));
}

int Map::getSectionID(int row, int col)
{
    return (row * this->size) + col;
}

bool Map::isValidMove(int row, int col, std::vector<Enemy> enemies)
{
    for (auto &e : enemies) {
        if (row == e.row && col == e.col) {
            return false;
        }
    }
    char arr[] = {loot_symbol, '.', '-'};
    std::vector<int>valid_syms(arr, arr + sizeof(arr) / sizeof(arr[0]));
    if (row < size && col < size && row >= 0 && col >= 0) {
        char sym = getMapSectionFromIndex(row, col).symbol;
        if (std::find(valid_syms.begin(), valid_syms.end(), sym) != valid_syms.end()) {
            return true;
        }
    }
    return false;
}

bool Map::isValidMove(int id, std::vector<Enemy> enemies)
{   
    int t_id;
    for (auto &e : enemies) {
        t_id = this->getSectionID(e.row, e.col);
        if (t_id == id) {
            return false;
        }
    }
    char arr[] = {loot_symbol, '.', '-'};
    std::vector<int>valid_syms(arr, arr + sizeof(arr) / sizeof(arr[0]));
    if (id < size * size && id >= 0) {
        char sym = (*(this->map + id)).symbol;
        if (std::find(valid_syms.begin(), valid_syms.end(), sym) != valid_syms.end()) {
            return true;
        }
    }
    return false;
}

void Map::printPlayerInfo(Player player, WINDOW *map_window)
{
    wmove(map_window, 1, 0);
    wprintw(map_window, "X: %d Y: %d     Moves: %d\n", player.col, abs(player.row - (this->size - 1)), player.allowed_moves - player.used_moves);
    wrefresh(map_window);
}

void Map::printMap(Player *player, int vision, std::vector<Enemy> enemies, std::vector<Enemy::Loot> loot, WINDOW *map_window) 
{
    //TODO:
    //  reduce flicker
    wclear(map_window);
    wmove(map_window, 2, 0);
    char to_add;
    int row_max = player->row + vision + 1;
    int col_max = player->col + vision + 1;
    int row_min = player->row - vision;
    int col_min = player->col - vision;
    int color_index = this->daytime_color; //init to daytime for now

    row_max > this->size ? row_max = this->size : false;
    col_max > this->size ? col_max = this->size : false;
    row_min < 0 ? row_min = 0 : false;
    col_min < 0 ? col_min = 0 : false;

    int count = 0;
    for (int i = row_min; i < row_max; i++) {
        for (int j = col_min; j < col_max; j++) {
            to_add = this->getMapSectionFromIndex(i, j).symbol;

            for (int k = 0; k < loot.size(); k++) {
                if (loot.at(k).row == i && loot.at(k).col == j) {
                    to_add = this->loot_symbol;
                }
            }   

            if (player->row == i && player->col == j) {
                to_add = this->player_symbol;
                color_index = this->player_color;
            }       

            for (auto &e : enemies) {
                if (e.row == i && e.col == j) {
                    to_add = e.symbol;
                    color_index = this->enemy_color;
                    break;
                }
            }
            wattron(map_window, COLOR_PAIR(color_index));
            waddch(map_window, to_add);
            wprintw(map_window, " ");
            wattroff(map_window, COLOR_PAIR(color_index));
            color_index = 1;
        }
        count++;
        wprintw(map_window, "\n");
    }
    printPlayerInfo(*player, map_window);
    wrefresh(map_window);
}
