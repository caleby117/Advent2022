#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main (int argc, char* argv[])
{
    string areas;
    ifstream infile(argv[1]);
    vector<int> area_ranges = vector<int>();
    int npairs = 0;

    if (infile.is_open())
    {
        while (getline(infile, areas))
        {
            //printf("%s\n", areas.c_str());
            string currElf;
            stringstream areas_stream;
            areas_stream << areas;
            while(getline(areas_stream, currElf, ','))
            {
                //printf("%s\n", currElf.c_str());
                string rangeID;
                stringstream elfss;
                elfss << currElf;
                while(getline(elfss, rangeID, '-'))
                {
                    //printf("%s\n", rangeID.c_str());
                    area_ranges.push_back(stoi(rangeID));
                }
            }
            //printf("printing vector ");
            /*
            for (auto i : area_ranges)
            {
                printf("%d ", i);
            }
            printf("\n");
            */
            int range1 = area_ranges[1]-area_ranges[0];
            int range2 = area_ranges[3]-area_ranges[2];
            if (range1 > range2)
            {
                if (area_ranges[3]<=area_ranges[1]&&area_ranges[2]>=area_ranges[0])
                {
                    printf("%s\n", areas.c_str());
                    npairs++;
                }
            }
            else
            {
                if (area_ranges[1]<=area_ranges[3]&&area_ranges[0]>=area_ranges[2])
                {
                    npairs++;
                    printf("%s\n", areas.c_str());
                }
            }
            area_ranges.clear();
        }
        infile.close();
    }

    printf("%d\n", npairs);
    return 0;
}