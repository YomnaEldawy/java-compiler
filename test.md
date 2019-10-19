<!----- Conversion time: 1.901 seconds.


Using this Markdown file:

1. Cut and paste this output into your source file.
2. See the notes and action items below regarding this conversion run.
3. Check the rendered output (headings, lists, code blocks, tables) for proper
   formatting and use a linkchecker before you publish this page.

Conversion notes:

* Docs to Markdown version 1.0β17
* Sat Oct 19 2019 12:12:39 GMT-0700 (PDT)
* Source doc: https://docs.google.com/open?id=1RfNPrufsJdmWdEpxFk2thbuNY3F69ouH9-dRVd8oo9M

WARNING:
Inline drawings not supported: look for ">>>>>  gd2md-html alert:  inline drawings..." in output.

* This document has images: check for >>>>>  gd2md-html alert:  inline image link in generated source and store images to your server.
----->


<p style="color: red; font-weight: bold">>>>>>  gd2md-html alert:  ERRORs: 0; WARNINGs: 1; ALERTS: 3.</p>
<ul style="color: red; font-weight: bold"><li>See top comment block for details on ERRORs and WARNINGs. <li>In the converted Markdown or HTML, search for inline alerts that start with >>>>>  gd2md-html alert:  for specific instances that need correction.</ul>

<p style="color: red; font-weight: bold">Links to alert messages:</p><a href="#gdcalert1">alert1</a>
<a href="#gdcalert2">alert2</a>
<a href="#gdcalert3">alert3</a>

<p style="color: red; font-weight: bold">>>>>> PLEASE check and correct alert issues and delete this message and the inline alerts.<hr></p>




<p id="gdcalert1" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/Compilers-Phase20.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert2">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/Compilers-Phase20.png "image_tooltip")


**Alexandria University**


# **Faculty of Engineering**


# **Computer and Systems Engineering Department**

**Programming Languages and Compilers**



<p id="gdcalert2" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline drawings not supported directly from Docs. You may want to copy the inline drawing to a standalone drawing and export by reference. See <a href="https://github.com/evbacher/gd2md-html/wiki/Google-Drawings-by-reference">Google Drawings by reference</a> for details. The img URL below is a placeholder. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert3">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![drawing](https://docs.google.com/a/google.com/drawings/d/12345/export/png)

**Project Phase 2**

**Parser Generator**

**Team members :**

Salma Samir El-Gammal         	 31

Fatma Mohamed Abd El-Aty           43

Yomna Saad El-Dawy                       72





1.  A description of used data structures:

    We splitted the project to the following classes:

*   grammar : 

        This class represents a grammar with attributes :

    *   non_terminals : vector of symbols which are non terminals.
    *   terminals : vector of symbols which are terminals.
    *   productions_list : vector of productions which represents the grammar rules.
*   symbol :

        This class represents a symbol with attributes :

    *   value : string of the symbol value (name).
    *   is_terminal : a boolean attribute that determines if this symbol is one of the terminal symbols or not.
    *   first : vector of strings and each string is one of the terminal symbols value.
    *   first_production : vector of integers and each integer refers to the index of production (in productions_list) that generates that first .
    *   follow : vector of strings and each string is one of the follow of that symbol.
    *   finish_first : boolean attribute which shows if that symbol’s first attribute has been calculated.
*   production :

        This class represents a production with attributes :

    *   LHS : symbol(non terminal).
    *   RHS : vector of symbols(terminals with non terminals).
*   output :

        This class represents the output with attributes:

*   rhs_productions_stack: stack of symbols
2.  All algorithms and techniques used:
    1. Eliminating left recursion \
for each production P<sub>j</sub> in productions vector \
if left hand side LHS<sub>1</sub> is equal to the first symbol in right hand side (left recursion detected):
        1. create a new production (P<sub>new</sub>) , with a new symbol (S<sub>new</sub>) in the left hand side.
        2. P<sub>new</sub>.LHS = S<sub>new</sub>, P<sub>new</sub>.RHS = P<sub>j</sub>.RHS after removing first element.
        3. add P<sub>new</sub> to the productions list.
        4. find every other production (p<sub>i</sub>) with the same left hand side.
        5. add the new symbol to the right hand side vector of each production with same LHS.
        6. create a new production with left-hand-side = LHS<sub>1 </sub>and right-hand-side = p<sub>i</sub>.right_hand_side + S<sub>new</sub>
    2. Left factoring \
for each production p<sub>i</sub>: \
find indices of all production with the same left-hand-side and same first symbol in right-hand-side and push them all to vector v \
if v.size is greater than one (i.e: there are other productions that make the grammar not left-factored): \
for each element in v (v<sub>j</sub>):
        7. create new symbol S<sub>new</sub>.
        8. create new production P<sub>new</sub>.
        9. P<sub>new</sub>.left-hand-side = P<sub>i</sub>.left-hand-side
        10. P<sub>new</sub>.right-hand-side = {P<sub>i</sub>.RHS[0], new_symbol}
        11. push P<sub>new</sub> to productions_list 
        12.  productions_list[v<sub>j</sub>].RHS = {sub_vector(productions_list[v<sub>j</sub>].RHS, 1, productions_list[v<sub>j</sub>].end};
        13. productions_list[v<sub>j</sub>].LHS = S<sub>new</sub>; \

    3. calculating first sets

        we use iterative technique to calculate the first set for every symbol in the grammar (terminal or non terminal) until there is no change with some rules :

*   If the symbol is terminal, its first is the symbol itself.
*   if the symbol is non terminal : 
    *   if the first symbol in the right hand side does not have ‘/L’ in its first set, its first is the first of that symbol.
    *   if the first symbol in the right hand side has ‘/L’ in its first set, its first is the first of that symbol with the first of the next symbol and that is repeated until the symbol does have ‘/L’ in its first set.
    4. calculating follow sets

        we iterate to get the final follow set for each non terminal symbol until there is no change in every follow set by using some rules : 

*   If the symbol is the starting symbol, one of its follow is “$”.
*   If the symbol exists in the right hand side of one of the productions :
    *   If it is the last symbol in the right hand side of the production or the following symbol has  ‘/L’ in its first set, its follow set have all follows of the left hand side symbol.
    *    If it is not the last symbol in the right hand side of the production and the following symbol does not have ‘/L’ in its first set, its follow set have all firsts of the following symbol.
    5. constructing predictive parsing table

        for each production rule A   of a grammar G


        – for each terminal a in FIRST()


        add A   to M[A,a]


        – If  in FIRST()


        for each terminal a in FOLLOW(A) add A   to M[A,a]


        – If  in FIRST() and $ in FOLLOW(A)


        add A   to M[A,$]

    6. generating output file

        Initialize a stack containing S$. 


        Repeat until the stack is empty:

*   Let the next character of ω be t.
*   If the top of the stack is a terminal r: 

            – If r and t don't match, report an error.


            – Otherwise consume the character t and pop r from the stack.

*   Otherwise, the top of the stack is a nonterminal A: 

            – If T[A, t] is undefined, report an error. 


            – Replace the top of the stack with T[A, t]

    7. handling errors using Panic-Mode error recovery

        – For each nonterminal A, mark the entries M[A,a] as synch if a is in the follow set of A. So,for an empty entry, the input symbol is discarded. This should continue until either:


            1) an entry with a production is encountered. In the case, parsing is continued as usual.


            2) an entry marked as synch is encountered. In this case, the parser will pop that non-terminal A from the stack. The parsing continues from that state.


        – To handle unmatched terminal symbols, the parser pops that unmatched terminal symbol


        from the stack and it issues an error message saying that that unmatched terminal is


        inserted.

3.  Parsing Tables if any

    

<p id="gdcalert3" ><span style="color: red; font-weight: bold">>>>>>  gd2md-html alert: inline image link here (to images/Compilers-Phase21.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert4">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>>> </span></p>


![alt_text](images/Compilers-Phase21.png "image_tooltip")


4.  Explanation of functions of all phases
5.  Any assumptions made and their justification.
    8. context-free grammar file:
        14. is space separated (separators are not optional).
        15. is syntactically correct. 
        16. each production starts with a ‘#’ before left-hand-side.
    9. Lambda symbol is represented like a terminal symbol with single quotes around (i.e: ‘\L’)
    10. The non terminal symbol in the left hand side of the first production is the starting symbol.

<!-- Docs to Markdown version 1.0β17 -->
