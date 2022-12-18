#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;

class Cell
{
public:
    pair<int,int> coords;
    char val = '#';
    int row;
    int col;
    bool visited = false;
    pair<int,int> prev;

    Cell(){};

    Cell(int r, int c, char v)
    {
        row = r;
        col = c;
        val = v;
        coords = pair<int,int>(row,col);
    }
};


class Grid
{
private:
    vector<vector<Cell> > grid;

public:
    int rows;
    int cols;
    Grid(){};
    Grid(int nrows, int ncols)
    {
        grid = vector<vector<Cell> > (nrows, vector<Cell>(ncols, Cell()));
        rows = nrows;
        cols = ncols;

    };

    void setVisited(Cell c)
    {
        grid[c.row][c.col].visited = true;
    }

    void setPrev(Cell c, Cell prev)
    {
        grid[c.row][c.col].prev = prev.coords;
    }

    void setVal(pair<int,int> coords, char val)
    {
        grid[coords.first][coords.second].val = val;
    }

    void setVal(Cell c, char val)
    {
        grid[c.row][c.col].val = val;
    }

    void add(Cell c)
    {
        if (checkBoundsOfCoords(c.coords))
        {
            grid[c.row][c.col] = c;
        }
    }

    bool checkBoundsOfCoords(int row, int col)
    {
        // Checks bounds of coords
        return row >= 0 && row < grid.size() && col >= 0 && col < grid[0].size();
    }

    bool checkBoundsOfCoords(pair<int,int> coord)
    {
        return checkBoundsOfCoords(coord.first, coord.second);
    }

    vector<Cell> getNeighbors(int row, int col, bool onlyOrth)
    {
        vector<Cell> res = vector<Cell>();
        for (int dr = -1; dr <= 1; dr++)
        {
            for (int dc = -1; dc <= 1; dc++)
            {
                if ((dr == 0 && dc==0) || !checkBoundsOfCoords(row+dr, col+dc))continue;
                if (!onlyOrth)
                {
                    res.push_back(grid[row+dr][col+dc]);
                }
                else
                {
                    if (dr==0 || dc==0)
                    {
                        res.push_back(grid[row+dr][col+dc]);
                    }
                }
            }
        }
        return res;
    }

    vector<Cell> getNeighbors(Cell &cell, bool onlyOrth)
    {
        return getNeighbors(cell.row, cell.col, onlyOrth);
    }

    Cell at(int row, int col)
    {
        return grid[row][col];
    }

    Cell at(pair<int,int> coords)
    {
        return grid[coords.first][coords.second];
    }

    void printGrid()
    {
        printf("printing grid %lu by %lu\n", grid.size(), grid[0].size());
        for(int r = 0; r < grid.size(); r++)
        {
            for (int c = 0; c < grid[r].size(); c++)
            {
                printf("%c ", grid[r][c].val);
            }
            printf("\n");
        }
    }
    void printPrev()
    {
        printf("printing grid %lu by %lu\n", grid.size(), grid[0].size());
        for(int r = 0; r < grid.size(); r++)
        {
            for (int c = 0; c < grid[r].size(); c++)
            {
                printf("%d,%d ", grid[r][c].prev.first, grid[r][c].prev.second);
            }
            printf("\n");
        }
    }
};

pair<int,int> getDimensionsOfGrid(ifstream& fs)
{
    string tmp;
    int nrows, ncols;
    fs.seekg(0, fs.end);
    nrows = fs.tellg();
    fs.clear();
    fs.seekg(0, fs.beg);
    getline(fs, tmp);
    ncols = tmp.length();
    nrows = nrows/ncols;
    fs.clear();
    fs.seekg(0,fs.beg);
    return pair<int,int>(nrows, ncols);
}

/*
    Part 2: Find shortest path from any a to E
        - basically just start from E and stop at the first a

    - start from E
    - Change conditions of adding nodes to frontier
    - Change end condition
*/


int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    Grid grid;
    pair<int,int> startCoords, sCoords;
    int result = 0;

    if (infile.is_open())
    {
        pair<int,int> dims = getDimensionsOfGrid(infile);
        grid = Grid(dims.first, dims.second);
        int row = 0;
        while (getline(infile, line))
        {
            // Code here
            for (int col = 0; col < line.length(); col++)
            {
                // create cell and add to grid
                if (line[col] == 'E')
                {
                    startCoords = pair<int,int>(row,col);
                }
                else if (line[col] == 'S')
                {
                    sCoords = pair<int,int>(row,col);
                }
                grid.add(Cell(row, col, line[col]));
            }
            row++;
        }
        infile.close();
    }

    grid.printGrid();

    // Get start cell and end cell
    // perform bfs on the grid and we should be ok
    grid.setVal(startCoords, 'z');
    grid.setVal(sCoords, 'a');
    printf("startcoords: %d %d\n", startCoords.first, startCoords.second);
    
    Cell start = grid.at(startCoords);
    deque<Cell> frontier = deque<Cell>();
    frontier.push_back(start);
    vector<Cell> neighbors;
    bool found = false;
    int depth = 0;
    int f_count = frontier.size();
    int f_i = 0;
    while(frontier.size())
    {
        if (f_i == f_count)
        {
            depth++;
            f_i = 0;
            f_count = frontier.size();
            printf("Depth %d\n", depth);
        }
        Cell curr = frontier.front();
        frontier.pop_front();
        printf("Curr: %d %d %c\n", curr.row, curr.col, curr.val);
        if (curr.val == 'a') break;
        f_i++;
        neighbors = grid.getNeighbors(curr, true);
        for (auto nb : neighbors)
        {
            if (nb.visited) continue;
            else if (curr.row == startCoords.first && curr.col == startCoords.second)
            {
                grid.setPrev(curr, curr);
                grid.setVisited(curr);
            }
            if(curr.val - nb.val <= 1)
            {
                grid.setPrev(nb, curr);
                grid.setVisited(nb);
                frontier.push_back(nb);
            }
        }
    }

    result = depth;
    printf("%d\n", result);
    return 0;
}
