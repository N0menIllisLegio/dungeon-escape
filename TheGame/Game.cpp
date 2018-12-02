#include "Game.h"

Game::Game(HWND window, shared_ptr<Level> level, int FOV)
{
	currentLevel = level;
	player = new Player(0, 0, BlockType::Trouper, true);
	gfx = new Graphics(window, currentLevel);
	this->FOV = FOV;
}

Game::~Game()
{	
	delete gfx;
	delete player;
}

void Game::DisperseFog(int X, int Y)
{
	int startY = Y + 1;
	int endY = Y - 1;

	for (int x = X - FOV; x < X + (FOV * 2 + 1); x++)
	{
		if (x <= X)
		{
			startY--;
			endY++;
		}
		else
		{
			startY++;
			endY--;
		}

		for (int y = startY; y <= endY; y++)
		{
			if ((x >= 0 && x < currentLevel->COLUMNS) &&
				(y >= 0 && y < currentLevel->ROWS - 1))
			{
				currentLevel->fogMap[y][x] = currentLevel->blocks[y][x];
			}
		}
	}
}

void Game::DisplayLevel() 
{
	DisperseFog((int)player->x / gfx->BLOCK_WIDTH, (int)player->y / gfx->BLOCK_WIDTH);
    
	gfx->BeginGraphics();
	gfx->DrawLevel(currentLevel);
	gfx->DrawHud(currentLevel);
	gfx->DrawBitmap(gfx->bitmapDictionary.at(BlockType::Trouper), (int)player->x, (int)player->y);
	gfx->DrawString("Steps: " + to_string(steps), RGB(102, 0, 255), 
		gfx->BLOCK_WIDTH * (currentLevel->COLUMNS / 2) - gfx->BLOCK_WIDTH, 
		gfx->BLOCK_HEIGHT * (currentLevel->ROWS - 1));
	gfx->EndGraphics();
}

void Game::Start() 
{
	for (int i = 0; i < currentLevel->ROWS - 1; i++)
	{
		fill(currentLevel->fogMap[i].begin(), currentLevel->fogMap[i].end(), BlockType::Fog);
	}

	DisperseFog(currentLevel->exitPos[1], currentLevel->exitPos[0]);

	player->y = currentLevel->startPos[0] * gfx->BLOCK_HEIGHT;
	player->x = currentLevel->startPos[1] * gfx->BLOCK_WIDTH;
    steps = 0;

	startTime = chrono::high_resolution_clock::now();
	
	DisplayLevel();
}

void Game::MovePlayer(Command command) 
{
    int x = (int)player->x / gfx->BLOCK_WIDTH;
    int y = (int)player->y / gfx->BLOCK_HEIGHT;
	int shiftX = 0;
	int shiftY = 0;

    if (command == Command::MoveNorth)
	{
		shiftY = -1;
    }

    if (command == Command::MoveEast) 
	{
		shiftX = 1;
    }

    if (command == Command::MoveSouth)
	{
		shiftY = 1;
    }

    if (command == Command::MoveWest) 
	{
		shiftX = - 1;
    }

	if ((shiftX + x >= 0 && shiftX + x < currentLevel->COLUMNS) && 
		(shiftY + y >= 0 && shiftY + y < currentLevel->ROWS) &&
		(currentLevel->blocks[shiftY + y][shiftX + x] != BlockType::Unwalkable))
	{
		player->x += gfx->BLOCK_HEIGHT * shiftX;
		player->y += gfx->BLOCK_HEIGHT * shiftY;
		steps++;
	}
}

bool Game::isWin() 
{
    int x = (int)player->x / gfx->BLOCK_WIDTH;
    int y = (int)player->y / gfx->BLOCK_HEIGHT;
	auto block = currentLevel->blocks[y][x];

    if (block == BlockType::Finish) 
	{
		string finishText = "You found the exit in " + to_string(steps) + " steps!\n";
		finishText += "Time spent: ";
		finishText += to_string(chrono::duration_cast<std::chrono::seconds>
			(chrono::high_resolution_clock::now() - startTime).count()) + " sec\n";
        finishText += "Would you like to play again?";

		
        if (gfx->ShowMessage(finishText))
		{
			return true;
        }
        else 
		{
            exit(1);
        }
    }

	return false;
}