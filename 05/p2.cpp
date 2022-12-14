#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    int result = 0;
    vector<deque<char> > cargo = vector<deque<char> >();
    int nstacks;
    if (infile.is_open())
    {
        // initialise the stacks
        getline(infile, line);
        std::stringstream(line) >> nstacks;
        for (int i = 0; i < nstacks; i++)
        {
            cargo.push_back(deque<char>());
            getline(infile, line);
            for (char c: line)
            {
                cargo[i].push_back(c);
            }
        }

        // perform the operations
        // use another stack for moving the crates this time
        int qty, from, to;
        deque<char> moving = deque<char>();
        while (getline(infile, line))
        {
            // Code here
            std::stringstream tmp;
            tmp << line;
            tmp >> qty;
            tmp >> from;
            tmp >> to;
            from--; to--;
            for (int i = 0; i < qty; i++)
            {
                moving.push_back(cargo[from].back());
                cargo[from].pop_back();
            }
            while(moving.size())
            {
                cargo[to].push_back(moving.back());
                moving.pop_back();
            }

        }
        infile.close();
    }
    for (int i = 0; i < nstacks; i++)
    {
        printf("%c", cargo[i].back());
    }
    printf("\n");
    return 0;
}