#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>
using namespace std;

// find the first 4-contiguous sequence of unique chars
void print_window(deque<char>& window);
int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    unordered_map<char, int> packets = unordered_map<char, int>();
    deque<char> window = deque<char>();
    int result = 0;

    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            //printf("Checking %s\n", line.c_str());
            // Code here
            int i = 0;
            for (; i < 13; i++)
            {
                if (packets.count(line[i]) == 0)
                {
                    packets[line[i]] = 1;
                }
                else packets[line[i]]++;
                window.push_back(line[i]);
                //printf("push_back %c\n", line[i]);
            }
            for (; i < line.length(); i++)
            {
                //printf("i = %d\n", i);
                window.push_back(line[i]);
                print_window(window);
                if (packets.count(line[i]) == 0)
                {
                    packets[line[i]] = 1;
                }
                else
                { 
                    packets[line[i]]++;
                }
                bool found = true;
                for (auto it = window.begin(); it != window.end(); it++)
                {
                    if (packets[*it] > 1) 
                    {
                        //printf("packets[%c] = %d\n", *it, packets[*it]);
                        found = false;
                        break;
                    }
                }
                if (found)
                {
                    result = i;
                    //printf("%d\n", ++result);
                    break;
                }
                else
                {
                    char to_remove = window.front();
                    window.pop_front();
                    packets[to_remove]--;
                }
            }
            window.clear();
            packets.clear();
        }
        infile.close();
    }
    result++;
    printf("%d\n", result);
    return 0;
}

void print_window(deque<char>& window)
{
    printf("Window: ");
    for (auto it = window.begin(); it != window.end(); it++)
    {
        printf("%c ", *it);
    }
    printf("\n");
}