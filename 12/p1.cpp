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
        printf("Set prev %d %d\n", prev.row, prev.col);
        grid[c.row][c.col].prev = prev.coords;
    }

    void setVal(pair<int,int> coords, char val)
    {
        grid[coords.first][coords.second].val = val;
    }

    void setVal(Cell c, char val)
    {
        printf("Set val of %d %d to %c\n", c.row, c.col, val);
        grid[c.row][c.col].val = val;
        printf("after val = %c\n", grid[c.row][c.col].val);
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
        printf("neighbors of %d %d %c\n", row, col, grid[row][col].val);
        for(auto c : res)
        {
            printf("%d %d %c, ", c.row, c.col, c.val);
        }
        printf("\n");
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
                if (line[col] == 'S')
                {
                    startCoords = pair<int,int>(row,col);
                }
                else if (line[col] == 'E')
                {
                    endCoords = pair<int,int>(row,col);
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
    grid.setVal(startCoords, 'a');
    grid.setVal(endCoords, 'z');
    printf("startcoords: %d %d\n", startCoords.first, startCoords.second);
    printf("endcoords: %d %d\n", endCoords.first, endCoords.second);
    
    Cell start = grid.at(startCoords);
    Cell end = grid.at(endCoords);
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
            printf("new depth\n");
            depth++;
            f_i = 0;
            f_count = frontier.size();
        }
        printf("\nDepth = %d\n", depth);
        Cell curr = frontier.front();
        printf("Curr: %d %d %c\n", curr.row, curr.col, curr.val);
        frontier.pop_front();
        if (curr.row==endCoords.first&&curr.col==endCoords.second) break;
        f_i++;
        neighbors = grid.getNeighbors(curr, true);
        for (auto cell : neighbors)
        {
            if (cell.visited) continue;
            else if (curr.row == startCoords.first && curr.col == startCoords.second)
            {
                printf("Start: Cell %d %d %c\n", cell.row, cell.col, cell.val);
                grid.setPrev(curr, curr);
                grid.setVisited(curr);
                grid.setVisited(cell);
                grid.setPrev(cell, curr);
                frontier.push_back(cell);
            }
            else if(cell.val - curr.val <= 1)
            {
                printf("Cell %d %d %c added\n", cell.row, cell.col, cell.val);
                grid.setPrev(cell, curr);
                grid.setVisited(cell);
                frontier.push_back(cell);
            }
        }
    }

    // print path
    Cell curr = grid.at(endCoords);
    deque<Cell> path = deque<Cell>();
    printf("start:%d %d\n", startCoords.first, startCoords.second);
    while (curr.row != startCoords.first || curr.col != startCoords.second)
    {
        printf("%d %d %c\n", curr.row, curr.col, curr.val);
        printf("prev: %d %d %c\n", curr.prev.first, curr.prev.second, grid.at(curr.prev).val);
        path.push_back(curr);
        curr = grid.at(curr.prev);
    }

    while(path.size())
    {
        curr = path.back();
        printf("%lu: %d %d %c\n",depth -path.size(), curr.row, curr.col, curr.val);
        path.pop_back();
    }

    grid.printPrev();
    printf("%d\n", result);
    return 0;
}
