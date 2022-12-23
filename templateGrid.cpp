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
    int row = 0;
    int col= 0;
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
                if ((!dr && !dc) || !checkBoundsOfCoords(row+dr, col+dc))continue;
                if (!onlyOrth || abs(dr+dc) <= 1)
                {
                    res.push_back(grid[row+dr][col+dc]);
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

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    Grid grid;
    pair<int,int> startCoords, endCoords;
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
                grid.add(Cell(row, col, line[col]));
            }
            row++;
        }
        infile.close();
    }

    printf("%d\n", result);
    return 0;
}
