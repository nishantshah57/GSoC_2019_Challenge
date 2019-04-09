#include <iostream>
#include <cstring>
#include <stdio.h>
#include <vector>
#include <queue>

using namespace std;


bool DEBUG = true;

int ROWS = 0;
int COLS = 0;

// To store cell cooridinates
struct Point
{
	int x;
	int y;
};

// A Data structure for queue used in finding path
struct queueNode
{
	Point pt; // The cooridnates of a cell
	int dist; // cell's distance from the source
};

// Check whether given cell is a valid cell or not
bool isValid(int row, int col)
{
	// return true if row number and column number is in range
	return (row >= 0) && (row < ROWS) && (col >= 0) && (col < COLS);
}

// Arrays are used to get the row and column numbers of 4 neighbours of a given cell
int rowNUM[] = {-1, 0, 0, 1};
int colNUM[] = {0, -1, 1, 0};

// Assuming labyrinth's size is unknown, the function finds out
// number of rows and columns while reading the Input file 
// containing the labyrinth and return the labyrinth contained in a 2D vector.
vector<vector<char> >  readFile(const char* inputFilename, const char* outputFilename, int& rows, int& columns)
{
	// cell will store each char in the file and store in 2D vector 
	char cell;

	// Opens the input file to read labyrinth
	FILE* fp = fopen(inputFilename, "r+");
	// FILE* fp_out = fopen(outputFilename, "w");
	// int columns = 0, rows = 0;

	// To store each char read from the file
	std::vector<char> maze_data;

	// Read the file till cell reached the end of file
	while (EOF != (cell = fgetc(fp))) 
	{

		// cell = fgetc(fp);
		if (cell == '\n')
			rows++;
		else
		{
			if (rows == 0)
				columns++;
		}
		maze_data.push_back(cell);
	}
	rows++;

	// Close the input file
	fclose(fp);	
	if (DEBUG)
		cout << "Rows: " << rows << ", Columns: " << columns << '\n';

	// Convert the 1D vector to 2D Maze to easily visualize the labyrinth
	vector<vector<char> > labyrinth_data(rows, vector<char>(columns, 0));
	int length = 0;
	for (int i = 0; i < rows; i++)
	{
		for(int j = 0; j < columns; j++)
		{
			labyrinth_data[i][j] = maze_data[length++];	
		}
		length++;
	}

	return labyrinth_data;
}

queue<queueNode> findPath(vector<vector<char> > mat, Point src)
{
	// Check source cell has '.'
	// if (mat[src.x][src.y] != '.')
	// 	break;

	bool visited[ROWS][COLS];
	memset(visited, false, sizeof visited);

	visited[src.x][src.y] = true;

	// Queue for BFS
	queue<queueNode> q;
	queue<queueNode> path;	

	// Distance of source cell is 0
	queueNode s = {src, 0};
	q.push(s);
	path.push(s);
	// Do BFS starting from source cell
	// untill queue is empty
	int curr_dist = -1;
	while(!q.empty())
	{
		queueNode curr = q.front();
		Point pt = curr.pt;

		q.pop();

		// Only Vertical and Horizontal Movements
		int row = pt.x;
		int col = pt.y;
		for(int i = 0; i < 4; i++)
		{
			// Move in x-direction
			row = pt.x + rowNUM[i];
			// If adjacent cell is valid, has path and not visited
			// yet, enqueue it.
			if(isValid(row, col) && mat[row][col] == '.' && !visited[row][col])
			{
				// Mark cell as visited and enqueue it
				visited[row][col] = true;
				queueNode Adjcell = {{row, col}, curr.dist + 1};
				q.push(Adjcell);
				path.push(Adjcell);
				break;
			}

			//Move in y-direction
			col = pt.y + colNUM[i];
			// If adjacent cell is valid, has path and not visited
			// yet, enqueue it.
			if(isValid(row, col) && mat[row][col] == '.' && !visited[row][col])
			{
				// Mark cell as visited and enqueue it
				visited[row][col] = true;
				queueNode Adjcell = {{row, col}, curr.dist + 1};
				q.push(Adjcell);
				path.push(Adjcell);
				break;
			}
			
		}
	}
	// cout << "Distance: " << path.back().dist << "\n";
	return path;
}

// A copy of labyrinth is taken as an input just to keep an original
// copy of labyrinth to compare the authenticity of labyrinth.
// This function will find the largest pathway available in the maze.
queue<queueNode> findEntrance(vector<vector<char> > maze)
{
	int entrances = 0;
	int prev_dist = -1, curr_dist = -1;
	queue<queueNode> prev_path;
	queue<queueNode> curr_path;
	for (int i = 0; i < maze.size(); i++)
	{
		for(int j = 0; j < maze[i].size(); j++)
		{
			if (j == 0 || i == 0 || j == COLS-1 || i == ROWS-1)
			{
				if (maze[i][j] == '.')
				{
					entrances++;
					Point source = {i, j};
					curr_path = findPath(maze, source);
					curr_dist = curr_path.back().dist;

					if (curr_dist > prev_dist)
					{
						cout << "Path: " << curr_dist << "\n"; 
						prev_dist = curr_dist;
						prev_path = curr_path;
					}
				}
			}
		}
	}
	if (entrances == 0)
	{
		cout << "No path available for the agent to enter from outside\n" << "Only checking the boundray walls\n";					
	}


	// if (DEBUG)
	// {
	// 	cout << "The maximum Path distance is: " << prev_path.back().dist << "\n";
	//  cout << "Number of Entrances in Labyrinth: " << entrances << "\n";
	// }
	cout << "Last Path Dist: " << prev_path.back().dist << "\n";
	return prev_path;
}

void updateOutFile(const char* outputFilename, vector<vector<char> > labyrinth, queue<queueNode> optimal_path)
{
	FILE* fp_out = fopen(outputFilename, "w");
	cout << "Maximum Pathway length: " << optimal_path.back().dist << "\n";
	fprintf(fp_out, "%d \n", optimal_path.back().dist);
	while(!optimal_path.empty())
		{
			// int curr_dist = curr_path.front().dist;
			// cout << "Size of dist: " << sizeof(curr_path.front().dist);
			labyrinth[optimal_path.front().pt.x][optimal_path.front().pt.y] = (char)(48 + optimal_path.front().dist);
			optimal_path.pop();
		}

	cout << "\nThe Output labyrinth with maximum pathway is: \n ";
	for(int i = 0; i < labyrinth.size(); i++)
	{
		for(int j = 0; j < labyrinth[i].size(); j++)
		{
			fprintf(fp_out, "%c", labyrinth[i][j]);
			cout << labyrinth[i][j];
		}
		if (i < labyrinth.size()-1)
		{
			fprintf(fp_out, "\n");
			cout << "\n";
		}
	}
	fclose(fp_out);

}

int main(int argc, char** argv)
{
	const char* inFile = "";
	const char* outFile = "";
	if (argc == 3)
	{
		inFile = argv[1];
		outFile = argv[2];
	}
	else
	{
		cout << "Usage: ./cppfile InputFile OutputFile\n" << "Please mention Valid Input File and Output File name\n";
	}

	vector<vector<char> > labyrinth = readFile(inFile, outFile,ROWS, COLS);

	if (DEBUG)
	{
		for(int i = 0; i < labyrinth.size(); i++)
		{
			for(int j = 0; j < labyrinth[i].size(); j++)
			{
				cout << labyrinth[i][j];
			}
			if (i < labyrinth.size()-1)
				cout << "\n";
		}
	}
	
	// ROWS and COLS are getting updated
	// cout << "\n" << ROWS << "," <<  COLS << endl;
	queue<queueNode> optimal_path = findEntrance(labyrinth);
	updateOutFile(outFile, labyrinth, optimal_path);

	return 0;
}