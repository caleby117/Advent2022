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
    deque<pair<int,int> > insts = deque<pair<int,int> >();
    int result = 0;

    if (infile.is_open())
    {
        int l = 1;
        while (getline(infile, line))
        {
            // Code here
            if (line.compare("noop")==0)
            {
                insts.push_back(pair<int,int>(1,0));
            }
            else
            {
                stringstream ss = stringstream(line);
                string tmp;
                getline(ss, tmp, ' ');
                getline(ss, tmp);
                insts.push_back(pair<int,int>(2,stoi(tmp,nullptr,10)));
            }
            l++;
        }
        infile.close();
    }

    printf("%lu\n", insts.size());
    int regx = 1;
    int clock = 1;
    int last = 1;
    while(clock < 221)
    {
        if (clock-last == insts.front().first)
        {
            regx += insts.front().second;
            insts.pop_front();
            last = clock;
            //printf("cycle %d - regx = %d\n",clock, regx);
        }
        if ((clock-20)%40==0)
        {
            result += regx*clock;
        }

        // advance the clock and assume next instruction loaded
        clock++;
    }

    printf("%d\n", result);
    return 0;
}
