#include "enemy.h"
#ifndef WOLF_H
#define WOLF_H

class Wolf : Enemy {
	public:
		Wolf();
		int bite();
		int tear();

	private:
		int reinforcementTimer;
};

#endif