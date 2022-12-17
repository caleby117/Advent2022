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
    int LR = 0;
    int RL = 0;
    int UD = 0;
    int DU = 0;
    TreeNode(int r, int c, int h)
    {
        row = r;
        col = c;
        height = h;
        isVisible = false;
    };
};

void look(vector<vector<TreeNode> >& forest, char dir);
void lookCol(vector<vector<TreeNode> >& forest, char dir);
void lookRow(vector<vector<TreeNode> >& forest, char dir);

void changeValues(vector<vector<TreeNode> >& f);

/*
    use a monotonic stack for each of the directions
    Looking in a certain direction:
        - eg looking left to right.
        30373
        25512
        65332
        33549
        35390
        
        - create monotonic stack eg row 0
        3: push 3 - 3
        0: push 0 - 3 0
        3: pop 0, measure dist. pop 3, measure dist. push 3 - 3
        7: pop 3, measure dist - 7
        3: push 3.
        end : first item to pop, set ntreesLR to 0. Subsequent items measure distance to edge

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
    /*
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
    */
    look(forest, 'r');
    look(forest, 'l');
    look(forest, 'u');
    look(forest, 'd');
    // test to make the forest alternating ones and zeros
    int maxTrees = 0;
    TreeNode maxNode = TreeNode(-1, -1, -1);
    for (auto r : forest)
    {
        for (auto c : r)
        {
            int prod = c.RL*c.LR*c.UD*c.DU;
            printf("%d", prod);
            if (prod > maxTrees)
            {
                maxTrees = prod;
                maxNode = c;
            }
        }
        printf("\n");
    }

    printf("Max is node %d,%d h=%d, prod=%d\n", maxNode.row, maxNode.col, maxNode.height, maxTrees);
    return 0;
}

void changeValues(vector<vector<TreeNode> >& f)
{
    for (int r = 0; r < f.size(); r++)
    {
        for (int c = 0; c < f[r].size(); c++)
        {
            if ((r+c)%2)
            {
                f[r][c].isVisible = true;
            }
            else
            {
                f[r][c].isVisible = false;
            }
        }
    }
}

void look(vector<vector<TreeNode> >& forest, char dir)
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

void lookCol(vector<vector<TreeNode> >& forest, char dir)
{
    deque<TreeNode*> monoStack = deque<TreeNode*>();
    for (int c = 0; c < forest[0].size(); c++)
    {
        if (dir == 'u')
        {
            for (int r = forest.size()-1; r > -1; r--)
            {
                //printf("tree height %d\n", forest[r][c].height);
                while (monoStack.size() && forest[r][c].height >= monoStack.back()->height)
                {
                    TreeNode* curr = monoStack.back();
                    curr->DU = curr->row - r;
                    monoStack.pop_back();
                }
                monoStack.push_back(&forest[r][c]);
            }
            while (monoStack.size())
            {
                    TreeNode* curr = monoStack.back();
                    curr->DU = curr->row;
                    monoStack.pop_back();
            }
        }
        else
        {
            for (int r = 0; r < forest.size(); r++)
            {
                //printf("tree height %d\n", forest[r][c].height);
                while (monoStack.size() && forest[r][c].height >= monoStack.back()->height)
                {
                    TreeNode* curr = monoStack.back();
                    curr->UD = r - curr->row;
                    monoStack.pop_back();
                }
                monoStack.push_back(&forest[r][c]);
            }
            while (monoStack.size())
            {
                TreeNode* curr = monoStack.back();
                curr->UD = forest.size() - 1 - curr->row;
                monoStack.pop_back();
            }
        }
        monoStack.clear();
    }
}

void lookRow(vector<vector<TreeNode> >& forest, char dir)
{
    deque<TreeNode*> monoStack = deque<TreeNode*>();
    for (int r = 0; r < forest.size(); r++)
    {
        if (dir == 'l')
        {
            for (int c = forest[0].size()-1; c > -1; c--)
            {
                //printf("tree height %d\n", forest[r][c].height);
                while (monoStack.size() && forest[r][c].height >= monoStack.back()->height)
                {
                    TreeNode* curr = monoStack.back();
                    curr->RL = curr->col - c;
                    monoStack.pop_back();
                }
                monoStack.push_back(&forest[r][c]);
            }
            while (monoStack.size())
            {
                TreeNode* curr = monoStack.back();
                curr->RL = curr->col;
                monoStack.pop_back();
            }
        }
        else
        {
            for (int c = 0; c < forest[r].size(); c++)
            {
                //printf("tree height %d\n", forest[r][c].height);
                while(monoStack.size() && forest[r][c].height>=monoStack.back()->height)
                {
                    TreeNode* curr = monoStack.back();
                    curr->LR = c - curr->col;
                    monoStack.pop_back();
                }
                monoStack.push_back(&forest[r][c]);
            }
            while (monoStack.size())
            {
                TreeNode* curr = monoStack.back();
                curr->LR = forest[r].size() - 1 - curr->col;
                monoStack.pop_back();
            }
        }
        monoStack.clear();
    }
}
