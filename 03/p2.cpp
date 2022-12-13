#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <cstdio>

using namespace std;
// priority for lowercase is c - 96, uppercase c - 38

int main(int argc, char* argv[])
{
    int priority = 0;
    string filename = argv[1];
    string items;
    vector<unordered_set<char> > itemsets = vector<unordered_set<char> >();
    for (int i = 0; i < 2; i++)
    {
        itemsets.push_back(unordered_set<char>());
    }
    ifstream infile(filename);
    int elf = 0;

    if (infile.is_open())
    {
        while(getline(infile, items))
        {
            if (elf%3==2)
            {
                // just check all the chars against the others
                for (char c: items)
                {
                    if (itemsets[0].count(c) && itemsets[1].count(c))
                    {
                        // found
                        printf("Found common item %c\n", c);
                        if (isupper(c))
                        {
                            priority += c - 38;
                        }
                        else
                        {
                            priority += c - 96;
                        }
                        break;
                    }
                }
                for (int i = 0; i < itemsets.size(); i++)
                {
                    itemsets[i].clear();
                }
                elf = 0;
            }
            else
            {
                for (char c : items)
                {
                    itemsets[elf].insert(c);
                }
                elf++;
            }

        }
        infile.close();
    }
    printf("Priority %d\n", priority);
    return 0;
}
