#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;

class Node
{
public:
string id;
char jobtype;
long val;
string child1ID="";
string child2ID="";
Node* child1=NULL;
Node* child2=NULL;
Node* parent = NULL;
bool isRoot = false;
bool isUnknownAncestor = false;
Node(string name, string job)
{
    id = name;
    if (id.compare("root")==0)
    {
        isRoot = true;
    }
    if (isdigit(job[0]))
    {
        // parse job type int
        jobtype = 'i';
        val = stol(job, nullptr, 10);
    }
    else
    {
        // parse job
        stringstream ss = stringstream(job);
        getline(ss,child1ID, ' ');
        string op;
        getline(ss,op, ' ');
        jobtype = op[0];
        getline(ss,child2ID, ' ');
    }
}
};


long findUnknown(Node* node, long val);
void dfsSetParents(Node* node);
long dfsEval(Node* root);
/*
    Create a binary parse tree for all the operations and monkeys
*/

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    long result = 0;
    unordered_map<string, Node*> graphNodes = unordered_map<string, Node*>();

    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            // Code here
            string ops = line.substr(6, line.length()-1);
            string name = line.substr(0,4);
            graphNodes.insert({name, new Node(name, ops)});
        }
        infile.close();
    }
    else 
    {
        printf("Error opening file\n");
        return 1;
    }

    // Create connections between nodes
    printf("Creating connections\n");
    for (auto kvpair : graphNodes)
    {
        string name = kvpair.first;
        Node* node = kvpair.second;
        if (node->jobtype == 'i') continue;
        node->child1 = graphNodes[node->child1ID];
        node->child2 = graphNodes[node->child2ID];
    }

    Node* root = graphNodes["root"];
    Node* humn = graphNodes["humn"];
    Node* curr= root;
    dfsSetParents(root);
    printf("p1 eval left = %ld\n", dfsEval(root->child1));
    printf("p1 eval right = %ld\n", dfsEval(root->child2));
    curr = humn;
    humn->val = 0;
    do
    {
        curr->isUnknownAncestor = true;
        curr = curr->parent;
    } while (curr != root);
    
    long valueOfKnown, valOfUnknown;
    
    // find value of subtree without unknown
    if (root->child1->isUnknownAncestor)
    {
        valueOfKnown = dfsEval(root->child2);
        printf("Known side : %ld\n", valueOfKnown);
        valOfUnknown = findUnknown(root->child1, valueOfKnown);
    }
    else
    {
        valueOfKnown = dfsEval(root->child1);
        printf("Known side : %ld\n", valueOfKnown);
        valOfUnknown = findUnknown(root->child2, valueOfKnown);
    }


    humn->val = valOfUnknown;
    long left = dfsEval(root->child1);
    long right = dfsEval(root->child2);
    printf("%ld L R %ld\n", left, right);

    printf("%ld\n", __LONG_MAX__);
    printf("%ld\n", valOfUnknown);
    return 0;
}

long findUnknown(Node* node, long val)
{
    if (node->id.compare("humn")==0) return val;
    // Backtrack downwards and find the value that corresponds to val
    long valueOfKnown;
    Node* unknownSubtree = node->child1->isUnknownAncestor ? node->child1 : node->child2;
    Node* knownSubtree = node->child1->isUnknownAncestor ? node->child2 : node->child1;
    
    // find value of subtree without unknown
    valueOfKnown = dfsEval(knownSubtree);

    printf("id: %s - ? %c %ld = %ld\n", node->id.c_str(), node->jobtype, valueOfKnown, val);
    if (node->jobtype == '*') 
        return findUnknown(unknownSubtree, val/valueOfKnown) ;
    if (node->jobtype == '+') 
        return findUnknown(unknownSubtree, val-valueOfKnown);
    if (node->jobtype == '/')
    {
        if (unknownSubtree == node->child1)
        {
            return findUnknown(unknownSubtree, val*valueOfKnown);
        }
        else return findUnknown(unknownSubtree, valueOfKnown/val);

    }
    if (node->jobtype == '-')
    {
        if (unknownSubtree == node->child1)
        {
            return findUnknown(unknownSubtree, val+valueOfKnown);
        }
        else return findUnknown(unknownSubtree, valueOfKnown-val);
    }
    else printf("bad op\n");
}

void dfsSetParents(Node* node)
{
    if (node->jobtype == 'i') return;
    node->child1->parent = node;
    node->child2->parent = node;
    dfsSetParents(node->child1);
    dfsSetParents(node->child2);
}

long dfsEval(Node* root)
{
    /*
        Evaluate the expression through the tree using DFS
        if the root type is immediate, return the value
        else dfs left tree, dfs right tree and eval. change type to immediate
    */
    if (root == NULL)
    {
        printf("Something went wrong - root is null\n");
        return -1;
    }

    if (root->jobtype == 'i')
    {
        return root->val;
    }
    long left = dfsEval(root->child1);
    long right = dfsEval(root->child2);
    if (root->jobtype == '*') return left * right;
    if (root->jobtype == '+') return left + right;
    if (root->jobtype == '/') return left / right;
    if (root->jobtype == '-') return left - right;
    printf("Bad op: %c\n", root->jobtype);
    return __INT_MAX__;
}