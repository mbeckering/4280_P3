/* 
 * File:   node.h
 * Author: Michael Beckering
 * Project 4
 * CS-4280-001-FS2019
 *
 * Created on November 10, 2019, 2:06 PM
 */

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "token.h"

// data structure for the each node of the parse tree
struct node {
    std::string label;
    token t0, t1, t2, t3; // token0, token1, etc
    struct node *c0, *c1, *c2, *c3; // child0, chil1, etc
};

#endif /* NODE_H */

