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
    deque<long long> items = deque<long long>();
    char op;
    string operand;
    int divisor;
    int condTrue;
    int condFalse;
    int inspects = 0;
};


void process_monkey(vector<Monkey> &monkeys, int i, int mod);

int main (int argc, char* argv[])
{
    std::string line;
    std::ifstream infile(argv[1]);
    vector<Monkey> monkeys = vector<Monkey>();
    long result = 0;

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
                curr.items.push_back(stoll(itemstr, NULL, 10));
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

    // find mod
    int mod = 1;
    for (auto monkey: monkeys)
    {
        mod *= monkey.divisor;
    }

    int nrounds = 10000;
    for (int n = 0; n < nrounds; n++)
    {
        if ((n)%1000==0)
        {
            printf("n= %d\n", n);
            for(auto monkey:monkeys)
            {
                printf("%d\n", monkey.inspects);
            }
            printf("\n");
        }
        for (int i = 0; i < monkeys.size(); i++)
        {
            // process monkey
            //printf("\nMonkey %d\n", i);
            process_monkey(monkeys, i, mod);
        }
    }

    deque<long> stack = deque<long>();
    //printf("done with throwing, calculating activity\n");
    for (int i = 0; i < monkeys.size(); i++)
    {
        int ninspects = monkeys[i].inspects;
        printf("%d\n", ninspects);
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

    printf("%ld\n", result);
    return 0;
}

void process_monkey(vector<Monkey> &monkeys, int i, int mod)
{
    Monkey& monkey = monkeys[i];
    // inspect starting items, add to inspected
    deque<long long> &items = monkey.items;
    monkey.inspects += items.size();
    //printf("monkey inspects:%d\n", monkey.inspects);
    
    while(items.size())
    {
        // inspect
        long long item = items.front()%mod;
        items.pop_front();
        long val;
        if (monkey.operand.compare("old") == 0)
        {
            if (item > monkey.divisor)
            {
                val = item;
            }
            else val = item;
        }
        else val = stoll(monkey.operand, NULL, 10);
        val = val%mod;
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
        //item /= 3;
        //printf("after: %ld\n", item);
        //printf("divisor: %d\n", monkey.divisor);
        item = item%mod;
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
