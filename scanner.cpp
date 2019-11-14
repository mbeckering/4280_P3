/* 
 * File:   scanner.cpp
 * Author: Michael Beckering
 * Project 2
 * CS-4280-001-FS2019
 * 
 * Created on October 14, 2019, 12:58 PM
 * Last edited: October 22
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "scanner.h"

using namespace std;

string keywords[] = {
    "", "",
    "start", "stop", "iterate", "void", "var",
    "return", "in", "out", "program", "cond",
    "then", "let"
};

string math[] = {\
    "", "", "", "", "", "", "",
    "", "", "", "", "", "", "",
    "=", "<", ">", "<=", ">=", "==", ":", "+",
    "-", "*", "/", "%", ".", "(", ")", ",",
    "{", "}", ";", "[", "]"
};

token scanner(ifstream& inFile) {
    static string line = "";
    static int i = 0; // line position index
    static int lineNumber = 0;
    static bool firstScan = 1;
    
    bool seenGraph = false; // flag to help eliminate leading whitespace
    bool foundValidToken = false; // flag to help find invalid tokens
    string word = "";
    string letter = "";
    token t;
    
    // get a new line if index has reached end of this line, or if first call
    if (i > line.length() || firstScan == true) {
        getline(inFile, line);
        // if end of file has been reached, return the eof token
        if (inFile.eof()) {
            t.ID = EOF_tk;
            t.tokenInstance = "EOF";
            t.lineNumber = lineNumber;
            return t;
        }
        // reset static variables if newline is not eof
        firstScan = false;
        lineNumber++;
        i = 0;
    }

    // iterate over this line to get the next "word"
    for (i; i <= line.length(); i++){
        letter = line[i];
        // skip any whitespace at the start of this "word"
        if (letter == " " && seenGraph == false) {
            continue;
        }
        // get new line upon seeing comment
        if (letter == "#") {
            getline(inFile, line);
            // check for EoF in new line, return eof token if found
            if (inFile.eof()) {
                t.ID = EOF_tk;
                t.tokenInstance = "EOF";
                t.lineNumber = lineNumber;
                return t;
            }
            // reset static vars and restart the loop if no eof is found
            lineNumber++;
            i = -1; // index will increment upon continue
            continue;
        }
        // if char has a graphic represenation, add it to the word
        if (isgraph(line[i])) {
            letter = line[i];
            word = word + letter;
            seenGraph = true;
        }
        // otherwise it's whitespace
        else {
            // if a word has been recorded, break and identify it
            if (word != "") {
                i++;
                break;
            }
            // else it's whitespace at the end of a line, get a new line
            getline(inFile, line);
            // check for EoF in new line, return eof token if found
            if (inFile.eof()) {
                t.ID = EOF_tk;
                t.tokenInstance = "EOF";
                t.lineNumber = lineNumber;
                return t;
            }
            // reset static vars and restart the loop if no eof is found
            lineNumber++;
            i = -1; // index will increment upon continue
            continue;
        }
    }
    
    // identify the "word"...
    // if it starts with lowercase, it's an ID token unless it matches a keyword
    if (islower(word[0])) {
        t.ID = ID_tk;
        // check against array of keywords, set KW_tk if match is found
        for (int j=0; j < sizeof(keywords)/sizeof(keywords[0]); j++) {
            if (word == keywords[j]) {
                t.ID = static_cast<tokenID>(j);
                break;
            }
        }
    }
    // else if the "word" starts with uppercase, must be an identifier
    else if (isupper(word[0])) {
        t.ID = ID_tk;
    }
    // else if the "word" starts with a digit, it's a number token
    else if (isdigit(word[0])) {
        t.ID = NUM_tk;
    }
    // otherwise, assuming only valid input, it's an operator or delimiter
    else {
        for (int j=0; j < sizeof(math)/sizeof(math[0]); j++) {
            if (word == math[j]) {
                foundValidToken = true;
                t.ID = static_cast<tokenID>(j);
                break;
            }
        }
        // if a valid token was not found in the group of operators and
        // delimiters, then the "word" is not a valid token...
        if (foundValidToken == false) {
            cout << "scanner error: line " << lineNumber 
                    << ": invalid token \"" << word << "\"\n";
            inFile.close();
            exit(EXIT_FAILURE);
        }
    }
    // assign token values and return token
    t.tokenInstance = word;
    t.lineNumber = lineNumber;
    return t;

}
