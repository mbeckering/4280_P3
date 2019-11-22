/* 
 * File:   parser.cpp
 * Author: Michael Beckering
 * Project 4
 * CS-4280-001-FS2019
 *
 * Created on November 1, 2019, 9:55 AM
 */

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include "parser.h"
#include "scanner.h"

using namespace std;

// global token for use in recursive descent parse functions
token t;

// global string array associated to the token.ID enumeration (for printing)
string tokenNames[] = {
        "ID_tk",      // 0
        "NUM_tk",     // 1
        "START_tk",   // 2
        "STOP_tk",    // 3
        "ITERATE_tk", // 4
        "VOID_tk",    // 5
        "VAR_tk",     // 6
        "RETURN_tk",  // 7
        "IN_tk",      // 8
        "OUT_tk",     // 9
        "PROGRAM_tk", // 10
        "COND_tk",    // 11
        "THEN_tk",    // 12
        "LET_tk",     // 13
        "EQUAL_tk",   // 14
        "LESSTHAN_tk",    // 15
        "GREATERTHAN_tk", // 16
        "LESSEQUAL_tk",   // 17
        "GREATEREQUAL_tk",// 18
        "EQUALEQUAL_tk",  // 19
        "COLON_tk",   // 20
        "PLUS_tk",    // 21
        "MINUS_tk",   // 22
        "MULTIPLY_tk",// 23
        "DIVIDE_tk",  // 24
        "MODULUS_tk", // 25
        "PERIOD_tk",  // 26
        "LEFTPAR_tk", // 27
        "RIGHTPAR_tk",// 28
        "COMMA_tk",   // 29
        "LEFTCURLY_tk",   // 30
        "RIGHTCURLY_tk",  // 31
        "SEMICOLON_tk",   // 32
        "LEFTBRACKET_tk", // 33
        "RIGHTBRACKET_tk",// 34
        "EOF_tk"      // 35
    };

// getNode function defined here to avoid what I think is a
// recursive  multiple inclusion issue with node.h
node* getNode(std::string label) {
    node *temp = new node();
    temp->label = label;
    temp->c0 = temp->c2 = temp->c2 = temp->c3 = NULL;
    return temp;
}

// auxiliary function to begin the recursive descent and return root pointer
node* parser(ifstream& inFile) {
    // create the root of the parse tree
    node* root;
    // get the first token from the scanner
    t = scanner(inFile);
    // call the first nonterminal's function
    root = program(inFile);
    
    return root;
}

// error() accepts the filestream, a token, and a string representing the
// expected token. it uses this information to print a detailed error 
// message and closes the filestream before exiting the program.
void error(ifstream& inFile, token t, string expected) {
    inFile.close();
    cout << "parser error: line " << t.lineNumber <<
            ": received " << tokenNames[t.ID];
    cout << " \"" << t.tokenInstance << "\", expected " << expected << ".\n";
    exit(EXIT_FAILURE);
}

// printToken used only for testing
void printToken() {
    cout << "Current token: " << tokenNames[t.ID] <<
            " , token instance " << t.tokenInstance << "\n";
}

// below is the structure of our recursive descent parser.
// each function represents a nonterminal of the same name.
// each function accepts an UNCONSUMED token, so it must call
// scanner() to see the token it should expect.

node* program(ifstream& inFile) {
    node* p = getNode("program");
    p->c0 = vars(inFile);
    p->c1 = block(inFile);
    if (t.ID == EOF_tk) {
        cout << "parse OK\n";
    }
    else {
        error(inFile, t, tokenNames[EOF_tk]);
    }
    return p;
}

node* block(ifstream& inFile) {
    node* p = getNode("block");
    if (t.ID == START_tk) {
        t = scanner(inFile);
        p->c0 = vars(inFile);
        p->c1 = stats(inFile);
        if (t.ID == STOP_tk) {
            t = scanner(inFile); // got expected STOP_tk token, consume it
            return p; // <block> grammar correct
        }
        else {
            // didn't see expected STOP token
            error(inFile, t, "STOP_tk \nWARNING: blank line required at"
                    " end of program");
        }
    }
    else {
        // didn't see expected START token
        error(inFile, t, tokenNames[START_tk]);
    }
}

node* vars(ifstream& inFile) {
    if (t.ID == VAR_tk) {
        node* p = getNode("vars");
        t = scanner(inFile);
        if (t.ID == ID_tk) {
            p->t0 = t;
            t = scanner(inFile);
            if (t.ID == COLON_tk) {
                t = scanner(inFile);
                if (t.ID == NUM_tk) {
                    p->t1 = t;
                    t = scanner(inFile);
                    p->c0 = vars(inFile);
                    return p; // grammar for this <vars> production is correct
                }
                else {
                    error(inFile, t, tokenNames[NUM_tk]);
                }
            }
            else {
                error(inFile, t, tokenNames[COLON_tk]);
            }
        }
        else {
            error(inFile, t, tokenNames[ID_tk]);
        }
    }
    else {
        // predict empty production for <vars>
        return NULL;
    }
}

node* expr(ifstream& inFile) {
    node* p = getNode("expr");
    p->c0 = A(inFile);
    if (t.ID == PLUS_tk) {
        p->t0 = t;
        t = scanner(inFile);
        p->c1 = expr(inFile);
        return p;
    }
    else {
        return p; // predict end of <expr>
    }
}

node* A(ifstream& inFile) {
    node* p = getNode("A");
    p->c0 = N(inFile);
    if (t.ID == MINUS_tk) {
        p->t0 = t;
        t = scanner(inFile);
        p->c1 = A(inFile);
        return p;
    }
    else {
        return p; // predict end of <A>
    }
}

node* N(ifstream& inFile) {
    node* p = getNode("N");
    p->c1 = M(inFile);
    if (t.ID == DIVIDE_tk) {
        p->t0 = t;
        t = scanner(inFile);
        p->c2 = N(inFile);
        return p;
    }
    else if (t.ID == MULTIPLY_tk) {
        p->t0 = t;
        t = scanner(inFile);
        p->c2 = N(inFile);
        return p;
    }
    else {
        return p; // predict end of <N>
    }
}

node* M(ifstream& inFile) {
    node* p = getNode("M");
    if (t.ID == MINUS_tk) {
        p->t0 = t;
        t = scanner(inFile);
        p->c0 = M(inFile);
        return p;
    }
    else {
        p->c0 = R(inFile);
        return p;
    }
}

node* R(ifstream& inFile) {
    node* p = getNode("R");
    if (t.ID == LEFTBRACKET_tk){
        p->t0 = t;
        t = scanner(inFile);
        p->c0 = expr(inFile);
        if (t.ID == RIGHTBRACKET_tk) {
            p->t1 = t;
            t = scanner(inFile);
            return p;
        }
        else {
            error(inFile, t, tokenNames[RIGHTBRACKET_tk]);
        }
    }
    else if (t.ID == ID_tk) {
        p->t0 = t;
        t = scanner(inFile);
        return p;
    }
    else if (t.ID == NUM_tk) {
        p->t0 = t;
        t = scanner(inFile);
        return p;
    }
    else {
        error(inFile, t, "ID_tk, or NUM_tk");
    }
}

node* stats(ifstream& inFile) {
    node* p = getNode("stats");
    p->c0 = stat(inFile);
    if (t.ID == SEMICOLON_tk) {
        t = scanner(inFile);
        p->c1 = mStat(inFile);
        return p;
    }
    else {
        error(inFile, t, tokenNames[SEMICOLON_tk]);
    }
}

node* mStat(ifstream& inFile) {
    // check if the next token is a production of <stat>
    if (t.ID == IN_tk ||
            t.ID == OUT_tk ||
            t.ID == START_tk ||
            t.ID == COND_tk ||
            t.ID == ITERATE_tk ||
            t.ID == ID_tk) {
        // if the next token is a production of <stat>, 
        // predict <stat> ; <mStat> production for <mStat>
        node* p = getNode("mStat");
        p->c0 = stat(inFile);
        if (t.ID == SEMICOLON_tk) {
            t = scanner(inFile);
            p->c1 = mStat(inFile);
            return p;
        }
        else {
            error(inFile, t, tokenNames[SEMICOLON_tk]);
        }
    }
    else {
        return NULL; // predict empty production for <mStat>
    }
}

node* stat(ifstream& inFile) {
    node* p = getNode("stat");
    if (t.ID == IN_tk) {
        p->c0 = in(inFile);
        return p;
    }
    else if (t.ID == OUT_tk) {
        p->c0 = out(inFile);
        return p;
    }
    else if (t.ID == START_tk) {
        p->c0 = block(inFile);
        return p;
    }
    else if (t.ID == COND_tk) {
        p->c0 = if_stat(inFile);
        return p;
    }
    else if (t.ID == ITERATE_tk) {
        p->c0 = loop(inFile);
        return p;
    }
    else if (t.ID == ID_tk) {
        p->c0 = assign(inFile);
        return p;
    }
    else {
        error(inFile, t, " IN_tk, OUT_tk, START_tk, COND_tk,"
        " ITERATE_tk, or ID_tk");
    }
}

node* in(ifstream& inFile) {
    if (t.ID == IN_tk) {
        t = scanner(inFile);
        if (t.ID == ID_tk) {
            node* p = getNode("in");
            p->t0 = t;
            t = scanner(inFile);
            return p;
        }
        else {
            error(inFile, t, tokenNames[ID_tk]);
        }
    }
    else {
        error(inFile, t, tokenNames[IN_tk]);
    }
}

node* out(ifstream& inFile) {
    if (t.ID == OUT_tk) {
        node* p = getNode("out");
        t = scanner(inFile);
        p->c0 = expr(inFile);
        return p;
    }
    else {
        error(inFile, t, tokenNames[OUT_tk]);
    }
}

node* if_stat(ifstream& inFile) {
    if (t.ID == COND_tk) {
        node* p = getNode("if");
        t = scanner(inFile);
        if (t.ID == LEFTPAR_tk) {
            p->t0 = t;
            t = scanner(inFile);
            if (t.ID == LEFTPAR_tk) {
                p->t1 = t;
                t = scanner(inFile);
                p->c0 = expr(inFile);
                p->c1 = RO(inFile);
                p->c2 = expr(inFile);
                if (t.ID == RIGHTPAR_tk) {
                    p->t2 = t;
                    t = scanner(inFile);
                    if (t.ID == RIGHTPAR_tk) {
                        p->t3 = t;
                        t = scanner(inFile);
                        p->c3 = stat(inFile);
                        return p;
                    }
                    else {
                        error(inFile, t, tokenNames[RIGHTPAR_tk]);
                    }
                }
                else {
                    error(inFile, t, tokenNames[RIGHTPAR_tk]);
                }
            }
            else {
                error(inFile, t, tokenNames[LEFTPAR_tk]);
            }
        }
        else {
            error(inFile, t, tokenNames[LEFTPAR_tk]);
        }
    }
    else {
        error(inFile, t, tokenNames[COND_tk]);
    }
}

node* loop(ifstream& inFile) {
    if (t.ID == ITERATE_tk) {
        node* p = getNode("loop");
        t = scanner(inFile);
        if (t.ID == LEFTPAR_tk) {
            p->t0 = t;
            t = scanner(inFile);
            if (t.ID == LEFTPAR_tk) {
                p->t1 = t;
                t = scanner(inFile);
                p->c0 = expr(inFile);
                p->c1 = RO(inFile);
                p->c2 = expr(inFile);
                if (t.ID == RIGHTPAR_tk) {
                    p->t2 = t;
                    t = scanner(inFile);
                    if (t.ID == RIGHTPAR_tk) {
                        p->t3 = t;
                        t = scanner(inFile);
                        p->c3 = stat(inFile);
                        return p;
                    }
                    else {
                        error(inFile, t, tokenNames[RIGHTPAR_tk]);
                    }
                }
                else {
                    error(inFile, t, tokenNames[RIGHTPAR_tk]);
                }
            }
            else {
                error(inFile, t, tokenNames[LEFTPAR_tk]);
            }
        }
        else {
            error(inFile, t, tokenNames[LEFTPAR_tk]);
        }
    }
    else {
        error(inFile, t, tokenNames[ITERATE_tk]);
    }
}

node* assign(ifstream& inFile) {
    if (t.ID == ID_tk) {
        node* p = getNode("assign");
        p->t0 = t;
        t = scanner(inFile);
        if (t.ID == LESSTHAN_tk) {
            p->t1 = t;
            t = scanner(inFile);
            if (t.ID == LESSTHAN_tk) {
                p->t2 = t;
                t = scanner(inFile);
                p->c0 = expr(inFile);
                return p;
            }
            else {
                error(inFile, t, tokenNames[LESSTHAN_tk]);
            }
        }
        else {
            error(inFile, t, tokenNames[LESSTHAN_tk]);
        }
    }
    else {
        error(inFile, t, tokenNames[ID_tk]);
    }
}

node* RO(ifstream& inFile) {
    node* p = getNode("RO");
    if (t.ID == LESSTHAN_tk) {
        p->t0 = t;
        t = scanner(inFile);
        if (t.ID == LESSTHAN_tk) { // detected "< <"
            p->t1 = t;
            t = scanner(inFile);
            return p;
        }
        else if (t.ID == GREATERTHAN_tk) { // detected "< >"
            p->t1 = t;
            t = scanner(inFile);
            return p;
        }
        else { // detected "<"
            return p;
        }
    }
    else if (t.ID == GREATERTHAN_tk) {
        p->t0 = t;
        t = scanner(inFile);
        if (t.ID == GREATERTHAN_tk) { // detected "> >"
            p->t1 = t;
            t = scanner(inFile);
            return p;
        }
        else { // detected ">"
            return p;
        }
    }
    else if (t.ID == EQUAL_tk) {
        p->t0 = t;
        t = scanner(inFile);
        return p;
    }
    else {
        error(inFile, t, "LESSTHAN_tk, GREATERTHAN_tk, or EQUAL_tk");
    }

}
