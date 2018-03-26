/*
 * ActorNode.h
 * Author: Bananannn
 * Date:  Dec.7, 2017
 *
 * This file contains the basic features of each actor node
 */
#include <string>
#include <vector>
#include <limits>
#include "ActorNode.h"
using namespace std;

ActorNode::ActorNode(void){
}

/*
 * the constructor initialize the name of the node
 */
ActorNode::ActorNode(string name){
    actorname = name;
    // Set the initial distance value to be infinite
    dist = numeric_limits<int>::max();
    // the index and prev to -1 
    index = -1;
    prev = -1;
    // used by the weighted tree
    done = false;
}




