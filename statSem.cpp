/* 
 * File:   statSem.cpp
 * Author: Michael Beckering
 * Project 3
 * CS-4280-001-FS2019
 * 
 * Created on November 14, 2019, 9:17 AM
 */

#include "statSem.h"
#include "token.h"

using namespace std;

// error() accepts the token for detailed reporting, and a string indicating
// the type of error: "multiple" or "undeclared"
void error(token, string);

// symbol table container, stores full tokens for detailed error reporting
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
            cout << "Error: Stack overflow (exceeded maximum token count)\n";
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
        cout << "[top->] [";
        for (int i=tos; i>0; i--) {
            cout << this->tokens[i].tokenInstance << "] [";
        }
        cout << "<-bottom]\n";
    }
};

int blockNum = 0; // first block entered = block 1
int varCount[100]; // block[1] holds # of variables declared in block 1

// instantiate symbol table for variables
ST STV;

// global variables
bool beforeFirstBlock = true;

void statSem(node* root, int depth) {
    if (root == NULL) {
        return;
    }
    
    if (root->label == "block") {
        if (beforeFirstBlock) {
            beforeFirstBlock = false;
            blockNum++;
            varCount[blockNum] = 0;
            //cout << "ENTER block " << blockNum << "\n";
        }
        else {
            blockNum++;
            varCount[blockNum] = 0;
            //cout << "ENTER block " << blockNum << "\n";
        }
    }
    
    // check if entering a <vars> node
    if (root->label == "vars") {
        //cout << "<vars> node\n";
        // check for ID token stored in t0 through t3
        if ((root->t0.lineNumber != 0) && (root->t0.ID == ID_tk)) {
            // if find() returns less than varCount (but not -1)...
            if ((varCount[blockNum] > 0) && (STV.find(root->t0) >= 0) 
                    && (STV.find(root->t0) < varCount[blockNum])) {
                error(root->t0, "multiple");
            }
            else {
                STV.push(root->t0);
                varCount[blockNum]++;
            }
        }
        if ((varCount[blockNum] > 0) && (root->t1.lineNumber != 0) 
                && (root->t1.ID == ID_tk)) {
            // if find() returns less than varCount (but not -1)...
            if ((STV.find(root->t1) >= 0) && (STV.find(root->t1) < varCount[blockNum])) {
                error(root->t1, "multiple");
            }
            else {
                STV.push(root->t1);
                varCount[blockNum]++;
            }
        }
        if ((varCount[blockNum] > 0) && (root->t2.lineNumber != 0) && 
                (root->t2.ID == ID_tk)) {
            // if find() returns less than varCount (but not -1)...
            if ((STV.find(root->t2) >= 0) && (STV.find(root->t2) < varCount[blockNum])) {
                error(root->t2, "multiple");
            }
            else {
                STV.push(root->t2);
                varCount[blockNum]++;
            }
        }
        if ((varCount[blockNum] > 0) && (root->t3.lineNumber != 0) && 
                (root->t3.ID == ID_tk)) {
            // if find() returns less than varCount (but not -1)...
            if ((STV.find(root->t3) >= 0) && (STV.find(root->t3) < varCount[blockNum])) {
                error(root->t3, "multiple");
            }
            else {
                STV.push(root->t3);
                varCount[blockNum]++;
            }
        }
        //STV.printStack();
    }
    // else not in a <vars> node, must verify any variables seen
    else {
        // for each initialized ID token in node...
        if ((root->t0.lineNumber != 0) && (root->t0.ID == ID_tk)) {
            // if find() returns -1, variable is undeclared
            if (STV.find(root->t0) == -1) {
                error(root->t0, "undeclared");
            }
        }
        if ((root->t1.lineNumber != 0) && (root->t1.ID == ID_tk)) {
            if (STV.find(root->t1) == -1) {
                error(root->t1, "undeclared");
            }
        }
        if ((root->t2.lineNumber != 0) && (root->t2.ID == ID_tk)) {
            if (STV.find(root->t2) == -1) {
                error(root->t2, "undeclared");
            }
        }
        if ((root->t3.lineNumber != 0) && (root->t3.ID == ID_tk)) {
            if (STV.find(root->t3) == -1){
                error(root->t3, "undeclared");
            }
        }
    }
    
    statSem(root->c0, depth+1);
    statSem(root->c1, depth+1);
    statSem(root->c2, depth+1);
    statSem(root->c3, depth+1);
    
    // if exiting a block
    if (root->label == "block") {
        //cout << "EXIT block " << blockNum << ", varCount=" << varCount[blockNum] << "\n";
        //STV.printStack();
        // pop the variables from this block
        for (int i=0; i<varCount[blockNum]; i++) {
            STV.pop();
            //STV.printStack();
        }
        // scope returns to previous block
        blockNum--;
    }
}

void error(token t, string errorType) {
    cout << "statSem error: line " << t.lineNumber << 
            ": ID_tk \"" + t.tokenInstance;
    if (errorType == "undeclared") {
        cout << "\" not declared in this scope\n";
    }
    else if (errorType == "multiple") {
        cout << "\" was already declared in this scope\n";
    }
    exit (EXIT_FAILURE);
}

