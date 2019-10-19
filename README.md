# Java Lexical Analyzer and Parser Generator
## Overview
This project was a part of CS321 Programming Languages and Translators course project. This repository contains the first two phases:
### 1. Lexical Analyzer
The lexical analyzer generator is required to automatically construct a lexical analyzer from a regular expression description of a set of tokens. The tool is required to construct a
nondeterministic finite automata (NFA) for the given regular expressions, combine these NFAs together with a new starting state, convert the resulting NFA to a DFA, minimize it and emit the transition table for the reduced DFA together with a lexical analyzer program that simulates the resulting DFA machine.

### 2. Parser Generator
The parser generator expects a grammar as input. It should compute First and Follow
sets and uses them to construct a predictive parsing table for the grammar.
The table is to be used to drive a predictive top-down parser. A bonus task we included is converting any grammar to be LL (1)

## Architecture
The project contains the following classes:
### nfa
This class represents an NFA. It contains the following attributes:
1. alphabet

    a vector of characters representing all possible inputs.
2. transition table
    
    represented by a 3 dimensional vector of integers, or a 2 dimensional vector where each entry is a vector. each row corresponds to a state, each column represents an input. each entry in row i and column j is a vector of characters representing the next states to state i under input alphabet[j]
3. accepted states

    A vector of integer representing indices of acceptance states
### dfa_state
This class represents one state of the DFA. It contains the following attributes
1. corresponding_states

    A vector of int.contains the indices of the NFA that correspond to this Dfa state
2. next_states

    A vector of integers, its length is the same size as the alphabet vector. element i represents the next state when the input is alphabet[j]
3. accepted
    A boolean that tells whether this is an acceptance state or not
3. state_name
    
    A string representing the name of the state if it is accepted (i.e: num, id, keyword .. ), “unaccepted” otherwise
### dfa
this class is responsible for converting an nfa to a dfa. Its attributes are
1. NFA
    
    type is nfa. represents the nfa we are trying to convert
2. transition_table
    a vector of dfa_state representing the states of the DFA with their corresponding NFA states and their next states in the DFA.
3. alphabet
    vector of character representing the alphabet
4. epsilon_closures_table
    a two dimensional vector of integer. each row i represents a vector of all states to which state i goes to under input epsilon
5. accepted_states
    a vector of integer representing indices of acceptance states
### minimization
this class is used for minimization a DFA. Its important attributes are
1. next_states 

    a two dimensional vector of integers. rows represent the states and columns represent under which input symbol.
2. minimized_table

    a  two dimensional vector of integers. rows represent a group of states and columns represent the other states in the same group, it is used through the partitions, so it has some cells which must be ignored.
3. final_minimized_table

    is the same as the minimized_table but without the cells of empty spaces.
4. DFA

    Its type is dfa and is the DFA which we want to minimize.

### dfa_instance
this class is responsible for reading the input string and splitting it into tokens
it uses a DFA (from class dfa), gets a string and splits the string into tokens.

### grammar : 

This class represents a grammar with attributes:

1. non_terminals

     vector of symbols which are non terminals.
2. terminals

     vector of symbols which are terminals.
3. productions_list
    
    vector of productions which represents the grammar rules.
###   symbol 

This class represents a symbol with attributes :

1. value

    string of the symbol value (name).
2. is_terminal

    a boolean attribute that determines if this symbol is one of the terminal symbols or not.
3. first
    
    vector of strings and each string is one of the terminal symbols value.
4. first_production

    vector of integers and each integer refers to the index of production (in productions_list) that generates that first .
5. follow

    vector of strings and each string is one of the follow of that symbol.
6. finish_first

    boolean attribute which shows if that symbol’s first attribute has been calculated.
###  production :

This class represents a production with attributes :
1. LHS : symbol(non terminal).
2.   RHS : vector of symbols(terminals with non terminals).
###  output :

This class represents the output with attribute:

*   rhs_productions_stack: stack of symbols
## Algorithms Used
### Eliminating left recursion
for each production P<sub>j</sub> in productions vector \
if left hand side LHS<sub>1</sub> is equal to the first symbol in right hand side (left recursion detected):

1. create a new production (P<sub>new</sub>) , with a new symbol (S<sub>new</sub>) in the left hand side.
2. P<sub>new</sub>.LHS = S<sub>new</sub>, P<sub>new</sub>.RHS = P<sub>j</sub>.RHS after removing first element.
3. add P<sub>new</sub> to the productions list.
4. find every other production (p<sub>i</sub>) with the same left hand side.
5. add the new symbol to the right hand side vector of each production with same LHS.
6. create a new production with left-hand-side = LHS<sub>1 </sub>and right-hand-side = p<sub>i</sub>.right_hand_side + S<sub>new</sub>
    
### Left factoring
for each production p<sub>i</sub>: \
find indices of all production with the same left-hand-side and same first symbol in right-hand-side and push them all to vector v \
if v.size is greater than one (i.e: there are other productions that make the grammar not left-factored): \
for each element in v (v<sub>j</sub>):

1. create new symbol S<sub>new</sub>.
2. create new production P<sub>new</sub>.
3. P<sub>new</sub>.left-hand-side = P<sub>i</sub>.left-hand-side
4. P<sub>new</sub>.right-hand-side = {P<sub>i</sub>.RHS[0], new_symbol}
5. push P<sub>new</sub> to productions_list 
6.  productions_list[v<sub>j</sub>].RHS = {sub_vector(productions_list[v<sub>j</sub>].RHS, 1, productions_list[v<sub>j</sub>].end};
7. productions_list[v<sub>j</sub>].LHS = S<sub>new</sub>; \

### calculating first sets
we use iterative technique to calculate the first set for every symbol in the grammar (terminal or non terminal) until there is no change with some rules :
*   If the symbol is terminal, its first is the symbol itself.
*   if the symbol is non terminal : 
*   if the first symbol in the right hand side does not have ‘/L’ in its first set, its first is the first of that symbol.
*   if the first symbol in the right hand side has ‘/L’ in its first set, its first is the first of that symbol with the first of the next symbol and that is repeated until the symbol does have ‘/L’ in its first set.
### calculating follow sets

we iterate to get the final follow set for each non terminal symbol until there is no change in every follow set by using some rules : 

*   If the symbol is the starting symbol, one of its follow is “$”.
*   If the symbol exists in the right hand side of one of the productions :
*   If it is the last symbol in the right hand side of the production or the following symbol has  ‘/L’ in its first set, its follow set have all follows of the left hand side symbol.
*    If it is not the last symbol in the right hand side of the production and the following symbol does not have ‘/L’ in its first set, its follow set have all firsts of the following symbol.
### constructing predictive parsing table

for each production rule A   of a grammar G


– for each terminal a in FIRST()


add A   to M[A,a]


– If  in FIRST()


for each terminal a in FOLLOW(A) add A   to M[A,a]


– If  in FIRST() and $ in FOLLOW(A)


add A   to M[A,$]

### generating output file

Initialize a stack containing S$. 

Repeat until the stack is empty:
*   Let the next character of ω be t.
*   If the top of the stack is a terminal r: 

    – If r and t don't match, report an error.


    – Otherwise consume the character t and pop r from the stack.

*   Otherwise, the top of the stack is a nonterminal A: 

    – If T[A, t] is undefined, report an error. 


    – Replace the top of the stack with T[A, t]

### handling errors using Panic-Mode error recovery

– For each nonterminal A, mark the entries M[A,a] as synch if a is in the follow set of A. So,for an empty entry, the input symbol is discarded. This should continue until either:


1) an entry with a production is encountered. In the case, parsing is continued as usual.


2) an entry marked as synch is encountered. In this case, the parser will pop that non-terminal A from the stack. The parsing continues from that state.


– To handle unmatched terminal symbols, the parser pops that unmatched terminal symbol


from the stack and it issues an error message saying that that unmatched terminal is inserted.

