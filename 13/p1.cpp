#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;


/*
first val = left
second val = right

CORRECT ORDER: LEFT < RIGHT
if left == right move on to compare the next item in both lists
if left and right are both lists:
    - compare first item
    - if same move on to next item
    - correct order means that left < right or left runs out of items
if both ints
    - left should be < right else move on

if one int one list
    - convert the int to list of length 1 and compare.

**** SHOULD RETURN TRUE OR FALSE AT THE FIRST DIFFERENCE **** 

idea:
    - function compareLists(left, right)
        parse out first elem of left and right
        compare left[0] and right[0]
            return according to above conditions
        if comparison is the same 
            - return compareLists(left[1 to end], right[1 to end])

should be straightforward enough la

*/

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    vector<pair<string,string> > packets = vector<pair<string,string> >();
    int result = 0;

    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            // Code here
            pair<string,string> curr = pair<string,string>();
            curr.first = line.substr(1,line.length()-2);
            getline(infile,line);
            curr.second = line.substr(1,line.length()-2);
            packets.push_back(curr);
            getline(infile,line);
        }
        infile.close();
    }
    for (auto p:packets)
    {
        printf("%s\n%s\n", p.first.c_str(), p.second.c_str());
    }
    printf("%d", result);
    return 0;
}

bool compareLists(string left, string right)
{
    // parse first element
    // check if element is a list - if it starts with '[' get the whole list as a single element
    return false;
}
