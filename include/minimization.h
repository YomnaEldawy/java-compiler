#ifndef MINIMIZATION_H
#define MINIMIZATION_H
#include <vector>
#include <dfa.h>

using namespace std;

class minimization {
public:
	vector<vector<int> > next_states;
    vector<vector<int> > minimized_table;
    vector<vector<int> > final_minimized_table;
    vector<vector<int> > from_to;
    vector<int> input_generator;
    bool part = false;
    int length = 0;
    int input_index = 0;
    dfa Dfa;
    minimization(dfa Dfa){
   	 this->Dfa = Dfa;
    }
    void first_partition (){
    	int i = 0;
    	for(i = 0; i < Dfa.accepted_states.size(); i++){
   	 minimized_table.push_back({Dfa.accepted_states[i]});
   	 from_to.push_back({i, i});
   	 }
   	 minimized_table.push_back(Dfa.non_accepted_states);
   	 from_to.push_back({i, i});
    }
    void partition_after_input(){
   	 part = false;
   	 bool test = false;
   	 for(int i = 0; i < length; i++){
   		 if(!is_only(minimized_table[i])){
       		 for(int j = 0; j < minimized_table[i].size(); j++){
           		 if(minimized_table[i][j] != -2){
               		 test = false;
               		 for(int l = 0; ((l < length) && !test); l++){
                   		 for(int m = 0; ((m < minimized_table[i].size())&& !test); m++){
                       		 if(minimized_table[l][m] == Dfa.transition_table[minimized_table[i][j]].next_state[input_index]){
                           		 test = true;
                           		 if(l != from_to[i][1]){
                               		 part = true;
                               		 bool found = false;
                               		 for(int n = 0; n < from_to.size(); n++){
                                   		 if((i == from_to[n][0]) && (l == from_to[n][1]) && (input_generator[n] == input_index)){
                                       		 minimized_table[n].push_back(minimized_table[i][j]);
                                       		 found = true;
                                   		 }
                               		 }
                               		 if(!found){
                                   		 minimized_table.push_back({minimized_table[i][j]});
                                   		 from_to.push_back({i, l});
                                   		 input_generator.push_back(input_index);
                               		 }
                               		 minimized_table[i][j] = -2;
                           		 }
                       		 }
                   		 }
               		 }
           		 }
       		 }
   		 }
   	 }
    }
    void partioning(){
   	 first_partition();
   	 while(length != minimized_table.size()){
   		 length = minimized_table.size();
   		 input_index = 0;
   		 while(input_index < Dfa.alphabet.size()-1){
       		 partition_after_input();
           		 input_index++;
   		 }
   	 }
    }

    bool is_only(vector<int> v){
   	 int counter = 0;
   	 for(int i = 0; i < v.size(); i++){
   		 if(v[i] != -2){
       		 counter ++;
   		 }
   	 }
   	 if(counter ==1){
   		 return true;
   	 }
   	 else{
   		 return false;
   	 }
    }

    bool empty_vector(vector<int> v){
   	 bool flag = false;
   	 for(int i = 0; i < v.size(); i++){
   		 if(v[i] != -2){
       		 flag = true;
   		 }
   	 }
   	 if(flag){
        	return false;
   	 }
   	 else{
        	return true;
   	 }
    }

	void specify_next_states(){
    	bool flag = false;
    	for(int m = 0; m < final_minimized_table.size(); m++){
        	for(int i = 0; i < Dfa.alphabet.size()-1; i++){
            	int next_number = Dfa.transition_table[final_minimized_table[m][0]].next_state[i];
            	flag = false;
            	for(int j = 0; j < final_minimized_table.size(); j++){
                	for(int l = 0; l < final_minimized_table[j].size(); l++){
                    	if(next_number == final_minimized_table[j][l]){
                        	flag = true;
                        	if(i == 0){
                            	next_states.push_back({j});
                        	}
                        	else{
                            	next_states[m].push_back(j);
                        	}
                    	}
                	}
            	}
            	if(!flag){
                	if(i == 0){
                    	next_states.push_back({-1});
                	}
                	else{
                    	next_states[m].push_back(-1);
                	}
            	}
        	}
    	}
	}

	void after_minimization(){
    	for(int i = 0; i < minimized_table.size(); i++){
        	if(!empty_vector(minimized_table[i])){
            	vector<int> v;
            	for(int j = 0; j < minimized_table[i].size(); j++){
                	if(minimized_table[i][j] != -2){
                    	v.push_back(minimized_table[i][j]);
                	}
            	}
            	final_minimized_table.push_back(v);
        	}
    	}
	}

    void print_minimization (){
    	after_minimization();
    	specify_next_states();
   	 for(int i = 0; i < final_minimized_table.size(); i++){
        	cout << i << ") ";
   		 for(int j = 0; j < final_minimized_table[i].size(); j++){
       		 cout << final_minimized_table[i][j] << " " ;
   		 }
   		 cout << "| ";
   		 for(int j = 0; j < Dfa.alphabet.size()-1; j++){
            	cout << next_states[i][j] << " ";
   		 }
   		 cout << endl;
   	 }
    }
};

#endif // MINIMIZATION_H

