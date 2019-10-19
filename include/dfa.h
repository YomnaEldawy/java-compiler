#ifndef DFA_H
#define DFA_H
#include <vector>
#include <nfa.h>
#include <dfa_state.h>
#include <algorithm>
#include <stack>

using namespace std;

const char EPSILON = '^';
const int DEAD_STATE_INDEX = -1;
class dfa
{
public:
    nfa NFA;
    vector<dfa_state> transition_table;
    vector<char> alphabet;
    vector<vector<int> > nfa_starting_state;
    vector<int> accepted_states;
    vector<vector<int> > epsilon_closures_table;
    dfa_state start_state;
    vector<int> non_accepted_states;
    dfa(){
    }
    dfa(nfa NFA)
    {
        nfa_starting_state = NFA.get_state(NFA.starting_state);
        this->NFA = NFA;
        this->alphabet = NFA.inputs;
        vector<int> corresponding_states;
        corresponding_states.push_back(NFA.starting_state);
        start_state.corresponding_nfa_states = corresponding_states;
        transition_table.push_back(start_state);
        build_dfa();
    }

    void build_dfa()
    {
        create_start_state();
        for (int i = 0; i < transition_table.size(); i++)
        {
            dfa_state current_state = transition_table[i];
            vector<int> next_states;
            for (int j = 0; j < alphabet.size(); j++)
            {
                if (alphabet[j] == EPSILON){
                    continue;
                }
                dfa_state next_state; // next state under input[j]
                for (int k = 0; k < current_state.corresponding_nfa_states.size(); k++)
                {
                    vector<int> v = NFA.get_next_states(current_state.corresponding_nfa_states[k], alphabet[j]);
                    for (int l = 0; l < v.size(); l++){
                        next_state.insert_corresponding_state(v[l]);
                    }
                }
                if (next_state.corresponding_nfa_states.size() == 0){
                    next_states.push_back(DEAD_STATE_INDEX);

                }else{
                    for (int k = 0; k < next_state.corresponding_nfa_states.size(); k++){
                        vector<int> epsilon_closure = epsilon_closures_table[next_state.corresponding_nfa_states[k]];
                        for (int l = 0; l < epsilon_closure.size(); l++){
                            next_state.insert_corresponding_state(epsilon_closure[l]);
                        }
                    }
                    int state_index = insert_state(next_state);
                    next_states.push_back(state_index);
                }

            }

            current_state.next_state = next_states;
            transition_table[i] = current_state;
        }
        specify_acceptance_states();
    }

    void create_start_state()
    {
        build_epsilon_closures_table();
        vector<int> next_states;
        vector<int> corresponding_states = epsilon_closures_table[NFA.starting_state];
        start_state = dfa_state(corresponding_states, next_states);
        transition_table[0] = start_state;
    }

    void build_epsilon_closures_table(){
        for (int i = 0; i < NFA.transition_table.size(); i++){
            vector<int> epsilon_closure = get_epsilon_closure(i);
            epsilon_closures_table.push_back(epsilon_closure);
        }
    }
    /*
        checks if a dfa_state exists in the transition table
        if it doesn't exist, insert it
        returns the index of the state (whether it was previously inserted or it is recently inserted)
    */
    int insert_state(dfa_state dfaState)
    {
        for (int i = 0; i < transition_table.size(); i++)
        {
            sort(transition_table[i].corresponding_nfa_states.begin(), transition_table[i].corresponding_nfa_states.end());
            sort(dfaState.corresponding_nfa_states.begin(), dfaState.corresponding_nfa_states.end());
            if (transition_table[i].corresponding_nfa_states == dfaState.corresponding_nfa_states)
                return i;
        }
        transition_table.push_back(dfaState);
        return (transition_table.size() - 1);
    }
    void print_dfa(){
        for (int i = 0; i < transition_table.size(); i++){
            cout << i << ")   ";
            transition_table[i].print_state();
        }
        cout << "Acceptance States: " << endl;
        for (int i = 0; i < accepted_states.size(); i++)
            cout << accepted_states[i] << " ";
    }
    void specify_acceptance_states(){
        for (int i = 0; i < transition_table.size(); i++){
            vector<int> current_corresponding_states = transition_table[i].corresponding_nfa_states;
            int index = accepted_state_index(current_corresponding_states, NFA.accepted_states);
            if (index != -1){
                accepted_states.push_back(i);
                transition_table[i].accepted = true;
                transition_table[i].state_name = NFA.accepted_states_names[index];
            }else{
                non_accepted_states.push_back(i);
            }
        }
    }
    int accepted_state_index(vector<int> v1, vector<int> v2){
        int index = -1;
        for (int i = 0; i < v1.size(); i++){
            for (int j = 0; j < v2.size(); j++)
                if (v1[i] == v2[j]){
                    if (index == -1){
                        index = j;
                    }else{
                        index = min(index, j);
                    }
                }
        }
        return index;
    }

    vector<int> get_epsilon_closure(int state_index){
        vector<int> epsilon_closure;
        vector<bool> visited (NFA.transition_table.size());
        stack<int> states_to_visit;
        states_to_visit.push(state_index);
        visited[state_index] = true;
        while ( !states_to_visit.empty() ){
            int current_state = states_to_visit.top();
            states_to_visit.pop();
            epsilon_closure.push_back(current_state);
            vector<int> current_epsilon_closure = NFA.get_next_states(current_state, EPSILON);
            for (int i = 0; i < current_epsilon_closure.size(); i++){
                if (!visited[current_epsilon_closure[i]]){
                    states_to_visit.push(current_epsilon_closure[i]);
                    visited[current_epsilon_closure[i]] = true;
                }
            }
        }
        return epsilon_closure;
    }

    void print_epsilon_closure(int state_index){
        vector<int> epsilon_closure = get_epsilon_closure(state_index);
        for (int i = 0; i < epsilon_closure.size(); i++){
            cout << epsilon_closure[i] << " ";
        }
    }
};

#endif // DFA_H
