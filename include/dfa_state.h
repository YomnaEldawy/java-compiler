#ifndef DFA_STATE_H
#define DFA_STATE_H
#include <vector>
#include <iostream>

using namespace std;

class dfa_state
{
public:
    vector<int> corresponding_nfa_states;
    vector<int> next_state;
    bool accepted = false;
    string state_name = "unaccepted";
    dfa_state()
    {
    };
    dfa_state(vector<int> corresponding_states, vector<int> next_states)
    {
        corresponding_nfa_states = corresponding_states;
        next_state = next_states;
    }
    void insert_corresponding_state(int state)
    {
        for (int i = 0; i < corresponding_nfa_states.size(); i++)
        {
            if (corresponding_nfa_states[i] == state){
                return;
            }
        }
        corresponding_nfa_states.push_back(state);
    }
    void print_state(){
        for (int i = 0; i < corresponding_nfa_states.size(); i++){
            cout << corresponding_nfa_states[i] << " ";
        }
        cout << " | ";
        for (int i = 0; i < next_state.size(); i++){
            cout << next_state[i] << " ";
        }
        if (accepted){
            cout << "   " << state_name;
        }
        cout << endl;
    }
};


#endif // DFA_STATE_H
