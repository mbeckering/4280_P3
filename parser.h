/* 
 * File:   parser.h
 * Author: Michael Beckering
 * Project 2
 * CS-4280-001-FS2019
 *
 * Created on November 1, 2019, 9:55 AM
 */

#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include <string>
#include "token.h"
#include "node.h"

node* parser(std::ifstream&);
void error(std::ifstream&, token, std::string);

// recursive descent parser architecture
node* program(std::ifstream&);
node* block(std::ifstream&);
node* vars(std::ifstream&);
node* expr(std::ifstream&);
node* A(std::ifstream&);
node* N(std::ifstream&);
node* M(std::ifstream&);
node* R(std::ifstream&);
node* stats(std::ifstream&);
node* mStat(std::ifstream&);
node* stat(std::ifstream&);
node* in(std::ifstream&);
node* out(std::ifstream&);
node* if_stat(std::ifstream&);
node* loop(std::ifstream&);
node* assign(std::ifstream&);
node* RO(std::ifstream&);

#endif /* PARSER_H */
