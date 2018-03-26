/*
 * ActorGraph.h
 * Author: Bananannn
 * Date:  Dec.7, 2017
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "ActorEdge.h"
#include "ActorNode.h"
// Maybe include some data structures here

using namespace std;

/*
 * class name: compare
 * Description: this class is used when adding a node into the priority
 * queue
 * The node with smaller distance will have higher priority
 */
class compare{
public:
    bool operator()(ActorNode*& one, ActorNode*& two ) const{
        return one->dist > two->dist;
    }
};

/*
 * class name: ActorGrpah
 * This class contains all the information to construct a valid graph and
 * manipulate on the graph by using the function in it
 */
class ActorGraph {
    public:
        /*struct Vertex
        {
            vector<int> adj;
            int dist = 0;
            int index = 0;
            int prev = 0;
        };*/
        // variables related to years are used in the actorconnections related
        // functions
        int early = 2015; 
        int latest = 0;
        
        // theingraph represents the graph in total
        vector<ActorNode*> ingraph;
        // the string indicates each movie name with its year
        // the unordered_set contains all the actors related to the movie
        unordered_map<string, unordered_set<string>> mymap;
        int parents[BUFSIZ];
        
        /*
         * Constructor which is used to initialize the graph
         */
        ActorGraph(void);
        // Maybe add some more methods here
        
        /**
         * Function name: BFSTraverse
         * Description: This function use BFS to traverse a graph built
         * and update the information in the nodes
         */
        bool BFSTraverse( vector<ActorNode*>& graph, int from, int to, 
             bool connection );

        /**
         * Function name: createGraph
         * Description: This function is used to create a graph and 
         * intialize all the nodes and edges in it with default value
         */
        void createGraph( vector<ActorNode*>& graph, vector<string>& unrepactor,
             vector<string>& actor, vector<pair<string, int>>& movie );
        /**
         * Function name: clearNode
         * Description: This function is used to erase all the data in the
         * node to be default ones
         */
        void clearNode( vector<ActorNode*>& graph );

        /**
         * Function name: Dijkstra
         * Description: This function travese through the graph by using
         * the Dijkstra algorithm. It also update all the information
         * in each actor node.
         */
        void Dijkstra(vector<ActorNode*> graph, int from, int to);
     
        
        /**
         * Function name: emptyGraph
         * Description: This graphs created have no edges but only
         * all the valid actor nodes
         */
        void emptyGraph(vector<ActorNode*>& graph, vector<string>& unrepactor,
             vector<string>& actor, vector<pair<string, int>>& mov );
        
        /** 
         * Load the graph from a tab-delimited file of actor->movie 
         * relationships.
         *
         * in_filename - input filename
         * use_weighted_edges - if true, compute edge weights as 1 + (2015 
         * - movie_year), otherwise all edge weights will be 1
         *
         * return true if file was loaded sucessfully, false otherwise
         */
        bool loadFromFile(std::string in_filename, bool use_weighted_edges,
             vector<string>& actor, vector<pair<string, int>>& movie);
       
        /**
         * Function name: sortMovie
         * Description: This funciton sort all the movies inside the input
         * file by their years. By using the set and map, the movies and 
         * the actors cast in them have no repeatation
         */
        void sortMovie( unordered_map<int, unordered_set<string>>& sorted, 
             vector<pair<string, int>>& mov);

        /**
         * Function name: updateNode
         * This method used to update all the nodes when the user wants to
         * use bfs to find the actor connections.
         * The graph used here only has actor nodes
         */ 
        int updateNode( int year, unordered_map<int, 
             unordered_set<string>>& sorted, int from, int to,
             vector<ActorNode*>& graph, vector<string>& unrepactor );
 
        /**
         * Function name: UFupdate
         * Description: This method also update the nodes in one graph when
         * the user chooses to use ufind to find the actor connections
         * The graph used here only has actor nodes
         */
        int UFupdate(int year, unordered_map<int, unordered_set<string>>& 
            sorted, int nodei, int nodej,int parents[],  vector<string>& 
            unrepactor);

        /**
         * Function name: Union
         * Description: This function will union to actor nodes
         */
        void Union( int nodei, int nodej, int parents[] );
        
        /**
         * Function name: Find
         * Description: This function will find the root of the node 
         * in the array
         */   
        int find(int nodei, int parents[] );

};


#endif // ACTORGRAPH_H
