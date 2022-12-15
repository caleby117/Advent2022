#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
/*
    Parse the input / Build the FS tree (doubly linked):
        - if starts with $, register as command
            - if cd -> traverse
            - if ls -> create child nodes 

    Next, run DFS to find deepest child. 
        - sum all children to get parents filesize.
        - keep going until all file sizes are done
*/
using namespace std;
struct FSNode 
{
    FSNode* parent;
    unordered_map<string, FSNode*> children;
    string label;
    int size;
    FSNode(FSNode* thisparent, string thislabel, int filesize)
    {
        children = unordered_map<string, FSNode*>();
        parent = thisparent;
        size = filesize;
        label = thislabel;
    }
    FSNode(FSNode* thisparent, string thislabel)
    {
        children = unordered_map<string, FSNode*>();
        parent = thisparent;
        size = -1;
        label = thislabel;
    }
};

int dfsFindSize(FSNode* node);
void dfsGetSmallest(FSNode* node, int& target, int& best);

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    int result = 0;

    // Build the FS tree
    // Build root node
    FSNode root = FSNode(NULL, "/");
    FSNode* curr = &root;
    getline(infile,line);
    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            // Code here
            // Command coming up
            printf("%s\n", line.c_str());
            // parse the cmd
            stringstream buf = stringstream(line);
            string tmp;
            vector<string> lineargs = vector<string>();
            while(getline(buf, tmp, ' '))
            {
                lineargs.push_back(tmp);
            }
            if (lineargs[1].compare("cd") == 0)
            {
                printf("%s\n", lineargs[2].c_str());
                if (lineargs[2].compare("..") == 0)
                {
                    printf("goto dir %s\n", curr->parent->label.c_str());
                    curr = curr->parent;
                }
                else
                {
                    curr = curr->children[lineargs[2]];
                    printf("goto dir %s\n", lineargs[2].c_str());
                    if (curr == nullptr)
                    {
                        printf("curr is null somehow\n");
                        return 1;
                    }
                }

            }
            else if (lineargs[1].compare("ls") == 0)
            {
                string res;
                vector<string> finfo = vector<string>();
                stringstream res_str;
                getline(infile, line);
                while (line[0] != '$')
                {
                    printf("%s\n", line.c_str());
                    res_str = stringstream(line);
                    getline(res_str, res, ' ');
                    finfo.push_back(res);
                    getline(res_str, res, ' ');
                    finfo.push_back(res);
                    FSNode* childNode;
                    if (finfo[0].compare("dir") == 0)
                    {
                        printf("Create new dir node for %s\n", finfo[1].c_str());
                        childNode = new FSNode(curr, finfo[1]);
                    }
                    else
                    {
                        printf("Create new file node for %s\n", finfo[1].c_str());
                        int filesize = stoi(finfo[0], nullptr, 10);
                        childNode = new FSNode(curr, finfo[1], filesize);
                    }
                    printf("Stored node\n");
                    curr -> children[finfo[1]] = childNode;
                    finfo.clear();
                    if (infile.peek() != '$' && infile.peek() != EOF) getline(infile, line);
                    else break;
                }
            }
        }
        infile.close();
    }

    // Traverse the tree to get filesizes
    dfsFindSize(&root);
    result = INT_MAX;
    printf("size of root = %d\n", root.size);
    int targetDeleteSize = 30000000 - (70000000 - root.size);
    printf("target = %d\n", targetDeleteSize);
    dfsGetSmallest(&root, targetDeleteSize, result);

    printf("%d\n", result);
    return 0;
}

void dfsGetSmallest(FSNode* node, int& target, int& best)
{
    if (node->size < target)
    {
        return;
    }
    if (node->size < best && node->children.size())
    {
        best = node->size;
    }
    for (auto it = node->children.begin(); it!=node->children.end(); it++)
    {
        dfsGetSmallest(it->second, target, best);
    }
}

int dfsFindSize(FSNode* node)
{
    printf("Exploring node %s\n", node->label.c_str());
    if (!node->children.size())
    {
        printf("%s bottom of tree, size = %d\n", node->label.c_str(), node->size);
        return node->size;
    }
    node->size =0 ;
    for (auto it = node->children.begin(); it != node->children.end(); it++)
    {
        node->size += dfsFindSize(it->second);
    }
    printf("Node %s - size %d\n", node->label.c_str(), node->size);
    return node->size;
}