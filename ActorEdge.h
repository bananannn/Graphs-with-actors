/*
 * ActorEdge.h
 * Author: Bananannn
 * Date:  Dec.7, 2017
 *
 * This file contains the basic structure of each edge
 */
#ifndef ACTOREDGE_H
#define ACTOREDGE_H

#include <string>
#include <vector>
#include "ActorNode.h"

using namespace std;

class ActorNode;
class ActorEdge{
    public:
        // the variables contained by each edge
        string first;
        // used by the weighted tree
        ActorNode* node1;
        ActorNode* node2;
        string second;
        string movie;
        // used by the weighted tree
        int weight;    
        int movie_year;
        vector<ActorNode*> relatednode;
        
        /*
         * Function name: ActorEdge
         * Input: the name of the node it connects, and the movie name
         * which both actors cast in
         */ 
        ActorEdge(string onenode, string twonode, string movie);
        //bool operator<(const ActorEdge& other);         
};

#endif
