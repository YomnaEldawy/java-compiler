#ifndef SYMBOL_H
#define SYMBOL_H
#include <iostream>
#include <vector>

using namespace std;

class symbol
{
    public:
        //symbol();
        string value;
        bool is_terminal;
        vector<string> first;
        vector<string> follow;
        vector<int> first_production;
        bool finish_first = false;
    protected:
    private:
};

#endif // SYMBOL_H
