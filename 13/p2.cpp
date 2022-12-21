#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <iostream>

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

string extractFirst(string list);
int compareLists(string left, string right);
string extractRemaining(string list);

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    vector<string> packets = vector<string>();
    int result = 1;

    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            if (line.length() == 0) continue;
            // Code here
            packets.push_back(line.substr(1,line.length()-2));
        }
        infile.close();
    }

    packets.push_back("[2]");
    packets.push_back("[6]");

    // Quick and dirty bubble sort implementation
    for (int i = packets.size(); i > 0; i--)
    {
        for (int j = 0; j < packets.size()-1; j++)
        {
            if (compareLists(packets[j], packets[j+1]) < 0)
            {
                string tmp = packets[j];
                packets[j] = packets[j+1];
                packets[j+1] = tmp;
                //printf("cmp %s and %s\n", packets[j].c_str(), packets[j+1].c_str());
            }
        }
    }


    for (int i = 0; i < packets.size(); i++)
    {
        if(packets[i] == "[2]"|| packets[i]=="[6]")
        {
            result*=i+1;
            if (packets[i] == "[6]") break;
        }

    }

    printf("%d\n", result);
    return 0;
}

int compareLists(string left, string right)
{
    // Takes in a list that is debracketed at the top level.
    // 2, 3, 4, [5,6], 7,8 form.
    //printf("Comparing\nL: %s\nR: %s\n", left.c_str(), right.c_str());
    if (left.length()==0) return right.length();
    else if (right.length()==0) return -1;

    // parse first element
    string firstLeft = extractFirst(left);
    string firstRight = extractFirst(right);
    
    // check if element is a list - if it starts with '[' get the whole list as a single element
    bool leftIsList = firstLeft[0]=='[';
    bool rightIsList = firstRight[0]=='[';
    string cmpLeft;
    string cmpRight;
    if (leftIsList)
    {
        cmpLeft = left.substr(1, firstLeft.length()-2);
    }
    if (rightIsList)
    {
        cmpRight = right.substr(1, firstRight.length()-2);
    }

    //printf("firstLeft: %s\n", firstLeft.c_str());
    //printf("firstRight: %s\n", firstRight.c_str());
    int cmpFirstsResult;
    if (leftIsList && rightIsList)
    {
        //printf("Both lists. Next layer\n");
        cmpFirstsResult = compareLists(cmpLeft, cmpRight);
    }
    else if (leftIsList)
    {
        //printf("Only left is list. Next layer\n");
        cmpRight = firstRight;
        cmpFirstsResult = compareLists(cmpLeft, cmpRight);
    }
    else if (rightIsList)
    {
        //printf("Only right is list. Next layer\n");
        cmpLeft = firstLeft;
        cmpFirstsResult = compareLists(cmpLeft, cmpRight);
    }
    else
    {
        //printf("Both ints\n");
        int L = stoi(firstLeft, nullptr, 10);
        int R = stoi(firstRight, nullptr, 10);
        cmpFirstsResult = R - L;
    }

    if (cmpFirstsResult == 0)
    {
        //printf("Extract Remaining\n");
        string rLeft = extractRemaining(left);
        string rRight = extractRemaining(right);
        return compareLists(rLeft, rRight);
    }
    else return cmpFirstsResult;
}

string extractRemaining(string list)
{
    string first = extractFirst(list);
    int idx = first.length();
    if (idx >= list.length()) return "";
    string res = list.substr(idx+1, list.length());
    //printf("Remaining of %s: %s\n", list.c_str(), res.c_str());
    return res;
}

string extractFirst(string list)
{
    //printf("Extracting first from %s\n", list.c_str());
    string result;
    if (list.length()==0) return list;

    if (list[0]=='[')
    {
        // extract the list/nested list as one element
        int bLevel = 0;
        int endIndex = 0;
        for (; endIndex < list.length(); endIndex++)
        {
            //printf("%c ", list[endIndex]);
            if (list[endIndex]=='[')
            {
                bLevel++;
            }
            else if (list[endIndex]==']')
            {
                bLevel--;
            }
            if (bLevel==0) break;
        }
        result = list.substr(0,endIndex+1);
    }
    else
    {
        int ind = 0;
        for (; ind < list.length(); ind++)
        {
            if (list[ind]==',')
            {
                break;
            }
        }
        result = list.substr(0, ind);
    }
    //printf("Result from extraction of %s: %s\n", list.c_str(), result.c_str());
    return result;
}