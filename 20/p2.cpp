#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;

class CLLNode
{
    long val;
    int id;
    int shift;
    CLLNode* next;
    CLLNode* prev;

public:

CLLNode(int ID, long value)
{
    id = ID;
    val = value;
}

CLLNode(int ID, long value, CLLNode* previous)
{
    id = ID;
    val = value;
    prev = previous;
}

long getVal(){ return val; } 
int getID() { return id; }
CLLNode* getNext() { return next; }
void setNext(CLLNode* n) {next = n;}
CLLNode* getPrev() { return prev; }
void setPrev(CLLNode* n) {prev = n;}
int getShift() {return shift;}
void setShift(int n) {shift = n;}
void calcShift(int size)
{
    shift = val%size;
}


void printNode()
{
    printf("id:%d  v:%ld\n", id, val);
}

/*
    0     1 .      2 .       3
      --> .  --> .   -->>
      <--    <--     <---
*/

void swopNext()
{
    CLLNode* oldnext = next;
    CLLNode* oldprev = prev;
    next = oldnext->getNext();
    next->setPrev(this);
    oldnext->setPrev(oldprev);
    oldprev->setNext(oldnext);
    oldnext->setNext(this);
    prev = oldnext;
}

void swopPrev()
{
    CLLNode* oldprev = prev;
    CLLNode* oldnext = next;
    prev = oldprev->getPrev();
    prev->next = this;
    oldnext->setPrev(oldprev);
    oldprev->setNext(oldnext);
    next = oldprev;
    oldprev->setPrev(this);
}

};

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    vector<long> inputs = vector<long>();
    int KEY = 811589153;

    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            //printf("%s\n", line.c_str());
            inputs.push_back(stol(line,nullptr, 10)*KEY);
        }
        infile.close();
    }

    vector<CLLNode*> nodes = vector<CLLNode*>();
    int zeroi;
    for (int i = 0; i < inputs.size(); i++)
    {
        //printf("%ld\n", inputs[i]);
        if (inputs[i]==0)
        {
            zeroi = i;
        }
        if (i == 0)
        {
            CLLNode* curr = new CLLNode(i, inputs[i]);
            nodes.push_back(curr);
        }
        else
        {
            CLLNode* curr = new CLLNode(i, inputs[i], nodes[i-1]);
            nodes.push_back(curr);
        }
    }

    printf("Nodes created\n");

    nodes[0]->setPrev(nodes[nodes.size()-1]);

    printf("Setting links between nodes\n");
    for (int i = 0; i < inputs.size(); i++)
    {
        int inext = (i+1)%inputs.size();
        nodes[i]->setNext(nodes[inext]);
        nodes[i]->calcShift(inputs.size()-1);
    }

    CLLNode* zero = nodes[zeroi];
    CLLNode* start = nodes[0];
    for (int n = 0; n < 10; n++)
    {
        for (int i = 0; i < nodes.size(); i++)
        {
            //printf("\n");
            int shift = nodes[i]->getShift();
            //nodes[i]->printNode();
            //printf("shift: %d\n", shift);
            if (shift < 0)
            {
                for (int j = shift; j < 0; j++)
                {
                    nodes[i]->swopPrev();
                }
            }
            else if(shift > 0)
            {
                for (int j = 0; j < shift; j++)
                {
                    nodes[i]->swopNext();
                }
            }
            /*
            CLLNode* toPrint = start;
            toPrint->printNode();
            toPrint = toPrint->getNext();
            while(toPrint != start)
            {
                toPrint->printNode();
                toPrint = toPrint->getNext();
            }
            */
        }
    }
    // Get the number of shifts from 0 to find relevant val
    int mod1k = 1000 % nodes.size();
    int mod2k = 2000 % nodes.size();
    int mod3k = 3000 % nodes.size();
    int mods[3] = {mod1k, mod2k, mod3k};
    /*
    // bubble sort the input cos its fast
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (mods[j] > mods[j+1])
            {
                int tmp = mods[j+1];
                mods[j+1] = mods[j];
                mods[j] = tmp;
            }
        }
    }
    */

    for (int i = 0; i < 3; i++)
    {
        printf("%d ", mods[i]);
    }
    printf("\n");

    CLLNode* curr = zero;
    printf("Zero's id: %d\n", zero->getID());
    long result = 0;
    for (int i = 1; i <= 3000; i++)
    {
        curr = curr->getNext();
        if (i%1000 == 0)
        {
            printf("%ld\n", curr->getVal());
            result += curr->getVal();
        }
    }

    printf("%ld\n", result);
    return 0;
}
