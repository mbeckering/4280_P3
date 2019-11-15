/* 
 * File:   main.cpp
 * Author: Michael Beckering
 * Project 2
 * CS-4280-001-FS2019
 *
 * Created on October 11, 2019, 3:12 PM
 * Last edited: Nov 1 2018
 * 
 * TODO: update README.txt : local or global option
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"
#include "testTree.h"
#include "node.h"
#include "statSem.h"

using namespace std;

int main(int argc, char** argv) {
    string inputFilenameStr; // name of data file WITH extension to open for sorting
    string outputFilenameStr;
    // parse command line arguments..
    // if no args, get input from stdin until eof or simulated eof
    if (argc == 1) {
        string line;
        ofstream out("input.fs19");
        cout << "Reading input from keyboard stream... (CTRL+D when finished)\n";
        while (getline(cin, line)) {
            out << line << "\n";
        }
        out.close();
        cout << "Saving keyboard input to file input.fs19...\n";
        inputFilenameStr = "input.fs19";
        outputFilenameStr = "input"; // base name for output files
    }
    // if one arg, append file extension and set output file base name
    else if (argc == 2) {
        string arg1(argv[1]);
        inputFilenameStr = arg1 + ".fs19";
        outputFilenameStr = arg1; // base name for output files
    }
    // else too many args, print error message and exit
    else {
        cout << "Error: Too many arguments.\n";
        cout << "Usage: ./P0 <filename>\n";
        cout << "Exclude file extension.\n";
        return 1;
    }
    
    // convert string to const char*
    const char *inputFilenameChar = inputFilenameStr.c_str();
    cout << "Attempting to open file " << inputFilenameStr << "...";
    ifstream in;
    in.open(inputFilenameChar);
    // if file failed to open, exit with error message
    if (!(in.is_open())) {
        cout << "Error opening file " << inputFilenameStr << "\n";
        return 1;
    }
    else {
        cout << "Opened " << inputFilenameStr << "\n";
    }
    
    // call the parser, which in turn uses the scanner to get tokens 1 by 1
    node* root = parser(in);
    in.close();
    
    // call the print function (used for testing)
    // printPreorder(root, 0);
    
    statSem(root, 0);
    cout << "main: Static Semantics OK\n";
    
    return 0;
}

