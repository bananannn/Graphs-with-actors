/*
 * ActorNode.h
 * Author: Moyan Zhou
 * Date:  Dec.7, 2017
 * csLogin: cs100fdl
 *
 * This file contains the basic features of each actor node
 */
#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <string>
#include <vector>
#include <limits>
#include <unordered_set>
 using namespace  std;
class ActorEdge;

/*
 * This file contains the basic features of each actor node
 */
class ActorNode{
    // all the variables and functions needed
    public:
        string actorname;
        // store all the edges
        vector<ActorEdge *> relatededge;
        vector<int> adj;
        int dist;
        int index;
        int prev;
        bool done; 
        int linkyear = 0;
        // store all the unrepeated adj nodes represented by numbers
        unordered_set<int> adjall;     
 
        // default constructor
        ActorNode(void); 
        // the constructor initialize the name of the node
        ActorNode(string name);
};

#endif

    
