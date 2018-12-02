#include "Level.h"

Level::Level(vector<vector<BlockType>> blocks, int columns, int rows)
{
	this->blocks = blocks;
	this->COLUMNS = columns;
	this->ROWS = rows;

	fogMap.resize(rows - 1);
	for (int i = 0; i < rows - 1; i++)
	{
		fogMap[i].resize(columns);
	}
}

shared_ptr<Level> Level::loadLevel(string filename)
{
	filename += createMaze(10, 10);
	//filename += createMaze(0, 0);

	int exitFlag[2] = { -1, -1 };
	int enterFlag[2] = { -1, -1 };
	int tempCols = 0;
	
	ifstream fileStream(filename);
	string line;

	int cols = 0;
	int rows = 0;
	vector<vector<BlockType>> tiles;

	while (getline(fileStream, line)) 
	{
		vector<BlockType> row;

		for (auto x : line) 
		{
			if (x == '0') 
			{
				row.push_back(BlockType::Walkable);
				continue;
			}
			if (x == '1') 
			{
				row.push_back(BlockType::Unwalkable);
				continue;
			}
			if (x == 's') 
			{
				row.push_back(BlockType::Start);
				enterFlag[0] = rows;
				enterFlag[1] = row.size() - 1;
				continue;
			}
			if (x == 'e') 
			{
				row.push_back(BlockType::Finish);
				exitFlag[0] = rows;
				exitFlag[1] = row.size() - 1;
			}
		}

		tiles.push_back(row);
		cols = line.length();

		if (tempCols == 0)
			tempCols = cols;
		if (cols != tempCols)
			return NULL;

		rows++;
	}

	if (enterFlag[0] == -1 || exitFlag[0] == -1)
	{
		return NULL;
	}


	vector<BlockType> row;
	row.resize(cols);
	fill(row.begin(), row.end(), BlockType::Hud);
	tiles.push_back(row);
	rows++;

	shared_ptr<Level> level = make_shared<Level>(tiles, cols, rows);
	
	level->exitPos[0] = exitFlag[0];
	level->exitPos[1] = exitFlag[1];

	level->startPos[0] = enterFlag[0];
	level->startPos[1] = enterFlag[1];

	return level;
}

bool Level::checkLevel(HWND hwnd, shared_ptr<Level> level)
{
	if (level == NULL)
	{
		string finishText = "Corrupt level data!\n";
		finishText += "Check your level file.";
		if (MessageBox(hwnd, finishText.c_str(), "Error!", MB_OK | MB_ICONERROR) == IDOK)
		{
			return false;
		}
	}
	
	return true;
}
