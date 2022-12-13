#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_set>

using namespace std;

int main()
{
    int priority = 0;
    string items;
    unordered_set<char> itemset = unordered_set<char>();
    ifstream infile("input.txt");

    if (infile.is_open())
    {
        while(getline(infile, items))
        {
            int mid = items.length() /2;
            for (int i = 0; i < mid; i++)
            {
                itemset.insert(items[i]);
            }
            for (int i = mid; i < items.length(); i++)
            {
                if (itemset.count(items[i]) > 0)
                {
                    // found the bad item
                    if (isupper(items[i]))
                    {
                        priority += items[i]-38;
                    }
                    else
                    {
                        priority += items[i]-96;
                    }
                    itemset.clear();
                    break;
                }
            }

        }
        infile.close();
    }
    cout << priority <<endl;
    return 0;
}