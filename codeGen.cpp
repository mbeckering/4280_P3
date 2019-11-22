/* 
 * File:   codeGen.cpp
 * Author: Michael Beckering
 * Project 4
 * CS-4280-001-FS2019
 * 
 * Created on November 22, 2019, 11:31 AM
 */

#include <iostream>
#include <stdio.h>
#include "codeGen.h"
#include "st.h"
#include "token.h"

using namespace std;

void codeGen(node* root, int depth, FILE* out){
    cout << "codeGen called\n";
    if (root == NULL) {
        return;
    }
    if (root->label == "program") {
        codeGen(root->c0, depth+1, out);
        codeGen(root->c1, depth+1, out);
        codeGen(root->c2, depth+1, out);
        codeGen(root->c3, depth+1, out);
        fprintf(out, "STOP\n");
        // TODO MEMORY ALLOCATION
    }
    else if (root->label == "vars") {
        // if t0 of this vars node is initialized, children are 
        // t0 ID_tk and t1 NUM_tk
        if (root->t0.lineNumber != 0) {
            fprintf(out, "LOAD %s\n", root->t1.tokenInstance.c_str());
            fprintf(out, "STORE %s\n", root->t0.tokenInstance.c_str());
        }
        // else this is an empty vars node, continue down to call children
    }
    // other nodes don't generate code, call children left to right
    else {
        codeGen(root->c0, depth+1, out);
        codeGen(root->c1, depth+1, out);
        codeGen(root->c2, depth+1, out);
        codeGen(root->c3, depth+1, out);
    }
    return;
}