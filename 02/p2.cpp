#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

int main()
{
    unordered_map<char, char> winsAgainst = unordered_map<char,char> ();
    unordered_map<char, char> drawsAgainst = unordered_map<char,char> ();
    unordered_map<char, char> losesAgainst = unordered_map<char,char> ();
    unordered_map<char, int> scoreMap = unordered_map<char, int>();
    winsAgainst['A'] = 'Y';
    winsAgainst['B'] = 'Z';
    winsAgainst['C'] = 'X';
    drawsAgainst['A'] = 'X';
    drawsAgainst['B'] = 'Y';
    drawsAgainst['C'] = 'Z';
    losesAgainst['A'] = 'Z';
    losesAgainst['B'] = 'X';
    losesAgainst['C'] = 'Y';
    scoreMap['X'] = 1;
    scoreMap['Y'] = 2;
    scoreMap['Z'] = 3;
    string line;
    char other;
    char self;
    char result;
    ifstream infile("test.txt");
    int score = 0;
    int n = 0;

    if (infile.is_open())
    {
        while(getline(infile, line))
        {
            n++;
            other = line[0];
            result = line[2];
            //cout << self << ' ' << other << endl;
            //cout << score << endl;
            if (result == 'X')
            {
                // lose
                self = losesAgainst[other];
            }
            else if (result == 'Y')
            {
                // draw
                self = drawsAgainst[other];
                score += 3;
            }
            else
            {
                // win 
                self = winsAgainst[other];
                score += 6;
            }

            score += scoreMap[self];
        }
        infile.close();
    }
    cout << n << endl;
    cout << score << endl;
    return 0;
}