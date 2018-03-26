/*
 * ActorEdge.h
 * Author: Bananannn
 * Date:  Dec.7, 2017
 *
 * This file contains the basic structure of each edge
 */
#include <string>
#include <vector>
#include "ActorEdge.h"

using namespace std;

/*
 * Function name: ActorEdge
 * Input: the name of the node it connects, and the movie name
 * which both actors cast in
 */ 
ActorEdge::ActorEdge(string onenode, string twonode, string mov){
    // name of those actors
    first = onenode;
    second = twonode;
    // the movie with the year
    movie = mov;
    // default weight is 1
    weight = 1;
    // default year is 0
    movie_year = 0;
}    

//ActorEdge::ActorEdge(ActorNode* onenode, ActorNode* twonode, string mov){
//    node1 = onenode;
//    node2 = twonode;
//}

/*bool ActorEdge::operator<(const ActorEdge& other){
    if( weight == other.weight ){
    }
    else{
        if( weight <other.weight ){
            return false;
        }
        return true;
    }
}*/
