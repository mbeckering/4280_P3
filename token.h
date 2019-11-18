/* 
 * File:   token.h
 * Author: Michael Beckering
 * Project 1
 * CS-4280-001-FS2019
 *
 * Created on October 14, 2019, 12:43 PM
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum tokenID {
        ID_tk,      // 0
        NUM_tk,     // 1
        START_tk,   // 2
        STOP_tk,    // 3
        ITERATE_tk, // 4
        VOID_tk,    // 5
        VAR_tk,     // 6
        RETURN_tk,  // 7
        IN_tk,      // 8
        OUT_tk,     // 9
        PROGRAM_tk, // 10
        COND_tk,    // 11
        THEN_tk,    // 12
        LET_tk,     // 13
        EQUAL_tk,   // 14
        LESSTHAN_tk,    // 15
        GREATERTHAN_tk, // 16
        LESSEQUAL_tk,   // 17
        GREATEREQUAL_tk,// 18
        EQUALEQUAL_tk,  // 19
        COLON_tk,   // 20
        PLUS_tk,    // 21
        MINUS_tk,   // 22
        MULTIPLY_tk,// 23
        DIVIDE_tk,  // 24
        MODULUS_tk, // 25
        PERIOD_tk,  // 26
        LEFTPAR_tk, // 27
        RIGHTPAR_tk,// 28
        COMMA_tk,   // 29
        LEFTCURLY_tk,   // 30
        RIGHTCURLY_tk,  // 31
        SEMICOLON_tk,   // 32
        LEFTBRACKET_tk, // 33
        RIGHTBRACKET_tk,// 34
        EOF_tk,     // 35
        NULL_tk     // 36
    };

// data structure for each token
struct token {
    tokenID ID;
    std::string tokenInstance;
    int lineNumber;
};

#endif /* TOKEN_H */

