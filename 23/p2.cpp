#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;

class Elf
{
public:
    pair<int,int>coords;
    pair<int,int>nextmove;
    Elf(){};
    Elf (pair<int,int> coord)
    {
        coords = coord;
    }

    void consider(pair<int,int> next)
    {
        nextmove = next;
    }

    void move()
    {
        //printf("Move elf %d %d to ", coords.first, coords.second);
        coords = pair<int,int>(coords.first+nextmove.first, coords.second+nextmove.second);
        //printf("%d %d\n", coords.first, coords.second);
    }
};

class Cell
{
public:
    pair<int,int> coords;
    char val = '#';
    int row = 0;
    int col= 0;
    bool visited = false;
    pair<int,int> prev;
    Elf firstElf;

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
public:
    int rows;
    int cols;
    vector<vector<Cell> > grid;
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
    vector<Cell> getNeighbors(pair<int,int> coords, bool onlyOrth)
    {
        return getNeighbors(coords.first, coords.second, onlyOrth);
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

    Cell& at(int row, int col)
    {
        return grid[row][col];
    }

    Cell& at(pair<int,int> coords)
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
                printf("%c", grid[r][c].val);
            }
            printf("\n");
        }
    }

    bool checkConditions(pair<int,int> coords, pair<int,int> dir)
    {
        char neighbors[3];
        if (dir.first == -1 && dir.second == 0)
        {
            // check north
            neighbors[0] = at(coords.first-1, coords.second-1).val; // NW
            neighbors[1] = at(coords.first-1, coords.second).val; // N
            neighbors[2] = at(coords.first-1, coords.second+1).val; // NE
        }
        else if(dir.first== 1 && dir.second == 0)
        {
            //check south
            neighbors[0] = at(coords.first+1, coords.second-1).val; // SW
            neighbors[1] = at(coords.first+1, coords.second).val; // S
            neighbors[2] = at(coords.first+1, coords.second+1).val; // SE
        }
        else if (dir.first == 0 && dir.second == -1)
        {
            //check west
            neighbors[0] = at(coords.first-1, coords.second-1).val; // NW
            neighbors[1] = at(coords.first, coords.second-1).val; // W
            neighbors[2] = at(coords.first+1, coords.second-1).val; // SW
        }
        else
        {
            // check east
            neighbors[0] = at(coords.first-1, coords.second+1).val; // NE
            neighbors[1] = at(coords.first, coords.second+1).val; // E
            neighbors[2] = at(coords.first+1, coords.second+1).val; // SE
        }
        for (int i = 0 ; i < 3; i++)
        {
            if (neighbors[i] == '#') return false;
        }
        return true;
    }
};

bool isSame(vector<Elf> prev, vector<Elf> elves);

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
    vector<string> rows = vector<string>();
    int gridSize = 200;
    pair<int,int> startCoords, endCoords;
    int result = 0;
    pair<int,int> dims;

    if (infile.is_open())
    {
        dims = getDimensionsOfGrid(infile);
        grid = Grid(gridSize, gridSize);
        while (getline(infile, line))
        {
            //put rows into the vector
            rows.push_back(line);
        }
        infile.close();
    }
    for (int r = 0; r < gridSize; r++)
    {
        for (int c = 0; c < gridSize; c++)
        {
            grid.grid[r][c] = Cell(r, c, '.');
        }
    }

    vector<Elf> elves = vector<Elf>();

    // centralise the elves in the middle of the 150x150 grid
    int rOffset = (gridSize-dims.first)/2;
    int cOffset = (gridSize-dims.second)/2;

    for (int r = 0; r < rows.size(); r++)
    {
        for (int c = 0; c < rows[r].size(); c++)
        {
            grid.grid[r+rOffset][c+cOffset].val=rows[r][c];
            if (rows[r][c]=='#')
            {
                //printf("Adding elf %d %d\n", r,c);
                elves.push_back(Elf(pair<int,int>(r+rOffset, c+cOffset)));
            }
        }
    }

    // try and move the elves
    pair<int,int> north = pair<int,int>(-1,0);
    pair<int,int> south = pair<int,int>(1,0);
    pair<int,int> east = pair<int,int>(0,1);
    pair<int,int> west = pair<int,int> (0,-1);
    pair<int,int> dirs[4] = {north, south, west, east};

    int rep = -1;
    printf("Init:\n");
    grid.printGrid();

    vector<Elf> prev = vector<Elf>();
    while (!isSame(prev,elves))
    {
        rep++;
        //printf("rep: %d\n", rep);
        //grid.printGrid();
        prev.clear();
        prev.insert(prev.begin(),elves.begin(), elves.end());
        // Have all elves consider movement
        vector<Cell> cellsToUnflag = vector<Cell>();
        for (Elf& e: elves)
        {
            //printf("Considering elf %d,%d\n", e.coords.first, e.coords.second);

            // check if all neighboring cells are empty
            vector<Cell> neighbors = grid.getNeighbors(e.coords, false);
            bool emptyNeighbors = true;
            for (Cell c: neighbors)
            {
                if (c.val == '#')
                {
                    emptyNeighbors = false;
                    break;
                }
            }
            if (emptyNeighbors)
            {
                e.consider(pair<int,int>(0,0));
                continue;
            }

            pair<int,int> consider = dirs[rep%4];
            //printf("Direction: %d %d\n", consider.first, consider.second);
            int i = 0;
            while (!grid.checkConditions(e.coords, consider) && i < 4)
            {
                // if cannot move in that direction, consider the next direction
                i++;
                //printf("Cannot move in direction - choose another direction ");
                consider = dirs[(rep+i)%4];
                //printf("%d %d\n", consider.first, consider.second);
            }
            if (i==4)
            {
                // dont move the elf
                consider = pair<int,int>(0,0);
                e.consider(consider);
                //printf("Surrounded by other elves\n");
                continue;
            }
            Cell& potential = grid.grid[e.coords.first+consider.first][e.coords.second+consider.second];
            if (potential.visited)
            {
                // Alr considered by another elf. Nobody moves.
                // find the elf with the correct coords
                Elf toundo = potential.firstElf;
                for (Elf& elf: elves)
                {
                    if (elf.coords == toundo.coords)
                    {
                        elf.consider(pair<int,int>(0,0));
                        break;
                    }
                }
                e.consider(pair<int,int>(0,0));
                cellsToUnflag.push_back(potential);
                //printf("Clash with anohter cell\n");
                continue;
            }
            e.consider(consider);
            //printf("After checking: direction: %d %d\n", e.nextmove.first, e.nextmove.second);
            potential.visited = true;
            potential.firstElf = e;
        }


        // Second part: Movement
        for (Elf& e : elves)
        {
            grid.grid[e.coords.first][e.coords.second].val = '.';
            //printf("Elf %d,%d moved to ", e.coords.first, e.coords.second);
            e.move();
            //printf("%d,%d\n", e.coords.first, e.coords.second);
            grid.grid[e.coords.first][e.coords.second].val = '#';
            grid.grid[e.coords.first][e.coords.second].visited = false;
            for (auto c: cellsToUnflag)
            {
                grid.grid[c.row][c.col].visited = false;
            }
        }

        //printf("\nAfter round %d\n", rep+1);
        //grid.printGrid();
    }

    printf("After:\n");
    grid.printGrid();
    printf("%d\n", rep+1);
    return 0;
}

bool isSame(vector<Elf> prev, vector<Elf> elves)
{
    if (prev.size() != elves.size()) return false;
    for (int i = 0; i < elves.size(); i++)
    {
        //printf("prev: %d %d, elf: %d %d\n", prev[i].coords.first, prev[i].coords.second, elves[i].coords.first, elves[i].coords.second);
        if (prev[i].coords != elves[i].coords) return false;
    }
    return true;
}
