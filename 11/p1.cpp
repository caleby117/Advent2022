#include <cstdio>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <deque>

using namespace std;

struct Monkey
{
    deque<int> items = deque<int>();
    char op;
    string operand;
    int divisor;
    int condTrue;
    int condFalse;
    int inspects = 0;
};


void process_monkey(vector<Monkey> &monkeys, deque<int>& items, int i);

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    vector<Monkey> monkeys = vector<Monkey>();
    int result = 0;

    if (infile.is_open())
    {
        while (getline(infile, line))
        {
            //printf("%s\n", line.c_str());
            // Get the monkeys' information
            // Get starting items
            getline(infile, line);
            stringstream ss;
            string token;
            Monkey curr;

            // Starting items:
            token = line.substr(18, line.length());
            //printf("%s\n", token.c_str());
            ss << token;

            // items
            while(getline(ss, token, ' '))
            {
                string itemstr = token.substr(0, token.length());
                curr.items.push_back(stoi(itemstr, NULL, 10));
                //printf("item: %s\n", itemstr.c_str());
            }

            // operation
            getline(infile, line);
            token = line.substr(23, line.length());
            char op = token[0];
            if (op!='+' && op!='*')
            {
                //printf("Error occurred: op is %c\n", op);
                infile.close();
                return 1;
            }
            //printf("op: %c\n", op);
            curr.op = op;
            token = token.substr(2, token.length());
            //printf("operand: %s\n", token.c_str());
            curr.operand = token;

            // test
            getline(infile, line);
            token = line.substr(21, line.length());
            //printf("divisor: %s\n", token.c_str());
            curr.divisor = stoi(token, NULL, 10);

            // if true
            getline(infile,line);
            token = line.substr(29, line.length());
            //printf("iftrue: %s\n", token.c_str());
            curr.condTrue = stoi(token, NULL, 10);

            // if false
            getline(infile, line);
            token = line.substr(30, line.length());
            //printf("iffalse: %s\n", token.c_str());
            curr.condFalse = stoi(token,NULL, 10);
            curr.inspects = 0;
            monkeys.push_back(curr);
            getline(infile, line);
        }
        infile.close();
    }

    // Now that we've parsed the input to monkeys, 
    // go through monkeys one by one to throw the items

    int nrounds = 20;
    for (int n = 0; n < nrounds; n++)
    {
        for (int i = 0; i < monkeys.size(); i++)
        {
            // process monkey
            //printf("\nMonkey %d\n", i);
            process_monkey(monkeys, monkeys[i].items, i);
        }
    }

    deque<int> stack = deque<int>();
    //printf("done with throwing, calculating activity\n");
    for (int i = 0; i < monkeys.size(); i++)
    {
        int ninspects = monkeys[i].inspects;
        //printf("%d\n", ninspects);
        if (stack.size() < 2)
        {
            if (stack.size() == 0 ) stack.push_back(ninspects);
            else if (ninspects < stack.back())
            {
                stack.push_back(ninspects);
            }
            else stack.push_front(ninspects);
        }
        else if (ninspects > stack.front())
        {
            stack.pop_back();
            stack.push_front(ninspects);
        }
        else if (ninspects > stack.back())
        {
            stack.pop_back();
            stack.push_back(ninspects);
        }
    }
    result = stack.front() * stack.back();

    printf("%d\n", result);
    return 0;
}

void process_monkey(vector<Monkey> &monkeys, deque<int>& items, int i)
{
    Monkey& monkey = monkeys[i];
    // inspect starting items, add to inspected
    monkey.inspects += items.size();
    //printf("monkey inspects:%d\n", monkey.inspects);
    
    while(items.size())
    {
        // inspect
        int item = items.front();
        items.pop_front();
        int val;
        if (monkey.operand.compare("old") == 0)
        {
            val = item;
        }
        else val = stoi(monkey.operand, NULL, 10);
        //printf("item: %d, (", item);
        // change worry value
        if (monkey.op == '*')
        {
            //printf("*");
            item *= val;
        }
        else
        {
            //printf("+");
            item += val;
        }
        //printf(" %d) / 3 = %d\n", val, item);
        item /= 3;
        //printf("after: %d\n", item);
        //printf("divisor: %d\n", monkey.divisor);
        if (item%monkey.divisor == 0)
        {
            //printf("true - Throw to monkey %d\n", monkey.condTrue);
            monkeys[monkey.condTrue].items.push_back(item);
        }
        else 
        {
            //printf("false - Throw to monkey %d\n", monkey.condFalse);
            monkeys[monkey.condFalse].items.push_back(item);
        }
    }
}
