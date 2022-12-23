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
    int dir = 0;
    pair<int, pair<int,int>> adjEdgeLinkLROut; // which cell to go to going in that direction
    pair<int, pair<int,int>> adjEdgeLinkUDOut; // which cell to go to going in that direction

    // Coming from another cell which direction does it map to here
    int adjEdgeLinkLRInDir;
    int adjEdgeLinkUDInDir;

    Cell() = default;

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
        // key:coord,direction, value:nextcoord, direction

        // populate the cubemap
        // A-D
        for (int i = 0; i < 50; i++)
        {
            pair<int,int> ACoord = pair<int,int>(i, 50);
            pair<int,int> DCoord = pair<int,int>(149-i,0);
            int Adir = 2;
            int Ddir = 0;

            Cell &Acell = grid[ACoord.first][ACoord.second];
            Cell &Dcell = grid[DCoord.first][DCoord.second];
            Acell.adjEdgeLinkLRInDir = 0;
            Dcell.adjEdgeLinkLRInDir = 0;

            Acell.adjEdgeLinkLROut = pair<int,pair<int,int> >(2, Dcell.coords);
            Dcell.adjEdgeLinkLROut = pair<int, pair<int,int> >(2, Acell.coords);
        }

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

    Cell move(pair<int,int> start, int dist, int dir)
    {
        int mH = 0;
        int mV = 0;
        switch (dir)
        {
            case 0:
                // RIGHT
                mH = dist;
                break;
            case 1:
                // DOWN
                mV = dist;
                break;
            case 2:
                //LEFT
                mH = -dist;
                break;
            case 3:
                // UP
                mV = -dist;
                break;
        }
        if (mH==0&&mV==0)
        {
            printf("Error - mH and mV are 0\n");
            return Cell();
        }
        pair<int,int> movement = pair<int,int>(mH, mV);
        pair<int,int> check = pair<int,int>(start.first, start.second);
        // move in the direction until
        // - hit a boundary (wrap around)
        // - hit a " " (wrap around)
        // - hit a "#" (stop)
        // oh goodness now we need to fold the map gah.
        if (dir%2 == 0)
        {
            // right and left movement
            int change;
            if (dir == 0) change = 1;
            else change = -1;
            for (int i = 1; i <= abs(mH); i++)
            {
                printf("At %d,%d\n", check.first, check.second);
                printf("i = %d, mH = %d\n", i, mH);
                int curr_col = check.second;
                check.second += change;
            
                // out of bounds case
                if (!checkBoundsOfCoords(check))
                {
                    // out of bounds to the rihgt
                    if (check.second < 0) check.second += cols;
                    else check.second = check.second%cols;
                }

                // wrap around and fast forward to the next wall or space
                while(at(check).val==' ')
                {
                    check.second += change;
                    if (!checkBoundsOfCoords(check))
                    {
                        if (check.second < 0) check.second += cols;
                        else check.second = check.second%cols;
                    }
                }
                if (at(check).val == '#') 
                {
                    check.second = curr_col;
                    break;
                }
                printf("After %d,%d\n", check.first, check.second);
            }
        }
        else
        {
            // up and down movement
            int change;
            if (dir == 1) change = 1;
            else change = -1;
            for (int i = 1; i <= abs(mV); i++)
            {
                printf("At %d,%d\n", check.first, check.second);
                printf("i = %d, mV = %d\n", i, mV);
                int curr_row = check.first;
                check.first += change;
            
                // out of bounds case
                if (!checkBoundsOfCoords(check))
                {
                    // out of bounds to the rihgt
                    if (check.first < 0) check.first += rows;
                    else check.first = check.first%rows;
                }

                // wrap around and fast forward to the next wall or space
                while(at(check).val==' ')
                {
                    check.first += change;
                    if (!checkBoundsOfCoords(check))
                    {
                        if (check.first < 0) check.first += rows;
                        else check.first = check.first%rows;
                    }
                }
                if (at(check).val == '#') 
                {
                    check.first = curr_row;
                    break;
                }
                printf("After %d,%d\n", check.first, check.second);
            }
        }
        return at(check);
    }
};

pair<int,int> getDimensionsOfGrid(ifstream& fs)
{
    string tmp;
    int nrows=0, ncols=0;
    while(getline(fs, tmp))
    {
        nrows++;
        if (tmp.length() > ncols) ncols = tmp.length();
    }
    fs.clear();
    fs.seekg(0,fs.beg);
    return pair<int,int>(nrows, ncols);
}

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    ifstream dirfile(argv[2]);
    string directions;
    if (dirfile.is_open()) getline(dirfile, directions);
    printf("%s\n", directions.c_str());
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
            while (line.length() < dims.second)
            {
                line += ' ';
            }
            for (int col = 0; col < line.length(); col++)
            {
                // create cell and add to grid
                grid.add(Cell(row, col, line[col]));
            }
            row++;
        }
        infile.close();
    }

    // search for coords of the starting position
    for (Cell c: grid.grid[0])
    {
        if (c.val=='.')
        {
            startCoords = c.coords;
            break;
        }
    }


    // parse the input directions
    vector<string> instructions = vector<string>();
    string tmp = "";
    for (char c: directions)
    {
        if (isdigit(c)) tmp += c;
        else 
        {
            instructions.push_back(tmp);
            tmp = "";
            tmp += c;
            instructions.push_back(tmp);
            tmp = "";
        }
    }
    instructions.push_back(tmp);

    // simulate movement
    // DIRECTIONS: Right - 0, Down - 1, Left - 2, UP - 3
    int curDirection = 0;

    printf("Start: %d, %d\n", startCoords.first, startCoords.second);
    Cell end = grid.at(startCoords);
    for(int i = 0; i < instructions.size(); i++)
    {
        if (i%2==0)
        {
            // move specified number of steps
            int dist = stoi(instructions[i], nullptr, 10);
            end = grid.move(end.coords, dist, curDirection);
        }
        else
        {
            // rotate
            char dir = instructions[i][0];
            switch (dir)
            {
                case 'L':
                    curDirection -= 1;
                    if (curDirection<0) curDirection = 3;
                    break;
                case 'R':
                    curDirection += 1;
                    if (curDirection>3) curDirection = 0;
                    break;
            }
        }


    }
    printf("End: %d,%d\n", end.row, end.col);
    result = (1000*(end.row+1)) + (4*(end.col+1)) + curDirection;
    printf("%d\n", result);
    return 0;
}
