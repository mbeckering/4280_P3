/* 
 * File:   st.h
 * Author: Michael Beckering
 * Project 4
 * CS-4280-001-FS2019
 *
 * Created on November 22, 2019, 11:36 AM
 */

#ifndef ST_H
#define ST_H

#include <cstdlib>

struct ST{
    token tokens[100]; // token stack
    int tos; // top of stack
    // push function
    void push(token t) {
        //cout << "pushing " + t.tokenInstance + "\n";
        this->tos++;
        this->tokens[this->tos] = t;
        if (this->tos >= 99) {
            // if this point is reached, the program exceeds 100 tokens
            std::cout << "Error: Stack overflow (exceeded maximum token count)\n";
            exit (EXIT_FAILURE);
        }
    }
    // pop function
    void pop() {
        //cout << "popping " << this->tokens[this->tos].tokenInstance << "\n";
        this->tokens[this->tos].ID = NULL_tk;
        this->tokens[this->tos].lineNumber = 0;
        this->tokens[this->tos].tokenInstance = "";
        tos--;
    }
    // verify function searches full stack for given token
    bool verify(token t) {
        for(int i = 0; i <= 99; i++) {
            // if token instance matches a stored instance, return true
            if (t.tokenInstance == this->tokens[i].tokenInstance) {
                //cout << "verified " + t.tokenInstance + " exists in stack\n";
                return true;
            }
            // if an uninitialized token is reached, return false to
            // avoid traversing the entire array pointlessly
            else if (this->tokens[i].lineNumber == 0) {
                return false;
            }
        }
        return false;
    }
    // find() takes a token to match, returns distance from top of stack
    // returns -1 if token not found
    int find(token t) {
        // traverse stack from top to bottom
        for (int i = tos; i>=0; i--) {
            if (t.tokenInstance == this->tokens[i].tokenInstance) {
                //cout << "find() returning " << tos - i << "\n";
                return (tos - i);
            }
        }
        //cout << "find() returning " << -1 << "\n";
        return -1; // token not found on stack
    }
    // printStack() is used for testing
    void printStack() {
        std::cout << "[top->] [";
        for (int i=tos; i>0; i--) {
            std::cout << this->tokens[i].tokenInstance << "] [";
        }
        std::cout << "<-bottom]\n";
    }
};

extern ST STV; // symbol table for local variables
extern ST STG; // symbol table for global variables

#endif /* ST_H */

