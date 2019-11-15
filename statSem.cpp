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

void error(token, string);

// symbol table container, stores full tokens for detailed error reporting
struct ST{
    token tokens[100];
    void insert(token t) {
        cout << "inserting " + t.tokenInstance + "\n";
        // if this variable already exists in this scope, error
        if (verify(t) == true) {
            error(t, "multiple");
        }
        // traverse the token array
        for(int i = 0; i <= 99; i++) {
            // if this token is uninitialized, insert here
            if (tokens[i].lineNumber == 0) {
                tokens[i].ID = t.ID;
                tokens[i].lineNumber = t.lineNumber;
                tokens[i].tokenInstance = t.tokenInstance;
                break;
            }
        }
    }
    bool verify(token t){
        for(int i = 0; i <= 99; i++) {
            // if token instance matches a stored instance, return true
            if (t.tokenInstance == tokens[i].tokenInstance) {
                cout << "verified " + t.tokenInstance + " exists in stack\n";
                return true;
            }
            // if an uninitialized token is reached, return false to
            // avoid traversing the entire array pointlessly
            else if (tokens[i].lineNumber == 0) {
                return false;
            }
        }
        return false;
    }
};

// instantiate symbol table for variables
ST STV;

void statSem(node* root, int depth) {
    if (root == NULL) {
        return;
    }
    
    if (root->label == "block") {
        cout << "ENTER block\n";
    }
    
    // check if entering a <vars> node
    if (root->label == "vars") {
        cout << "<vars> node\n";
        // check for ID token stored in t0 through t3
        if ((root->t0.lineNumber != 0) && (root->t0.ID == ID_tk)) {
            STV.insert(root->t0);
        }
        if ((root->t1.lineNumber != 0) && (root->t1.ID == ID_tk)) {
            STV.insert(root->t1);
        }
        if ((root->t2.lineNumber != 0) && (root->t2.ID == ID_tk)) {
            STV.insert(root->t2);
        }
        if ((root->t3.lineNumber != 0) && (root->t3.ID == ID_tk)) {
            STV.insert(root->t3);
        }
    }
    // else not in a <vars> node, must verify any variables seen
    else {
        // for each initialized ID token in node, verify previous declaration
        if ((root->t0.lineNumber != 0) && (root->t0.ID == ID_tk)) {
            if (STV.verify(root->t0) == false) {
                error(root->t0, "undeclared");
            }
        }
        if ((root->t1.lineNumber != 0) && (root->t1.ID == ID_tk)) {
            if (STV.verify(root->t1) == false) {
                error(root->t1, "undeclared");
            }
        }
        if ((root->t2.lineNumber != 0) && (root->t2.ID == ID_tk)) {
            if (STV.verify(root->t2) == false) {
                error(root->t2, "undeclared");
            }
        }
        if ((root->t3.lineNumber != 0) && (root->t3.ID == ID_tk)) {
            if (STV.verify(root->t3) == false){
                error(root->t3, "undeclared");
            }
        }
    }
    
    statSem(root->c0, depth+1);
    statSem(root->c1, depth+1);
    statSem(root->c2, depth+1);
    statSem(root->c3, depth+1);
    
    if (root->label == "block") {
        cout << "EXIT block\n";
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
