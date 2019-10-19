#ifndef GRAMMAR_H
#define GRAMMAR_H
#include <production.h>
#include <symbol.h>
#include <fstream>
#include <map>
#include <stack>
using namespace std;

class grammar
{
public:

    vector<production> productions_list;
    vector<symbol> terminals;
    vector<symbol> non_terminals;
    stack<vector<symbol> > rhs_productions_stack;
    map<string, bool> existing_terminals, existing_non_terminals;
    map<string, int> extra_states;
    int new_productions_count = 0;
    string EPSILON="\\L";
    /**
        predictive table: row i corresponds to non_terminals[i]
                          column j corresponds to terminal[j]
                          predictive_table[i][j] is an integer containing the index of the production in productions_list
    **/
    vector<vector<int> > predictive_table;

    /**
    ** reads input file
    ** generates the productions and pushes them to productions_list, creates a list of symbols, terminals and nonterminals
    ** Task goes for: Yomna
    **/
    void generate_grammar(ifstream& infile)
    {
        string s;
        bool start = true;
        production currentProduction;
        symbol lhsSymbol;
        while (infile >> s)
        {
            if (s == "#")
            {
                if (start)
                    start = false;
                else
                {
                    productions_list.push_back(currentProduction);
                    production new_production;
                    currentProduction = new_production;
                }
                infile >> s;
                lhsSymbol.value = s;
                lhsSymbol.is_terminal = false;
                push_to_non_terminals(lhsSymbol);
                currentProduction.LHS = lhsSymbol;
            }
            else if (s == "=")
                continue;
            else if (s == "|")
            {
                productions_list.push_back(currentProduction);
                production new_production;
                currentProduction = new_production;
                currentProduction.LHS = lhsSymbol;
            }
            else
            {
                symbol rhsSymbol;
                if (s[0] == '\'' && s[s.size() - 1] == '\'')
                {
                    rhsSymbol.value = s.substr(1, s.size()-2);
                    rhsSymbol.is_terminal = true;
                    push_to_terminals(rhsSymbol);
                }
                else
                {
                    rhsSymbol.value = s;
                    rhsSymbol.is_terminal = false;
                    push_to_non_terminals(rhsSymbol);
                }

                currentProduction.RHS.push_back(rhsSymbol);
            }
        }
        productions_list.push_back(currentProduction);
        move_epsilon();
    }

    /**
    task goes to: Fatma
    **/
//    bool is_first_contains_epsilon(symbol s){
//        vector<int> indexes = find_productions_with_LHS(s);
//        for(int i=0;i<indexes.size();i++){
//            if(productions_list[indexes[i]].RHS[0].value==EPSILON){
//                return true;
//            }
//        }
//        return false;
//    }
//    void get_all_firsts(){
//        for (int i = 0; i < non_terminals.size(); i++){
//            vector<string> first_set = get_first(non_terminals[i]);
//            non_terminals[i].first = first_set;
//        }
//        bool is_changed = true;
//        while (is_changed){
//            for (int i = 0; i < non_terminals.size(); i++){
//                ///TODO
//                vector<string> rhs;
//                for (int j = 0; j < rhs.size(); j++){
//                    if (is_first_contains_epsilon(rhs[j])){
//                        non_terminals[i].first.push_back(get_first())
//                    }else{
//                        ///TODO: move
//                        break;
//                    }
//                }
//            }
//        }
//    }
//    vector<string> get_first(symbol s){
//        vector<string> first_set;
//        vector<int> ind=find_productions_with_LHS(s);
//        bool changed=true;
//        for(int i=0;i<ind.size();i++){
//            vector<symbol> rhs=productions_list[ind[i]].RHS;
//            if(rhs[0].is_terminal){
//                int lhsInd=getI(s.value,non_terminals);
//                non_terminals[lhsInd].first.push_back(rhs[0]);
//            }
//        }
//
//
//        /*while(changed){
//            vector<symbol> rhs=productions_list[ind[i]].RHS;
//            if(rhs[0].)
//        }**/
//    }

  bool calculate_first(){
            bool change = false;
            for(int m = 0; m < non_terminals.size(); m++){
                for(int i = 0; i < productions_list.size(); i++){
                        if(non_terminals[m].value == productions_list[i].LHS.value){
                            bool ep = true;
                            for(int j = 0; ep && j < productions_list[i].RHS.size(); j++){
                            ep = false;
                           // cout << j <<" "<<productions_list[i].RHS[j].value<< endl;
                            symbol temp = match(productions_list[i].RHS[j]);
                                for(int k = 0; k < temp.first.size(); k++){
                                    if(temp.first[k] != EPSILON){
                                        bool found = false;
                                        for(int l = 0; l < non_terminals[m].first.size(); l++){
                                            if(non_terminals[m].first[l] == temp.first[k]){
                                                found = true;
                                            }
                                        }
                                        if(! found){
                                            non_terminals[m].first.push_back(temp.first[k]);
                                            change = true;
                                        }
                                    }
                                    else {
                                        if(temp.is_terminal){
                                            bool found = false;
                                            for(int l = 0; l < non_terminals[m].first.size(); l++){
                                                if(non_terminals[m].first[l] == temp.first[k]){
                                                    found = true;
                                                }
                                            }
                                            if(! found){
                                                non_terminals[m].first.push_back(temp.first[k]);
                                                change = true;
                                            }
                                        }
                                        else{
                                            ep = true;
                                        }
                                    }
                                }

                                }
                            }
                        }
                    }
            return change;
        }



        void call_first(){
            //to calculate first of terminals
            for (int i = 0; i < terminals.size(); i++){
                terminals[i].first.push_back(terminals[i].value);
            }

            //to calculate first of non_terminals
            bool change = calculate_first();
            while(change){
                change = calculate_first();
            }

            /*for(int i = 0; i < non_terminals.size(); i++){
                    cout << non_terminals[i].value << " : ";
                    for(int j = 0; j < non_terminals[i].first.size(); j++){
                        cout << non_terminals[i].first[j] << " ";
                    }
                    cout << endl;
            }*/
        }


        /**
        task goes to: Fatma
        **/


        symbol match(symbol s){
            for(int i = 0; i < non_terminals.size(); i++){
                if(s.value == non_terminals[i].value){
                    return non_terminals[i];
                }
            }
            for(int i = 0; i < terminals.size(); i++){
                if(s.value == terminals[i].value){
                    return terminals[i];
                }
            }
        }


        bool calculate_follow(){
            bool change = false;
            for(int m = 0; m < non_terminals.size(); m++){
                symbol s = non_terminals [m];
                for(int i = 0; i < productions_list.size(); i++){
                    for(int j = 0; j < productions_list[i].RHS.size(); j++){
                        if(s.value == productions_list[i].RHS[j].value){
                            bool ep = false;
                            if(j != productions_list[i].RHS.size()-1){
                                symbol temp = match(productions_list[i].RHS[j+1]);
                                for(int k = 0; k < temp.first.size(); k++){
                                    if(temp.first[k] != EPSILON){
                                        bool found = false;
                                        for(int l = 0; l < non_terminals[m].follow.size(); l++){
                                            if(non_terminals[m].follow[l] == temp.first[k]){
                                                found = true;
                                            }
                                        }
                                        if(! found){
                                            non_terminals[m].follow.push_back(temp.first[k]);
                                            change = true;
                                        }
                                    }
                                    else{
                                        ep = true;
                                    }
                                }
                            }
                            if((j == productions_list[i].RHS.size()-1)||(ep)){
                                symbol temp = match(productions_list[i].LHS);
                                for(int k = 0; k < temp.follow.size(); k++){
                                    bool found = false;
                                    for(int l = 0; l < non_terminals[m].follow.size(); l++){
                                        if(non_terminals [m].follow[l] == temp.follow[k]){
                                            found = true;
                                        }
                                    }
                                    if(! found){
                                        non_terminals [m].follow.push_back(temp.follow[k]);
                                        change = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return change;
        }

        void call_follow(){
            for(int i = 0; i < non_terminals.size(); i++){
                if(non_terminals[i].value == productions_list[0].LHS.value){
                    non_terminals[i].follow.push_back("$");
                    }
                }
            bool change = calculate_follow();
            while(change){
                change = calculate_follow();
            }
            /*for(int i = 0; i < non_terminals.size(); i++){
                cout << non_terminals[i].value << " : ";
                for(int j = 0; j < non_terminals[i].follow.size(); j++){
                    cout << non_terminals[i].follow[j] << " ";
                    }
                    cout << endl;
            }*/
        }


    /**
    fills predictive_table
    task goes to: Salma
    **/
    int getI(string s,vector<symbol> v){
        for(int i=0;i<v.size();i++){
            if(s==v[i].value){
                return i;
            }
        }
        return -1;
    }
    bool containsEpsilon(production p){
        for(int i=0;i<p.RHS.size();i++){
            if(p.RHS[i].value==EPSILON){
                return true;
            }
        }
        return false;
    }
    bool there_is_another_production(int ind){
        string s=productions_list[ind].LHS.value;
        for(int i=ind+1;i<productions_list.size();i++){
            if(productions_list[i].LHS.value==s){
                return true;
            }
        }
        return false;
    }
    void add_first_or_follow(vector<string> v,int productionNo,int productionIndex){
        vector<int> row=predictive_table[productionIndex];

        for(int i=0;i<terminals.size();i++){
            for(int j=0;j<v.size();j++){
                if(v[j]=="$"){
                    row[terminals.size()-1]=productionNo;
                }
                else if(v[j]==terminals[i].value){
                    row[i]=productionNo;
                }
            }
        }
        predictive_table[productionIndex]=row;
    }
    void fill_one_cell(string terminal,int productionNo,int productionIndex){
        vector<int> row=predictive_table[productionIndex];
        for(int i=0;i<terminals.size();i++){
            if(terminals[i].value==terminal){
                row[i]=productionNo;
            }
        }
        predictive_table[productionIndex]=row;
    }
    int in_follow(symbol sym,int lhsInd,string s){

        for(int i=0;i<non_terminals[lhsInd].follow.size();i++){
            if(non_terminals[lhsInd].follow[i]==s){
                return i;
            }
        }
        return -1;
    }
    void addSynch(symbol sym,int lhsInd,int productionIndex){
        vector<int> row=predictive_table[productionIndex];
        for(int i=0;i<terminals.size();i++){
            if((in_follow(sym,lhsInd,terminals[i].value)!=-1)&&(row[i] == 0) ){
                row[i]=-1;
            }
        }
        if(in_follow(sym,lhsInd,"$")!=-1){
            row[terminals.size()-1]=-1;
        }
        predictive_table[productionIndex]=row;
    }


    void generate_predictive_table()
    {
        vector<string> first;
        vector<string> follow;
        symbol lhs;
        for(int i=0;i<productions_list.size();i++){
            lhs=match(productions_list[i].LHS);
            int lhsInd=getI(lhs.value,non_terminals);
            first=lhs.first;
            follow=lhs.follow;
            if(containsEpsilon(productions_list[i])){
                //add follow to table
                add_first_or_follow(follow,i+1,lhsInd);
            }else{
                symbol first_symbol_rhs;
                first_symbol_rhs=match(productions_list[i].RHS[0]);
                first=first_symbol_rhs.first;
                add_first_or_follow(first,i+1,lhsInd);
            }
            addSynch(lhs,lhsInd,lhsInd);
        }

    }

    void push_to_terminals(symbol s){
        if (!existing_terminals[s.value])
            {
                terminals.push_back(s);
                existing_terminals[s.value] = true;
            }

        //resize table
        for(int i=0;i<predictive_table.size();i++){
            predictive_table[i].resize(terminals.size());
        }
    }

    void push_to_non_terminals(symbol s){
        if (!existing_non_terminals[s.value])
            {
                non_terminals.push_back(s);
                existing_non_terminals[s.value] = true;
            }
        predictive_table.resize(non_terminals.size());
    }

    void print_productions()
    {
        cout << "Size = " << productions_list.size() << endl;
        for (unsigned int i = 0; i < productions_list.size(); i++)
            productions_list[i].print_production();
    }

    void print_terminals(){
        cout << "terminals are: " << endl;
        cout<<"  ";
        for (unsigned int i = 0; i < terminals.size(); i++){
            cout << terminals[i].value <<" " ;
        }
        cout<<endl;
    }

    void print_non_terminals(){
        cout << "non-terminals are: " << endl;
        for (unsigned int i = 0; i < non_terminals.size(); i++){
            cout << non_terminals[i].value << endl;
        }
    }
    void eliminate_left_recursion(){
        vector<int> productions_to_remove;
        for (int i = 0; i < productions_list.size(); i++){
            production current_production = productions_list[i];
            if(contains_left_recursion(current_production)){
                cout << "left recursion found at index " << i << endl;
                productions_list.erase(productions_list.begin() + i);
                i--;
                symbol new_symbol;
                new_symbol.value = current_production.LHS.value +  std::to_string(++(extra_states[current_production.LHS.value]));
                cout << "new symbol value = " << new_symbol.value << endl;
                new_symbol.is_terminal = false;
                push_to_non_terminals(new_symbol);
                production new_production;
                new_production.LHS = new_symbol;
                new_production.RHS = sub_vector(current_production.RHS, 1, current_production.RHS.size() );
                new_production.RHS.push_back(new_symbol);
                productions_list.push_back(new_production);
                symbol epsilon_symbol;
                epsilon_symbol.value = EPSILON;
                epsilon_symbol.is_terminal = true;
                new_production.RHS = {epsilon_symbol};
                productions_list.push_back(new_production);
                vector<int> indexes = find_productions_with_LHS(current_production.LHS);
                for (int j = 0; j < indexes.size(); j++){
                    int current_index = indexes[j];
                    if (!contains_left_recursion(productions_list[current_index])){
                        production alternative_production;
                        alternative_production.LHS = current_production.LHS;
                        alternative_production.RHS = productions_list[current_index].RHS;
                        alternative_production.RHS.push_back(new_symbol);
                        productions_list[current_index] = alternative_production;
                    }
                }
            }

        }
    }

    void left_factor(){
        for (int i = 0; i < productions_list.size(); i++){
            production current_production = productions_list[i];
            if (current_production.RHS.size() > 0){
                vector<int> indexes = find_productions_with_LHS_and_first_RHS(current_production.LHS, current_production.RHS[0]);
                if (indexes.size() > 1){
                    symbol new_symbol;
                    new_symbol.value = "s" + to_string(++new_productions_count);
                    new_symbol.is_terminal = false;
                    push_to_non_terminals(new_symbol);
                    production new_production;
                    new_production.LHS = current_production.LHS;
                    new_production.RHS = {current_production.RHS[0], new_symbol};
                    productions_list.push_back(new_production);
                    for (int j = 0; j < indexes.size(); j++){
                        productions_list[indexes[j]].RHS = {sub_vector(productions_list[indexes[j]].RHS, 1, productions_list[indexes[j]].RHS.size())};
                        productions_list[indexes[j]].LHS = new_symbol;
                    }
                }
            }
        }
    }
    vector<int> find_productions_with_LHS(symbol lhs){
        vector<int> indexes;
        for (int i = 0; i < productions_list.size(); i++){
            if (productions_list[i].LHS.value == lhs.value){
                indexes.push_back(i);
            }
        }
        return indexes;
    }

    vector<int> find_productions_with_LHS_and_first_RHS(symbol lhs, symbol rhs){
        vector<int> indexes;
        for (int i = 0; i < productions_list.size(); i++){
            if (productions_list[i].LHS.value == lhs.value
                && productions_list[i].RHS.size() > 0
                && productions_list[i].RHS[0].value == rhs.value
                && productions_list[i].RHS[0].is_terminal == rhs.is_terminal){
                    indexes.push_back(i);
            }
        }
        return indexes;
    }

    vector<symbol> sub_vector(vector<symbol> v, int left, int right){
        vector<symbol> result;
        for (int i = left; i < right; i++){
            result.push_back(v[i]);
        }
        return result;
    }

    bool contains_left_recursion(production p){
        symbol lhs = p.LHS;
        if (p.RHS.size() > 0)
            return lhs.value == p.RHS[0].value;
        return false;
    }

    void move_epsilon(){
        bool epsilon_exists = false;
        symbol epsilon_symbol;
        for (int i = 0; i < terminals.size(); i++){
            if (terminals[i].value == EPSILON){
                epsilon_exists = true;
                epsilon_symbol = terminals[i];
                terminals.erase(terminals.begin() + i);
            }
        }
        if (epsilon_exists){
            terminals.push_back(epsilon_symbol);
        }
    }

    void print_new_file(){
        ofstream file;
        file.open("rules2.txt");
        for (int i = 0; i < productions_list.size(); i++){
            production current_production = productions_list[i];
            file << "# " << current_production.LHS.value << " = ";
            for (int j = 0; j < current_production.RHS.size(); j++){
                if (current_production.RHS[j].is_terminal){
                    file << "'" << current_production.RHS[j].value << "' ";
                }else{
                    file << current_production.RHS[j].value << " ";
                }
            }
            file << endl;
        }
    }
protected:
private:
};

#endif // GRAMMAR_H
