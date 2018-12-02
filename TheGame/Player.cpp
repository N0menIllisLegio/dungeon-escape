#include "Player.h"

Player::Player(double x, double y, BlockType block, bool isDrawable) 
{
	this->x = x;
	this->y = y;
	this->IsDrawable = isDrawable;
	this->resource = block;
}
Player::~Player() { }
