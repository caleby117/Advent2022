#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <numeric>

using namespace std;

deque<int> cal_stack = deque<int>();

void check_stack(int amount);
int main()
{
    int cals = 0;
    int curr = 0;
    int maxcals = 0;
    string amt;
    ifstream infile("input.txt");

    if (infile.is_open())
    {
        while(getline(infile, amt))
        {
            if (amt.compare("")==0)
            {
                //check stack
                check_stack(cals);
                cals = 0;
                continue;
            }
            stringstream(amt)>>curr;
            cals += curr;
        }
        infile.close();
        check_stack(cals);
    }
    cout << accumulate(cal_stack.begin(), cal_stack.begin()+3, 0) << endl;

    return 0;
}

void check_stack(int amount)
{
    int stack_size = cal_stack.size();
    cout<< "examine " << amount << endl;
    cout << "stack size " << stack_size << endl;
    for (int i = 0; i < stack_size; i++)
    {
        cout << cal_stack[i] << ' ';
    }
    cout << endl;
    if (stack_size == 0)
    {
        cal_stack.push_back(amount);
        return;
    }
    if (stack_size < 3)
    {
        //definitely going in, just compare
        if (amount > cal_stack.front())
        {
            cal_stack.push_front(amount);
        }
        else
        {
            cal_stack.push_back(amount);
        }
        return;
    }

    deque<int> aux = deque<int>();
    while (cal_stack.size() > 0 && amount > cal_stack.back())
    {
        aux.push_back(cal_stack.back());
        cal_stack.pop_back();
    }

    if (cal_stack.size() == 3) return;
    cal_stack.push_back(amount);
    while (cal_stack.size() < 3)
    {
        cal_stack.push_back(aux.back());
        aux.pop_back();
    }
}