#ifndef PRODUCTION_H
#define PRODUCTION_H
#include <iostream>
#include <vector>
#include <symbol.h>

using namespace std;

class production
{
    public:
        symbol LHS;
        vector<symbol> RHS;
        //production();
        void print_production(){
            cout << LHS.value << endl << " ::= " << endl;
            for (unsigned int i = 0; i < RHS.size(); i++){
                cout << RHS[i].value << ", " << RHS[i].is_terminal << endl;
            }
            cout << endl;
        }
    protected:
    private:
};

#endif // PRODUCTION_H
