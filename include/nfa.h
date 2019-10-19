#ifndef NFA_H
#define NFA_H
#include <vector>
#include <iostream>
#include <stack>
using namespace std;
class nfa
{
public:

    vector<int> accepted_states;
    struct RD{
    string rdName;
    char representation;
    char firstChar;
    char secondChar;
};

vector<string> keywords;
vector<pair<string,bool> > re;
vector<string> reNames;
vector<char> punc;
vector<RD> allRDs;
vector<string> accepted_states_names;
vector<int> startingStates;
int nRD=0;
vector<char> regularType;
vector<char> inputs;
vector<vector<vector<int> > > transition_table;
int starting_state,nStates=0,nLetters;
bool isOp=false;

void setStartingState(){
    vector<vector<int> > state(nLetters);
    for(int i=0;i<startingStates.size();i++){
        state[nLetters-1].push_back(startingStates[i]);
    }
    transition_table.push_back(state);
    starting_state=nStates;
    nStates++;

}
int getI(vector<char> v,char c){
    for(int i=0;i<v.size();i++){
        if(v[i]==c)
            return i;
    }
    return -1;
}
int pushAC(){
    vector<vector<int> > state(nLetters);
    transition_table.push_back(state);

}
void handleKeywords(){
    nLetters=inputs.size();
    vector<vector<int> > state(nLetters);
    int ind;
    for(int i=0;i<keywords.size();i++){
        startingStates.push_back(nStates);
        for(int j=0;j<keywords[i].length();j++){
            ind=getI(inputs,keywords[i].at(j));
            for(int k=0;k<nLetters;k++){
                if(k==ind){
                    state[k].push_back(nStates+1);
                    nStates++;
                }
            }
            transition_table.push_back(state);
            for(int i=0;i<nLetters;i++){
                state[i].clear();
            }
        }
        pushAC();
        accepted_states.push_back(nStates);
        accepted_states_names.push_back(keywords[i]);
        nStates++;
    }
}

void handlePunc(){
    nLetters=inputs.size();
    vector<vector<int> > state(nLetters);
    int ind;
    for(int i=0;i<punc.size();i++){
        startingStates.push_back(nStates);
        ind=getI(inputs,punc[i]);
        for(int k=0;k<nLetters;k++){
            if(k==ind){
                state[k].push_back(nStates+1);
                nStates++;
            }
        }
        transition_table.push_back(state);
        for(int k=0;k<nLetters;k++){
            state[k].clear();
        }
        pushAC();
        accepted_states.push_back(nStates);
        accepted_states_names.push_back(string(1,punc[i]));
        nStates++;
    }
}

void handleRE(){
    nLetters=inputs.size();
    for(int i=0;i<re.size();i++){
        pair<char,pair<int,int> > v=evaluatePostfix(infixToPostfix(re[i]));
        pushAC();
        startingStates.push_back(v.second.first);
        accepted_states.push_back(nStates-1);
        accepted_states_names.push_back(reNames[i]);
    }

}

void handleAll(){
    inputs.push_back('^');
    handleKeywords();
    handlePunc();
    handleRE();
    setStartingState();
}
void setInputs(char c1,char c2){
    for(char c=c1+1;c<c2;(int)c++){
        inputs.push_back(c);
    }
}

bool notExistsInLetters(char c){
    for(int i=0;i<inputs.size();i++){
        if(c==inputs[i]){
            return false;
        }
    }
    return true;
}
void splitStr(string s){
    bool b=false;           //for several spaces
    string temp="";
    for(int i=0;i<s.length();i++){
        if(s.at(i)!=' '){
            b=true;
            temp+=s.substr(i,1);
            if(notExistsInLetters(s.at(i)))
                inputs.push_back(s.at(i));
            if(i==s.length()-1){        //last string
                keywords.push_back(temp);
            }
        }else if (s.at(i)==' ' && b ){
            b=false;
            keywords.push_back(temp);
            temp="";
        }
    }
}
bool checkLetter(char c){
    if((c>='a' && c<='z')||(c>='A' && c<='Z')||(c>='0' && c<='9')){
        return true;
    }
    return false;
}
bool notOp(char c){
    if(c!='+' && c!='*' && c!='|'){
        return true;
    }
    return false;
}
string extractLHS(string s){
    string temp="";
    for(int i=0;i<s.length();i++){
        if(s.at(i)=='=' || s.at(i)==':'){
            break;
        }
        temp+=string(1,s.at(i));
    }

    return temp;
}
void setRange(string rhs,RD d){
    for(int k=0;k<rhs.length();k++){
        if(rhs.at(k)=='-'){

            d.firstChar=rhs.at(k-1);
            d.secondChar=rhs.at(k+1);
            allRDs.push_back(d);
            setInputs(rhs.at(k-1),rhs.at(k+1));
        }
        else if(checkLetter(rhs.at(k))){
            inputs.push_back(rhs.at(k));
        }
    }
}
string mapRD(string s){
    string temp="";
    int k;
    for(int l=0;l<s.length();l++){
        temp="";
        for(k=l;k<s.length() && checkLetter(s.at(k));k++)
            temp+=string(1,s.at(k));

        //search for temp in lhs vector
        for(int i=0;i<allRDs.size();i++){
            if(allRDs[i].rdName==temp){
                s.replace(l,temp.length(),string(1,allRDs[i].representation));

                break;
            }
        }
        l=k-temp.length();
    }
    return s;
}

string addAt(string s,bool b){
    if(b){
        for(int i=0;i<s.length();i++){
            if((s.at(i)=='(' && i>=1) || (i>0 && s.at(i-1)==')' && notOp(s.at(i)))  || s.at(i)=='.' || (i>0 && s.at(i-1)=='.')){
                 if(s.at(i)=='.'){
                    inputs.push_back('.');
                 }
                 string t=string(1,'@')+s.substr(i,s.length()-1);
                 s.replace(i,s.length()-i,t);
                 i++;

            }
        }
    }else{
        for(int i=0;i<s.length();i++){
            if(s.at(i)=='\\'){
                s.at(i)='@';
            }
        }
    }
    return s;
}
void addAllChars(string s){
    for(int i=0;i<s.length();i++){
        if(s.at(i)!='@' && s.at(i)!='|' && notExistsInLetters(s.at(i)))
            inputs.push_back(s.at(i));
    }
}
string parseStr(string s){
        int j=0;
        if(s.at(0)=='{'){
            splitStr(s.substr(1,s.length()-2));
        }
        else if(s.at(0)=='['){
            for(j=1; s.at(j)!=']' ;j++){
                if(s.at(j)!='\\'){
                    inputs.push_back(s.at(j));
                    punc.push_back(s.at(j));
                }
            }
        }
        else if(checkLetter(s.at(0))){
            string lhs=extractLHS(s);
            int midI=lhs.length();
            char midC=s.at(midI);      // = or :
            char c='a';
            if(midC=='='){
                RD d;
                d.rdName=lhs;
                d.representation=c+allRDs.size();
                setRange(s.substr(midI+1,s.length()-lhs.length()-1),d);
            }else{
                reNames.push_back(lhs);
                if(s.at(midI+1)=='=' && s.length()==midI+2){    //assign
                    if(notExistsInLetters('='))
                        inputs.push_back('=');
                    pair<string,bool> p(string(1,'='),true);
                    re.push_back(p);
                }
                else if(s.at(midI+1)=='\\'){
                    string afterAddAt=addAt(s.substr(midI+2,s.length()-lhs.length()-1),false);
                    addAllChars(afterAddAt);
                    pair<string,bool> p(afterAddAt,false);
                    re.push_back(p);
                }else{
                    s=addAt(mapRD(s.substr(midI+1,s.length()-lhs.length()-1)),true);
                    pair<string,bool> p(s,true);
                    re.push_back(p);
                }
        }


        }
            return s;

}


//********************************************************************************************************



int prec(char c)
{
	if(c == '*' || c=='+')
	return 3;
	else if(c == '@')
	return 2;
	else if(c == '|')
	return 1;
	else
	return -1;
}

int prec2(char c)
{
	if(c == '@')
	return 2;
	else if(c == '|')
	return 1;
	else
	return -1;
}
pair<string,bool> infixToPostfix(pair<string,bool> s)
{
	stack<char> st;
	int l = s.first.length();
	string temp="";
	for(int i = 0; i < l; i++){
		if(!notExistsInLetters(s.first[i]) && s.first[i]!='(' && s.first[i]!=')'){
            temp+=s.first[i];
		}else if(s.first[i] == '(')
            st.push('(');
		else if(s.first[i] == ')')
		{
			while(!st.empty() && st.top() != '(')
			{
				char c = st.top();
				st.pop();
                temp += string(1,c);
			}
			if(st.top() == '(')
			{
				st.pop();
			}
		}
		else{
            if(s.second){
                while(!st.empty() && prec(s.first[i]) <= prec(st.top()))
                {
                    char c = st.top();
                    st.pop();
                    temp += string(1,c);
                }
                st.push(s.first[i]);
            }else{
                while(!st.empty() && prec2(s.first[i]) <= prec2(st.top()))
                {
                    char c = st.top();
                    st.pop();
                    temp += string(1,c);
                }
                st.push(s.first[i]);
            }
		}
	}
	//Pop all the remaining elements from the stack
	while(!st.empty())
	{

		char c = st.top();
		st.pop();
		temp += string(1,c);
	}
	pair<string,bool> p(temp,s.second);
	return p;
}


pair<char,pair<int,int> > concatenateTwoChar(pair<char,pair<int,int> > c1,pair<char,pair<int,int> > c2,bool anotherR){
    vector<vector<int> > state(nLetters);
    pair<char,pair<int,int> > res;
    res.first='r';
    res.second.first=c1.second.first;
    res.second.second=c2.second.second;
    state[nLetters-1].push_back(c2.second.first);
    if(!anotherR){
        transition_table.insert(transition_table.begin()+c1.second.second,state);
    }else{
        transition_table.insert(transition_table.begin()+c1.second.second,state);
    }
    return res;

}

pair<char,pair<int,int> > ored(pair<char,pair<int,int> > c1,pair<char,pair<int,int> > c2,bool anotherR){
    pair<char,pair<int,int> > res;
    vector<vector<int> > state(nLetters);
    vector<vector<int> > state2(nLetters);
    state[nLetters-1].push_back(c1.second.first);        //new starting state
    state[nLetters-1].push_back(c2.second.first);
    res.second.first=c2.second.second+1;
    res.second.second=c2.second.second+2;

    transition_table.push_back(state);
    state2[nLetters-1].push_back(res.second.second);
    //change acceptance state
    if(!anotherR ){
        transition_table.insert(transition_table.begin()+c1.second.second,state2);
        transition_table.insert(transition_table.begin()+c2.second.second,state2);
    }else{
        transition_table.insert(transition_table.begin()+c1.second.second-1,state2);
        transition_table.insert(transition_table.begin()+c2.second.second-1,state2);
    }

    nStates+=2;
    res.first='r';
    return res;
}
pair<char,pair<int,int> > closure(pair<char,pair<int,int> > c){
    vector<vector<int> > state(nLetters);

    pair<char,pair<int,int> > res;
    state[nLetters-1].push_back(c.second.first);
    state[nLetters-1].push_back(c.second.second+2);      //new acceptance state

    transition_table.push_back(state);     //old acceptance state points to new acceptance state
    transition_table.push_back(state);     //new starting state
    nStates+=2;
    res.first='r';
    res.second.first=c.second.second+1;
    res.second.second=c.second.second+2;

    return res;
}

pair<char,pair<int,int> > oneOrMore(pair<char,pair<int,int> > c){
    vector<vector<int> > state(nLetters);
    pair<char,pair<int,int> > res;

    state[nLetters-1].push_back(c.second.first);
    state[nLetters-1].push_back(c.second.second+2);      //new acceptance state
    transition_table.push_back(state);
    state[nLetters-1].pop_back();
    transition_table.push_back(state);     //new starting state
    nStates+=2;
    res.first='r';
    res.second.first=c.second.second+1;
    res.second.second=c.second.second+2;
    return res;
}

void pushInTable(char c){
    int ind1,ind2;
    vector<vector<int> > state(nLetters);
    if(c>='a' && c<='z'){
        for(int i=0;i<allRDs.size();i++){
            if(allRDs[i].representation==c){
                ind1=getI(inputs,allRDs[i].firstChar);
                ind2=getI(inputs,allRDs[i].secondChar);
                break;
            }
        }

        for(int i=ind1;i<=ind2;i++){
            state[i].push_back(nStates+1);
        }
    }else{
        ind1=getI(inputs,c);
        state[ind1].push_back(nStates+1);
    }
    transition_table.push_back(state);
}
bool anotherR(string s,int i,char c){
    bool res=false;
    for(int ind=i;ind<s.length();ind++){
        if(s.at(ind)==c){
            res= true;
        }
    }
    return res;
}
pair<char,pair<int,int> > evaluatePostfix(pair<string,bool> s){
    stack<pair<char,pair<int,int> > > values;
    int l=s.first.length();
    for(int i=0;i<l;i++){
        if(s.first.at(i)=='@'){

            pair<char,pair<int,int> > input2=values.top();
            values.pop();
            pair<char,pair<int,int> > input1=values.top();
            values.pop();
            pair<char,pair<int,int> > res=concatenateTwoChar(input1,input2,anotherR(s.first,i+1,'|'));
            values.push(res);

        }else if(s.first.at(i)=='|'){
            pair<char,pair<int,int> > input2=values.top();
            values.pop();
            pair<char,pair<int,int> > input1=values.top();
            values.pop();
            pair<char,pair<int,int> > res=ored(input1,input2,anotherR(s.first,i+1,'@'));
            values.push(res);
        }else if(s.first.at(i)=='*' && s.second){
            pair<char,pair<int,int> > input2=values.top();
            values.pop();
            pair<char,pair<int,int> > res=closure(input2);
            values.push(res);
        }else if(s.first.at(i)=='+' && s.second){
            pair<char,pair<int,int> > input2=values.top();
            values.pop();
            pair<char,pair<int,int> > res=oneOrMore(input2);
            values.push(res);
        }else{
            pair<int,int> p1(nStates,nStates+1);
            pair<char,pair<int,int> > p(s.first.at(i),p1);
            values.push(p);                         //character 'a' has start state 0 and acceptance state 1
            pushInTable(s.first.at(i));
            nStates+=2;
        }
    }
    return values.top();
}

vector<vector<int> > get_state(int state_index){
    return transition_table[state_index];
}

vector<int> get_next_states(int state_number, char input)
{
    int index = -1;
    for (int i = 0; i < inputs.size(); i++)
    {
        if (inputs[i] == input)
        {
            index = i;
            break;
        }
    }
    return transition_table[state_number][index];
}

void displayTable(){
    for(int i=0;i<inputs.size();i++){
        cout<<inputs[i]<<" ";
    }
    cout<<endl;
    for(int i=0;i<transition_table.size();i++){
        cout<<"State "<<i<<endl;
        for(int j=0;j<transition_table[i].size();j++){
            cout<<"{";
            for(int k=0;k<transition_table[i][j].size();k++){
                cout<<transition_table[i][j][k]<<",";
            }
            cout<<"}";
        }
        cout<<endl;
    }

}
};

#endif // NFA_H
