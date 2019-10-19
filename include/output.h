#ifndef OUTPUT_H
#define OUTPUT_H
#include <production.h>
#include <symbol.h>
#include <grammar.h>
#include <stack>

class output
{
    public:
        //output();

    stack<symbol> rhs_productions_stack;
    string EPSILON="\\L";

    void print_production(vector<symbol> v){
        for(int i=0;i<v.size();i++){
            cout<<v[i].value;
        }
        cout<<endl;
    }
    int getI(vector<symbol> v, symbol s){
        for(int i=0;i<v.size();i++){
            if(s.value==v[i].value){
                return i;
            }
        }
        return -1;
    }
    int check_in_table(grammar g,symbol sym,string s){
        vector<vector<int> > v = g.predictive_table;
        int row_ind=getI(g.non_terminals,sym);
        int col_ind=-1;

        if(s=="$"){
            col_ind=g.terminals.size()-1;
        }else{
            for(int i=0;i<g.terminals.size();i++){
                if(g.terminals[i].value== s.substr(0,g.terminals[i].value.length())){
                    col_ind=i;
                    break;
                }
            }
        }


        if(col_ind==-1){
            return -1;
        }
        int production_no=v[row_ind][col_ind];
        return production_no;
    }
    void print_stack(stack<symbol> s){
        while(!s.empty()){
            cout<<s.top().value<<" ";
            s.pop();
        }
        cout<<endl;
    }
    void generate_output(grammar *g, vector<string> s){
        s.push_back("$");
        //cout<<s<<endl;
        symbol dollar_sign;
        dollar_sign.value="$";
        dollar_sign.is_terminal=true;
        (*g).generate_predictive_table();

        rhs_productions_stack.push(dollar_sign);          //push starting symbol
        rhs_productions_stack.push((*g).productions_list[0].LHS);

        while(!rhs_productions_stack.empty()){
            print_stack(rhs_productions_stack);
            //cout<<"     "<<s[0]<<endl;
            symbol first=rhs_productions_stack.top();

            if(first.is_terminal){
                int l=first.value.length();
                if(first.value=="$" && s[0]=="$"){
                    rhs_productions_stack.pop();
                }
                else if(first.value==s[0]){
                    rhs_productions_stack.pop();
                    s.erase(s.begin());
                }else{
                    //report error
                    cout<<"Error: missing "<<first.value<<", inserted"<<endl;
                    rhs_productions_stack.pop();
                }

            }else{ // first is non-terminal
                int production_no=check_in_table((*g),first,s[0]);
                if(production_no>0){
                    rhs_productions_stack.pop();
                    vector<symbol> v2=(*g).productions_list[production_no-1].RHS;
                    if(v2[0].value!=EPSILON){
                        for(int i=v2.size()-1;i>=0;i--){
                            rhs_productions_stack.push(v2[i]);
                        }

                    }
                }else{
                    while(!production_no){
                        cout<<"Error:(illegal "<<first.value<<") - discard "<<s[0]<<endl;
                        s.erase(s.begin());
                        production_no=check_in_table((*g),first,s[0]);
                    }
                    if(production_no==-1){
                        rhs_productions_stack.pop();
                    }else if(production_no){
                        rhs_productions_stack.pop();
                        vector<symbol> v2=(*g).productions_list[production_no-1].RHS;
                        if(v2[0].value!=EPSILON){
                            for(int i=v2.size()-1;i>=0;i--){
                                rhs_productions_stack.push(v2[i]);
                            }

                        }
                    }
                }
            }

        }
    }
    protected:

    private:
};

#endif // OUTPUT_H
