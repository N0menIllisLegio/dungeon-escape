#ifndef GAME_H
#define GAME_H

#include <Windows.h>
#include <vector>
#include <chrono>

#include "BlockType.h"
#include "Command.h"
#include "Graphics.h"
#include "Player.h"
#include "Level.h"

using namespace std;

class Game 
{
public:
    Game(HWND window, shared_ptr<Level> level, int FOV);
    ~Game();

	void DisplayLevel();
    void Start();
    void MovePlayer(Command command);
    bool isWin();

private:    
	int steps;
	int FOV;

	void DisperseFog(int X, int Y);

	chrono::time_point<chrono::high_resolution_clock> startTime;
	shared_ptr<Level> currentLevel;
	Player *player;
	Graphics *gfx;
};

#endif