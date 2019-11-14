/* 
 * File:   statSem.cpp
 * Author: Michael Beckering
 * Project 3
 * CS-4280-001-FS2019
 * 
 * Created on November 14, 2019, 9:17 AM
 */

#include "statSem.h"

using namespace std;

// symbol table container, stores full tokens for detailed error reporting
struct ST{
    token tokens[100];
    void insert(token t) {
        cout << "inserting " + t.tokenInstance + "\n";
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

void error(token);

// instantiate symbol table for variables
ST STV;

void statSem(node* root, int depth) {
    if (root == NULL) {
        return;
    }
    bool check = false;
    
    if (root->label == "block") {
        cout << "entering a block\n";
    }
    
    // check if entering a <vars> node
    if (root->label == "vars") {
        cout << "entering a <vars> node\n";
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
            if (!STV.verify(root->t0)) {
                error(root->t0);
            }
        }
        if ((root->t1.lineNumber != 0) && (root->t1.ID == ID_tk)) {
            if (!STV.verify(root->t1)) {
                error(root->t1);
            }
        }
        if ((root->t2.lineNumber != 0) && (root->t2.ID == ID_tk)) {
            if (!STV.verify(root->t2)) {
                error(root->t2);
            }
        }
        if ((root->t3.lineNumber != 0) && (root->t3.ID == ID_tk)) {
            if (!STV.verify(root->t3)){
                error(root->t3);
            }
        }
    }
    
    statSem(root->c0, depth+1);
    statSem(root->c1, depth+1);
    statSem(root->c2, depth+1);
    statSem(root->c3, depth+1);
    
    if (root->label == "block") {
        cout << "exiting a block\n";
    }
    
    // below code is pasted from testTree(), used as
    // reference for development of statSem()
    /*
    string data = root->label; // string to hold node data
    
    // concatenate token data to the node data if the node holds tokens
    // if a token's line number is initialized to 0, the token is uninitialized
    if (root->t0.lineNumber != 0) {
        data = data + " \"" + root->t0.tokenInstance + "\"";
    }
    if (root->t1.lineNumber != 0) {
        data = data + " \"" + root->t1.tokenInstance + "\"";
    }
    if (root->t2.lineNumber != 0) {
        data = data + " \"" + root->t2.tokenInstance + "\"";
    }
    if (root->t3.lineNumber != 0) {
        data = data + " \"" + root->t3.tokenInstance + "\"";
    }
    
    printf("%*c ", depth*2, ' ');
    cout << data << "\n";
    
    printPreorder(root->c0, depth+1);
    printPreorder(root->c1, depth+1);
    printPreorder(root->c2, depth+1);
    printPreorder(root->c3, depth+1);
    */
}

void error(token t) {
    cout << "statSem error: \"" + t.tokenInstance + "\" not declared\n";
    exit (EXIT_FAILURE);
}
