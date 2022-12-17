#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <stdlib.h>

using namespace std;

struct GridSquare
{
    bool visitedByTail;
    int row;
    int col;
    GridSquare(int r, int c)
    {
        row = r;
        col = c;
        visitedByTail = false;
    }
};


bool tailIsNearHead(pair<int,int>& head, pair<int,int>& tail);
void moveTail(pair<int,int>&head, pair<int,int>&tail);
void printGrid(vector<vector<GridSquare> >&grid, pair<int,int>&head, pair<int,int>&tail);


int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    int result = 0;
    vector<pair<int,int> > movements = vector<pair<int, int> > ();
    pair<int, int> vRange = pair<int,int>(0,0);
    pair<int,int> hRange = pair<int,int>(0,0);
    int r = 0, c = 0;

    if (infile.is_open())
    {
        while (getline(infile, line, ' '))
        {
            // Code here
            // Get the next instruction
            char dir = line[0];
            int dist;
            getline(infile, line);
            dist = stoi(line, nullptr, 10);
            //printf("%c %d\n", dir, dist);
            switch (dir)
            {
                case 'U':
                    r -= dist;
                    if (r < vRange.first) vRange.first = r;
                    for (int i =0 ; i < dist; i++)
                    {
                        movements.push_back(pair<int,int>(-1,0));
                    }
                    break;
                
                case 'D':
                    r += dist;
                    if (r > vRange.second) vRange.second = r;
                    for (int i =0 ; i < dist; i++)
                    {
                        movements.push_back(pair<int,int>(1,0));
                    }
                    break;

                case 'L':
                    c -= dist;
                    if (c < hRange.first) hRange.first = c;
                    for (int i =0 ; i < dist; i++)
                    {
                        movements.push_back(pair<int,int>(0,-1));
                    }
                    break;
                
                case 'R':
                    c += dist;
                    if (c > hRange.second) hRange.second = c;
                    for (int i =0 ; i < dist; i++)
                    {
                        movements.push_back(pair<int,int>(0,1));
                    }
                    break;
            }

        }
        infile.close();
    }

    // offset params to create a suitable sized grid
    int hOffset = -hRange.first;
    int vOffset = -vRange.first;
    printf("hO = %d, vO = %d\n", hOffset, vOffset);

    // start will be offset such that it moves about the grid without going out of bounds.
    pair<int,int> start = pair<int,int>(vOffset, hOffset);
    hRange.first = 0;
    hRange.second += hOffset;
    vRange.first = 0;
    vRange.second += vOffset;

    // Initialise the grid
    vector<vector<struct GridSquare> > grid = vector<vector<struct GridSquare> >(vRange.second+1, vector<struct GridSquare>());
    for (int r = 0; r <= vRange.second; r++)
    {
        for(int c = 0; c <= hRange.second; c++)
        {
            grid[r].push_back(GridSquare(r,c));
        }
    }

    // Simulate movement
    printf("Start: (%d, %d)\n", start.first, start.second);
    pair<int,int> rope[10];
    for (int i = 0; i < 10; i++)
    {
        rope[i] = pair<int,int>(start.first,start.second);
    }
    pair<int,int> &head = rope[0];
    pair<int,int> &tail = rope[9];
    grid[tail.first][tail.second].visitedByTail = true;
    for (int i = 0; i < movements.size(); i++)
    {
        // Since we're only moving in orthogonal directions
        // can in/decrement both r and c at the same time
        head.first += movements[i].first;
        head.second += movements[i].second;
        //printf("Move head (%d, %d) to (%d, %d)\n", movements[i].first, movements[i].second, head.first, head.second);
        
        // Simulate movement of the rope based on the previous one
        for (int i = 0; i < 9; i++)
        {
            if (!tailIsNearHead(rope[i],rope[i+1]))
            {
                moveTail(rope[i], rope[i+1]);
                //printf("Move tail to (%d, %d)\n", tail.first, tail.second);
                if (i==8)
                {
                    grid[tail.first][tail.second].visitedByTail = true;
                }
            }
        }
        //printGrid(grid, head, tail);
    }

    for (int r = 0; r < grid.size(); r++)
    {
        for (int c = 0; c < grid[r].size(); c++)
        {
            GridSquare curr = grid[r][c];
            //printf("%d", curr.visitedByTail);
            if (curr.visitedByTail)
            {
                result++;
            }
        }
        //printf("\n");
    }
    printf("%d\n", result);
    return 0;
}

bool tailIsNearHead(pair<int,int>& head, pair<int,int>& tail)
{
    return abs(head.first-tail.first) <= 1 && abs(head.second-tail.second) <= 1;
}


void moveTail(pair<int,int>&head, pair<int,int>&tail)
{
    if (head.first > tail.first)
    {
        //head below tail
        if (head.second > tail.second)
        {
            // head to the right of tail. move down and right
            tail.first++;
            tail.second++;
        }
        else if (head.second < tail.second)
        {
            // head to left of tail. move down and left
            tail.first++;
            tail.second--;
        }
        else
        {
            tail.first++;
        }
    }
    else if (head.first < tail.first)
    {
        // head above tail
        if (head.second > tail.second)
        {
            // head to the right of tail. move up and right
            tail.first--;
            tail.second++;
        }
        else if (head.second < tail.second)
        {
            // head to left of tail. move up and left
            tail.first--;
            tail.second--;
        }
        else
        {
            tail.first--;
        }
    }
    else 
    {
        // head same level as tail
        if (head.second > tail.second)
        {
            tail.second++;
        }
        else
        {
            tail.second--;
        }
    }
}

void printGrid(vector<vector<GridSquare> >&grid, pair<int,int>&head, pair<int,int>&tail)
{
    for (int r = 0; r <grid.size(); r++)
    {
        for (int c = 0; c < grid[r].size(); c++)
        {
            if (head.first==r && head.second==c)
            {
                printf("H");
            }
            else if (tail.first==r&&tail.second==c)
            {
                printf("T");
            }
            else if (grid[r][c].visitedByTail)
            {
                printf("#");
            }
            else
            {
                printf(".");
            }
        }
        printf("\n");
    }
}