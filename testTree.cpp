/* 
 * File:   testTree.cpp
 * Author: Michael Beckering
 * Project 4
 * CS-4280-001-FS2019
 * 
 * Created on November 10, 2019, 1:54 PM
 */

#include "testTree.h"
#include "node.h"
#include "token.h"
#include <string>
#include <iostream>
#include <ctype.h>
#include <stdio.h>

using namespace std;

void printPreorder(node *root, int depth){
    if (root == NULL) {
        return;
    }
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
}