#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>
#include <nfa.h>
#include <dfa_state.h>
#include <dfa.h>
#include <dfa_instance.h>
#include <minimization.h>
#include <output.h>
#include <grammar.h>

#include <fstream>
using namespace std;

void print_vector(vector<string> v){
    ofstream outFile("D:\\Old downloads\\Last version of compiler\\output.txt");
    for(int i=0;i<v.size();i++){
        outFile<<v[i]<<"   ";
    }
    outFile<<endl;
}
nfa NFA;
void readFile(string filePath){
    ifstream file(filePath);
    string str;
    while (getline(file, str))
    {
        cout<<str<<endl;
        NFA.parseStr(str);
    }
    NFA.handleAll();
}

int main()
{
    //Phase 1
    readFile("D:\\Old downloads\\Last version of compiler\\rules.txt");
    dfa DFA = dfa(NFA);
    dfa_instance DFA_INSTANCE;
    DFA_INSTANCE.DFA = DFA;
    string s;
    ifstream f("D:\\Old downloads\\Last version of compiler\\code.txt");
    ofstream outFile("D:\\Old downloads\\Last version of compiler\\output.txt");
    while (f >> s){
        vector<string> tokens = DFA_INSTANCE.split_to_tokens(s);
        for(int i=0;i<tokens.size();i++){
            outFile<<tokens[i]<<"   ";
        }
        outFile<<endl;
	}
	minimization Min = minimization(DFA);
    Min.partioning();

    //Phase 2
    grammar g;
    ifstream myfile("D:\\Old downloads\\Last version of compiler\\grammar.txt");
    g.generate_grammar(myfile);
    //g.left_factor();
    //g.eliminate_left_recursion();
    g.call_first();
    g.call_follow();
    vector<string> tokens;
    string ss;
    g.generate_predictive_table();
    ifstream input_file("D:\\Old downloads\\Last version of compiler\\output1.txt");
    while(input_file>> ss){
        tokens.push_back(ss);
    }
    output o;
    o.generate_output(&g,tokens);
    g.print_terminals();
    for(int i=0;i<g.predictive_table.size();i++){
        cout<<g.non_terminals[i].value<<" ";
        for(int j=0;j<g.predictive_table[i].size();j++){
            cout<<g.predictive_table[i][j]<<" ";
        }
        cout<<endl;
    }

    return 0;
}


