#include "MazeGenerator.h"

using namespace std;

#define UP    0
#define DOWN  1
#define LEFT  2
#define RIGHT 3

int maze_size[2];
int start_axis;
int start_side;

vector<vector<int>> dfs_path;
vector<vector<vector<bool>>> maze;

bool randomMove(bool first_move)
{
	int random_neighbor;
	vector<vector<int>> unvisited_neighbors;

	for (int direction = 0; direction < 4; direction++)
	{
		int possible_pmd[2] = { 0, 0 };

		if (direction == UP)
		{
			possible_pmd[1] = -1;
		}
		else if (direction == DOWN)
		{
			possible_pmd[1] = 1;
		}
		else if (direction == LEFT)
		{
			possible_pmd[0] = -1;
		}
		else
		{
			possible_pmd[0] = 1;
		}

		if (dfs_path.back()[0] + possible_pmd[0] * 2 > 0 &&
			dfs_path.back()[0] + possible_pmd[0] * 2 < maze_size[0] - 1 &&
			dfs_path.back()[1] + possible_pmd[1] * 2 > 0 &&
			dfs_path.back()[1] + possible_pmd[1] * 2 < maze_size[1] - 1)
		{
			if (!maze[dfs_path.back()[1] + possible_pmd[1] * 2]
				[dfs_path.back()[0] + possible_pmd[0] * 2][1])
			{
				vector<int> possible_move_delta = { possible_pmd[0], possible_pmd[1] };

				unvisited_neighbors.push_back(possible_move_delta);
			}
		}
	}

	if (unvisited_neighbors.size() > 0)
	{
		random_neighbor = rand() % unvisited_neighbors.size();

		for (int a = 0; a < !first_move + 1; a++)
		{
			vector<int> new_location;

			new_location.push_back(dfs_path.back()[0] + unvisited_neighbors[random_neighbor][0]);
			new_location.push_back(dfs_path.back()[1] + unvisited_neighbors[random_neighbor][1]);

			dfs_path.push_back(new_location);

			maze[dfs_path.back()[1]][dfs_path.back()[0]][0] = false;
			maze[dfs_path.back()[1]][dfs_path.back()[0]][1] = true;
		}

		return true;
	}
	else
	{
		return false;
	}
}

void generateMaze()
{
	bool first_move = true;
	bool success = true;

	while ((int)dfs_path.size() > 1 - first_move)
	{
		if (!success)
		{
			dfs_path.pop_back();

			if (!first_move && dfs_path.size() > 2)
			{
				dfs_path.pop_back();
			}
			else
			{
				break;
			}

			success = true;
		}

		while (success)
		{
			success = randomMove(first_move);

			if (first_move) {
				first_move = false;
			}
		}
	}
}

void initializeMaze()
{
	for (int a = 0; a < maze_size[1]; a++)
	{
		for (int b = 0; b < maze_size[0]; b++)
		{
			bool is_border;

			if (a == 0 || a == maze_size[1] - 1 ||
				b == 0 || b == maze_size[0] - 1)
			{
				is_border = true;
			}
			else
			{
				is_border = false;
			}

			vector< bool > new_cell = { true, is_border };

			if ((unsigned int)a + 1 > maze.size())
			{
				vector<vector<bool>> new_row = { new_cell };

				maze.push_back(new_row);
			}
			else
			{
				maze[a].push_back(new_cell);
			}
		}
	}
}

void randomPoint(bool part) {
	int axis;
	int side;

	if (!part)
	{
		axis = rand() % 2;
		side = rand() % 2;

		start_axis = axis;
		start_side = side;
	}
	else
	{
		bool done = false;

		while (!done) {
			axis = rand() % 2;
			side = rand() % 2;

			if (axis != start_axis ||
				side != start_side) {
				done = true;
			}
		}
	}

	vector<int> location = { 0, 0 };

	if (!side)
	{
		location[!axis] = 0;
	}
	else
	{
		location[!axis] = maze_size[!axis] - 1;
	}

	location[axis] = 2 * (rand() % ((maze_size[axis] + 1) / 2 - 2)) + 1;

	if (!part)
	{
		dfs_path.push_back(location);
	}

	maze[location[1]][location[0]][0] = false;
	maze[location[1]][location[0]][1] = true;
}

string createMazeFile(const string directory)
{
	string path = directory + "/GeneratedLevel.txt";
	ofstream myfile(path);
	const char* startEnd = "se";
	int startEndPtr = 0;

	if (myfile.is_open())
	{
		for (unsigned int a = 0; a < maze.size(); a++) {
			for (unsigned int b = 0; b < maze[a].size(); b++) {
				if (maze[a][b][0])
				{
					myfile << "1";
				}
				else
				{
					if (a == 0 || b == 0 || a == maze.size() - 1 || b == maze[a].size() - 1)
					{
						myfile << startEnd[startEndPtr++];
					}
					else
					{
						myfile << "0";
					}
				}
			}

			myfile << endl;
		}

		myfile.close();
	}
	else
	{
		return NULL;
	}

	return path;
}

void clearVars()
{
	dfs_path.clear();
	maze.clear();
}

const string createMaze(int width, int height, const string directory)
{
	srand((unsigned int)time(nullptr));

	if (width < 10 || height < 10)
	{
		maze_size[0] = 20 + rand() % 60;
		maze_size[1] = 20 + rand() % 20;
	}
	else
	{
		maze_size[0] = width;
		maze_size[1] = height;
	}

	for (int a = 0; a < 2; a++)
	{
		if (maze_size[a] % 2 == 0)
		{
			maze_size[a]--;
		}
	}

	initializeMaze();
	randomPoint(false);
	randomPoint(true);
	generateMaze();
	string result = createMazeFile(directory);
	clearVars();

	return result;
}