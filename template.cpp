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

    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            // Code here
        }
        infile.close();
    }
    else 
    {
        printf("Error opening file\n");
        return 1;
    }
    printf("%d", result);
    return 0;
}
