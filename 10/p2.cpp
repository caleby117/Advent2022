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
    int clock = 0;
    int last = 0;
    while(clock < 240)
    {
        // complete current instruction at the end of the last cycle
        if (clock-last == insts.front().first)
        {
            regx += insts.front().second;
            insts.pop_front();
            last = clock;
            //printf("cycle %d - regx = %d\n",clock, regx);
        }
        
        // draw pixel during current instruction
        int pos = (clock)%40;
        if (abs(pos-regx) <= 1)
        {
            printf("#");
        }
        else printf(".");
        if (pos == 39)
        {
            // Go to next line
            printf("\n");
        }



        // advance the clock and assume next instruction loaded
        clock++;
    }

    printf("%d\n", result);
    return 0;
}
