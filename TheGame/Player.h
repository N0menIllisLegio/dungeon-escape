#ifndef PLAYER_H
#define PLAYER_H

#include "BlockType.h"

class Player
{
public:
    Player(double x, double y, BlockType block, bool isDrawable);
    ~Player();

	double x;
	double y;

	BlockType resource;
	bool IsDrawable;
private:

};
#endif