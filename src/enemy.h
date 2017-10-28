#include <string>
#ifndef ENEMY_H
#define ENEMY_H

class Enemy
{
	public:
		Enemy(int rw, int cl, int vi, int sz);
		void seek(int p_row, int p_col);
		int row;
		int col;

	private:
		int health;
		int vision;
};

#endif