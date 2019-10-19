#ifndef DFA_INSTANCE_H
#define DFA_INSTANCE_H
#include <vector>
#include <dfa.h>
#include <dfa_state.h>
#include <stack>

using namespace std;
class dfa_instance{
public:
    dfa_instance(){

    }
    dfa DFA;
    int NON_EXISTING_ALPHABET_ERROR_CODE = -2;
    vector<string> split_to_tokens(string s){
        vector<string> tokens;
        int current_state = 0;
        int last_accepted_index = 0;
        int last_accepted_state = 0;
        if (s.size() > 0){
            last_accepted_state = get_next_state(0, s[0]);
        }
        for (int i = 0; i < s.size(); i++)
        {

            int next_state = get_next_state(current_state, s[i]);
            current_state = next_state;
            if (current_state == DEAD_STATE_INDEX || current_state == NON_EXISTING_ALPHABET_ERROR_CODE)
            {
                if (last_accepted_state != DEAD_STATE_INDEX && last_accepted_state != NON_EXISTING_ALPHABET_ERROR_CODE)
                    tokens.push_back(DFA.transition_table[last_accepted_state].state_name);
                else
                    tokens.push_back ("unaccepted");
               // cout << endl << "pushing at index" << last_accepted_index << endl;
                i = last_accepted_index;
                last_accepted_index++;
                current_state = 0;
                last_accepted_state = get_next_state(0, s[last_accepted_index]);
              //  cout << "stepping back" << endl;
                continue;
            }

            if (DFA.transition_table[current_state].accepted)
            {
                last_accepted_index = i;
                last_accepted_state = current_state;
            }

          //  cout << "(" << i << "), input= " << s[i] <<" current_state= " << current_state << ", lastAcceptedIndex= " << last_accepted_index << ", lasAcceptedState= " << last_accepted_state << endl;
        }
            tokens.push_back(DFA.transition_table[current_state].state_name);
        // cout << "current state is " << current_state << endl;
        return tokens;
    }

    int get_next_state(int current_state, char input){
        vector<char> alphabet = DFA.alphabet;
        for (int i = 0; i < alphabet.size(); i++){
            if (alphabet[i] == input){
                int next_state = DFA.transition_table[current_state].next_state[i];
                return next_state;
            }
        }
        return NON_EXISTING_ALPHABET_ERROR_CODE;
    }

};
#endif // DFA_INSTANCE_H
