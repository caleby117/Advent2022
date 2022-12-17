#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;

struct TreeNode
{
    int row;
    int col;
    int height;
    bool isVisible;
    TreeNode(int r, int c, int h)
    {
        row = r;
        col = c;
        height = h;
        isVisible = false;
    }
};

vector<TreeNode> look(vector<vector<TreeNode> >& forest, char dir);
vector<TreeNode> lookCol(vector<vector<TreeNode> >& forest, char dir);
vector<TreeNode> lookRow(vector<vector<TreeNode> >& forest, char dir);

/*
    use a monotonic stack for each of the directions
*/

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    vector<vector<TreeNode> >forest = vector<vector<TreeNode> >();
    int result = 0;

    if (infile.is_open())
    {
        int row = 0;
        while (getline(infile, line))
        {
            forest.push_back(vector<TreeNode>());
            // Code here
            for (int col = 0; col < line.length(); col++)
            {
                int h = line[col] - '0';
                forest[row].push_back(TreeNode(row, col, h));
            }
            row++;
        }
        infile.close();
    }
    vector<TreeNode> toMark, tmp;
    toMark = vector<TreeNode>();
    tmp = look(forest, 'u');
    toMark.insert(toMark.end(), tmp.begin(), tmp.end());
    tmp = look(forest, 'd');
    toMark.insert(toMark.end(), tmp.begin(), tmp.end());
    tmp = look(forest, 'l');
    toMark.insert(toMark.end(), tmp.begin(), tmp.end());
    tmp = look(forest, 'r');
    toMark.insert(toMark.end(), tmp.begin(), tmp.end());

    for (int i = 0; i < toMark.size(); i++)
    {
        forest[toMark[i].row][toMark[i].col].isVisible = true;
    }

    for (auto r : forest)
    {
        for (auto c : r)
        {
            printf("%d", c.isVisible);
            if (c.isVisible)
            {
                result++;
            }
        }
        printf("\n");
    }

    printf("%d\n", result);
    return 0;
}

vector<TreeNode> look(vector<vector<TreeNode> >& forest, char dir)
{
    if (dir == 'u' || dir == 'd')
    {
        return lookCol(forest, dir);
    }
    else if (dir == 'l' || dir == 'r')
    {
        return lookRow(forest, dir);
    }
}

vector<TreeNode> lookCol(vector<vector<TreeNode> >& forest, char dir)
{
    vector<TreeNode> res = vector<TreeNode>();
    deque<TreeNode> monoStack = deque<TreeNode>();
    for (int c = 0; c < forest[0].size(); c++)
    {
        if (dir == 'd')
        {
            for (int r = forest.size()-1; r > -1; r--)
            {
                //printf("tree height %d\n", forest[r][c].height);
                while (monoStack.size() && forest[r][c].height >= monoStack.back().height)
                {
                    monoStack.pop_back();
                }
                monoStack.push_back(forest[r][c]);
            }
        }
        else
        {
            for (int r = 0; r < forest.size(); r++)
            {
                //printf("tree height %d\n", forest[r][c].height);
                while (monoStack.size() && forest[r][c].height >= monoStack.back().height)
                {
                    monoStack.pop_back();
                }
                monoStack.push_back(forest[r][c]);
            }
        }
        // whatever is left in the monoStack is visible
        for (auto t : monoStack)
        {
            //printf("tree height %d visible \n", t.height);
            res.push_back(t);
        }
        monoStack.clear();
    }
    return res;
}

vector<TreeNode> lookRow(vector<vector<TreeNode> >& forest, char dir)
{
    vector<TreeNode> res = vector<TreeNode>();
    deque<TreeNode> monoStack = deque<TreeNode>();
    for (int r = 0; r < forest.size(); r++)
    {
        if (dir == 'l')
        {
            for (int c = forest[0].size()-1; c > -1; c--)
            {
                //printf("tree height %d\n", forest[r][c].height);
                while (monoStack.size() && forest[r][c].height >= monoStack.back().height)
                {
                    monoStack.pop_back();
                }
                monoStack.push_back(forest[r][c]);
            }
        }
        else
        {
            for (int c = 0; c < forest[r].size(); c++)
            {
                //printf("tree height %d\n", forest[r][c].height);
                while(monoStack.size() && forest[r][c].height>=monoStack.back().height)
                {
                    monoStack.pop_back();
                }
                monoStack.push_back(forest[r][c]);
            }
        }
        for (auto t : monoStack)
        {
            //printf("tree height %d visible \n", t.height);
            res.push_back(t);
        }
        monoStack.clear();
    }
    return res;
}
