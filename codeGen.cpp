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

char *temp0;

// variables for generating unique temp var names and labels
static int labelNum = 0;
static int varNum = 0;
typedef enum {VAR, LABEL} nameType;
static char Name[100];

// storage for the var and label names, used for memory allocation
string tempVarNames[100];

static string newName(nameType nametype) {
    if (nametype == VAR) {
        sprintf(Name, "tempVar%d", varNum++);
        tempVarNames[varNum - 1] = string(Name);
    }
    else if (nametype == LABEL) {
        sprintf(Name, "tempLabel%d", labelNum++);
    }
    string returnval = string(Name);
    return returnval;
}

void storageAlloc(FILE* out) {
    for(int i=0; i<varNum; i++) {
        fprintf(out, "%s 0\n", tempVarNames[i].c_str());
    }
    
    for(int i=0; i<100; i++){
        if (STV.tokens[i].ID == ID_tk && STV.tokens[i].lineNumber != 0)
            fprintf(out, "%s 0\n", STV.tokens[i].tokenInstance.c_str());
    }
    
}

//TODO ENSURE ALL EXPLICIT RETURNS!
void codeGen(node* root, FILE* out){
    if (root == NULL) {
        return;
    }
    if (root->label == "program") {
        codeGen(root->c0, out);
        codeGen(root->c1, out);
        fprintf(out, "STOP\n");
        storageAlloc(out);
        return;
    }
    else if (root->label == "block") {
        codeGen(root->c0, out);
        codeGen(root->c1, out);
        return;
    }
    else if (root->label == "vars") {
        if (root->t0.lineNumber != 0) {
            fprintf(out, "LOAD %s\n", root->t1.tokenInstance.c_str());
            fprintf(out, "STORE %s\n", root->t0.tokenInstance.c_str());
            codeGen(root->c0, out);
            return;
        }
        // else this is an empty vars node, continue down to call children
    }
    else if (root->label == "expr") {
        // if t0 is uninitialized, this node has only <A> and generates no code
        if (root->t0.lineNumber == 0) {
            codeGen(root->c0, out);
            return;
        }
        // else produce code for c0 <A> + c1 <expr>
        else {
            codeGen(root->c1, out);
            string temp = newName(VAR);
            fprintf(out, "STORE %s\n", temp.c_str());
            codeGen(root->c0, out);
            fprintf(out, "ADD %s\n", temp.c_str());
            return;
        }
    }
    else if (root->label == "A") {
        // if t0 is uninitialized, this node has only 
        // the <N> child and generates no code
        if (root->t0.lineNumber == 0) {
            codeGen(root->c0, out);
            return;
        }
        // else produce code for c0 <N> - c1 <A>
        else {
            codeGen(root->c1, out);
            string temp = newName(VAR);
            fprintf(out, "STORE %s\n", temp.c_str());
            codeGen(root->c0, out);
            fprintf(out, "SUB %s\n", temp.c_str());
            return;
        }
    }
    else if (root->label == "N") {
        // if t0 is uninitialized, this node has only 
        // the <M> child and generates no code
        if (root->t0.lineNumber == 0) {
            codeGen(root->c0, out);
            return;
        }
        // else it's division or multiplication
        else {
            string temp = newName(VAR);
            codeGen(root->c1, out);
            fprintf(out, "STORE %s\n", temp.c_str());
            codeGen(root->c0, out);
            if (root->t0.ID == DIVIDE_tk)
                fprintf(out, "DIV %s\n", temp.c_str());
            else
                fprintf(out, "MULT %s\n", temp.c_str());
            return;
        }
    }
    else if (root->label == "M") {
        // if t0 is uninitialized, this node has only 
        // the <R> child and generates no code
        if (root->t0.lineNumber == 0) {
            codeGen(root->c0, out);
            return;
        }
        // else NEGATION: call child and multiply it by -1
        else {
            codeGen(root->c0, out);
            fprintf(out, "MULT -1\n");
            return;
        }
    }
    else if (root->label == "R") {
        // if the first token is a bracket, we're looking at [<expr>] production
        if (root->t0.ID == LEFTBRACKET_tk) {
            codeGen(root->c0, out);
            return;
        }
        // else this is an ID token or integer token
        // load the token's instance in either case (no children to call)
        else {
            fprintf(out, "LOAD %s\n", root->t0.tokenInstance.c_str());
            return;
        }
    }
    else if (root->label == "stats") {
        codeGen(root->c0, out);
        codeGen(root->c1, out);
    }
    else if (root->label == "mStat") {
        // if the first token is initialized, <stat> ; <mStat> production
        if (root->t0.lineNumber != 0) {
            codeGen(root->c0, out);
            codeGen(root->c1, out);
            return;
        }
        return; // otherwise empty production of <mStat>
    }
    else if (root->label == "stat") {
        codeGen(root->c0, out);
        return;
    }
    else if (root->label == "in") {
        fprintf(out, "READ %s\n", root->t0.tokenInstance.c_str());
        return;
    }
    else if (root->label == "out") {
        string temp = newName(VAR);
        codeGen(root->c0, out);
        fprintf(out, "STORE %s\n", temp.c_str());
        fprintf(out, "WRITE %s\n", temp.c_str());
        return;
    }
    else if (root->label == "if") {
        string temp = newName(VAR);
        codeGen(root->c2, out);
        fprintf(out, "STORE %s\n", temp.c_str());
        codeGen(root->c0, out);
        fprintf(out, "SUB %s\n", temp.c_str());
        // here we've performed [arg1 - arg2], result is in ACC
        string templabel = newName(LABEL);
        // determine <RO> operator: call <RO> child and let it print BR????
        codeGen(root->c1, out);
        // finish <RO>'s ASM statement with the label
        fprintf(out, " %s\n", templabel.c_str());
        // call the child holding the <stat> to execute depending on conditional
        codeGen(root->c3, out);
        fprintf(out, "%s: NOOP\n", templabel.c_str());
        return;
    }
    else if (root->label == "loop") {
        // set start point of the loop
        string entryPoint = newName(LABEL);
        fprintf(out, "%s: NOOP\n", entryPoint.c_str());
        // check the conditional
        string temp = newName(VAR);
        codeGen(root->c2, out);
        fprintf(out, "STORE %s\n", temp.c_str());
        codeGen(root->c0, out);
        fprintf(out, "SUB %s\n", temp.c_str());
        // here we've performed [arg1 - arg2], result is in ACC
        string exitPoint = newName(LABEL);
        // determine <RO> operator: call <RO> child and let it print BR????
        codeGen(root->c1, out);
        // finish <RO>'s ASM statement with label
        fprintf(out, " %s\n", exitPoint.c_str());
        // call the child holding the <stat> to execute depending on conditional
        codeGen(root->c3, out);
        // return to top of the loop
        fprintf(out, "BR %s\n", entryPoint.c_str());
        // exit point
        fprintf(out, "%s: NOOP\n", exitPoint.c_str());
        return;
    }
    else if (root->label == "assign") {
        codeGen(root->c0, out);
        fprintf(out, "STORE %s\n", root->t0.tokenInstance.c_str());
        return;
    }
    else if (root->label == "RO") {
        // EQUAL TO operator
        if (root->t0.ID == EQUAL_tk){
            //fprintf(out, "BRZERO"); // OPPOSITE TODO
            // make 2 labels in cond/iterate nodes?
        }
        // LESS THAN operator
        else if (root->t0.ID == LESSTHAN_tk && root->t1.lineNumber == 0) {
            fprintf(out, "BRZPOS");
        }
        // LESS THAN EQUAL TO operator
        else if (root->t0.ID == LESSTHAN_tk && root->t1.ID == LESSTHAN_tk) {
            fprintf(out, "BRPOS");
        }
        // GREATER THAN operator
        else if (root->t0.ID == GREATERTHAN_tk && root->t1.lineNumber == 0) {
            fprintf(out, "BRZNEG");
        }
        // GREATER THAN EQUAL TO operator
        else if (root->t0.ID == GREATERTHAN_tk && root->t1.ID == GREATERTHAN_tk) {
            fprintf(out, "BRNEG");
        }
        // NOT EQUAL operator
        else if (root->t0.ID == LESSTHAN_tk && root->t1.ID == GREATERTHAN_tk) {
            fprintf(out, "BRZERO");
        }
        return;
    }
    // other node types don't generate code, just call children left to right
    /*
    else {
        codeGen(root->c0, out);
        codeGen(root->c1, out);
        codeGen(root->c2, out);
        codeGen(root->c3, out);
    }
    */
    return;
}