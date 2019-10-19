#ifndef TESTNFA_H
#define TESTNFA_H
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

class testNFA
{
    public:

struct RD{
    char representation;
    string rdName;
    char firstChar;
    char secondChar;
};

    vector<RD> allRDs;
    vector<string> reNames;
    vector<char> inputs;
    void parseStr(string s){
        string lhs=extractLHS(s);
        int midI=lhs.length();
        char midC=s.at(midI);      // = or :
        char c='a';
        if(midC=='='){
            RD d;
            d.rdName=temp;
            d.representation=c+allRDs.size();
            setRange(s.substr(midI+1,s.length()-lhs.length()-1),d);
        }else{

        }


    }
    void setInputs(char c1,char c2){
        for(char c=c1+1;c<c2;(int)c++){
            inputs.push_back(c);
        }
    }

    bool checkLetter(char c){
        if((c>='a' && c<='z')||(c>='A' && c<='Z')||(c>='0' && c<='9')){
            return true;
        }
        return false;
    }

    void setRange(string rhs,RD d){
        for(int k=0;k<rhs.length();k++){
            if(rhs.at(k)=='-'){

                d.firstChar=rhs.at(k-1);
                d.secondChar=rhs.at(k+1);
                allRDs.push_back(d);
                setInputs(rhs.at(k-1),rhs.at(k+1));
                k++;
            }
            else if(checkLetter(rhs.at(k))){
                inputs.push_back(s.at(k));
            }
        }
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
    // removes each regular definition and replaces it with its character representation
    string mapRD(string s){
        string temp="";
        for(int l=0;l<s.length();l++){
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
    }

    string removeBackslash(string s){
        for(int i=0;i<s.length();i++){
            string t=s.substr(i+1,s.length()-1);
            s.replace(i,s.length()-i,t);
            i--;
        }
        return s;
    }
    bool notOp(char c){
        if(c!='+' && c!='*' && c!='|'){
            return true;
        }
        return false;
    }
    string addAt(string s,bool b){
        if(b){
            for(int i=0;i<s.length();i++){
                if((s.at(i)=='(' && i>1) || (s.at(i-1)==')' || (notOp(s.at(i)) && i>1)){
                     string t=string(1,'@')+s.substr(i,s.length()-1);
                     s.replace(i,s.length()-i,t);
                     i++;
                }
            }
        }else{
            for(int i=1;i<s.length();i++){
                if(s.at(i)=='\\'){
                    s.at(i)='@';
                }
            }
        }
    }

};

#endif // TESTNFA_H
