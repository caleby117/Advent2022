#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;

void print_vec(vector<T>& vec);
void print_map(unordered_map<T,T>& m);

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    int result = 0;

    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            // Code here
        }
        infile.close();
    }
    printf("%d", result);
    return 0;
}

void print_vec(vector<T>& vec)
{
    for(auto item : vec)
    {
        cout << item << " ";
    }
    cout << endl;
}

void print_map(unordered_map<T,T>& m)
{
    cout << "{";
    for(auto p : m)
    {
        cout << p->first << ": "<<p->second << ", ";
    }
    cout << "}\n";
}