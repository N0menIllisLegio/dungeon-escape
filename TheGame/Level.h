#ifndef LEVEL_H
#define LEVEL_H

#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>

#include "BlockType.h"
#include "MazeGenerator.h"

using namespace std;

class Game;

class Level
{
public:
    Level(vector<vector<BlockType>> blocks, int width, int height);
	static shared_ptr<Level> loadLevel(string filename);
	static bool checkLevel(HWND hwnd, shared_ptr<Level> level);

	vector<vector<BlockType>> blocks;
	vector<vector<BlockType>> fogMap;
	
    int COLUMNS;
    int ROWS;
	int startPos[2];
	int exitPos[2];
};
#endif