#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
    int cals = 0;
    int curr = 0;
    int maxcals = 0;
    string amt;
    ifstream infile("input");

    if (infile.is_open())
    {
        while(getline(infile, amt))
        {
            if (amt.compare("")==0)
            {
                // compare this to maxcals
                if (cals > maxcals)
                {
                    maxcals = cals;
                }
                cals = 0;
                continue;
            }
            stringstream(amt)>>curr;
            cals += curr;
        }
        infile.close();
        if (cals > maxcals)
        {
            maxcals = cals;
        }
    }
    cout << maxcals << endl;

    return 0;
}