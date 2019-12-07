/* 
 * File:   statSem.cpp
 * Author: Michael Beckering
 * Project 4
 * CS-4280-001-FS2019
 * 
 * Created on November 14, 2019, 9:17 AM
 */

#include "statSem.h"
#include "token.h"
#include "st.h"
#include <fstream>

using namespace std;

// error() accepts the token for detailed reporting, and a string indicating
// the type of error: "multiple" or "undeclared"
void error(token, string);

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
    // TODO delete file
    exit (EXIT_FAILURE);
}

