/*
 * Name: Bananannn
 */
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <utility>
#include "ActorGraph.h"
#include <unordered_map>
#include <unordered_set>
#include <utility>


/*
 * Function name: actorconnections()
 * Function prototype: int main(int argc, char** argv );
 * Description: This function will find number of unique neighbors
 * for each actor/actress. The movies are edges, and the actors
 * are nodes.
 */
int main( int argc, char** argv ){
    if( argc < 4 || argc > 5){
        std::cout<<"not enough arguments"<<std::endl;
        return 0;
    }

    // get all the input arguments
    std::string movieFile = argv[1];
    std::string pairs = argv[2];
    std::string output = argv[3];
    std::string flag;
    if( argc == 5 ){
        flag = argv[4];
    }
    ActorGraph myGraph; 
    bool algorithm = false;
    if( flag == "bfs" ){
        algorithm = true;
    }

    // create all the storages needed
    std::vector<std::string> act;
    std::vector<std::pair<std::string, int>> mov;

    // load the graph to get all the actors and movies
    myGraph.loadFromFile(movieFile, algorithm, act, mov);


    std::vector<ActorNode*> graph;
    std::vector<std::string> unrepactor;
    std::vector<std::pair<int, int>> actors;

    // create the graph based on the provided vectors
    myGraph.emptyGraph(graph, unrepactor, act, mov); // Different way
    
    // Start editing the file with pairs of actors
    std::ifstream infile(pairs);
    //std::getline(infile, next);
    
    // for the header
    bool have_header = false;
    while (infile) {
        std::string name;
        // get the next line
        if(!getline( infile, name )) break;
        // check the header
        if( !have_header ){
            have_header = true;
            continue;
        }
        std::istringstream ss(name);
        std::vector <std::string> thepair;
        while(ss){
            std::string next;
            if(!getline(ss, next, '\t' )) break;
            thepair.push_back(next);
        }

        // we should have exactly 2 columns
        if( thepair.size() != 2 ){
            continue;
        }
        
        // convert the string to int to get the pairs
        // represented by integers
        int converted = 0;
        std::pair<int, int> temp;
        for( int i = 0; i<unrepactor.size(); i++){
            //std::cout<<"unrepactor "<<unrepactor[i]<<std::endl;
            if(thepair[0] == unrepactor[i]){
                temp.first = i;
                converted += 1;
            } 
            if(thepair[1] == unrepactor[i]){
                temp.second = i;
                converted += 1;
            }
            // if we successfully convert two strings
            if( converted == 2 ){
                //std::cout<<"converted"<<std::endl;
                actors.push_back(temp);
                converted = 0;
                break;
            } 
        }
    }
    
    // traverse through the graph, then print in the output file
    std::ofstream outstream;
    outstream.open(output);
    //std::string buffer = "Actor1";
    outstream << "Actor1";
    outstream << '\t';
    outstream << "Actor2";
    outstream << '\t';
    outstream << "Year";

    // Build the int array to store data related to Union and find
    int totalactors = unrepactor.size();
    int parents[totalactors];
    for( int i = 0; i<totalactors; i++ ){
        parents[i] = -1;
    }
    vector<int> storeyear;
    unordered_map<int, unordered_set<string>> sorted;
    myGraph.ActorGraph::sortMovie( sorted, mov );
    int currentyear = myGraph.early;
    // find the shortest path for each of the actor pairs
    // if we need to use the BFS
    if( algorithm == true ){
        //while( current != 2015 ){
        for( int i = 0; i<actors.size(); i++ ){
            // until the path has been made, continue 
            while( myGraph.BFSTraverse(graph, actors[i].second, 
            actors[i].first, true) == true ){
            //while( currentyear != 2015 ){
                //myGraph.clearNode(graph);
            //int finalyear = 0;
                myGraph.updateNode(currentyear, 
                sorted, actors[i].first, actors[i].second, graph, unrepactor);
                
                //graph[actor[i].first]
                currentyear++;          
            }
            
            storeyear.push_back(currentyear);
            myGraph.clearNode(graph);
                                          
        }
    }
    else{
        std::cout<<"using the UnionFind"<<std::endl;
        // if the method we want to use is ufind
        for( int i=0; i<actors.size(); i++){
            int linkyear = myGraph.early;
            // only when both find function return the same value,
            // can we make sure that the two actors are connected
            while( myGraph.ActorGraph::find(actors[i].first, parents) != 
            myGraph.ActorGraph::find(actors[i].second, parents) ){
                // then, keep updating the graph by iterating 
                // each movie based on sequence of their years
                myGraph.UFupdate(linkyear, sorted, actors[i].first,
                actors[i].second,parents, unrepactor);
            
                if( myGraph.ActorGraph::find(actors[i].first, parents) != 
                myGraph.ActorGraph::find(actors[i].second, parents) ){
                 
                    // update the next year's movies
                    linkyear++;
                }
                
            }
            storeyear.push_back(linkyear);
            for(int j = 0; j<unrepactor.size(); j++){
                parents[j] = -1;
            }
        }
        
    }

    // output the result
    for( int i = 0; i<actors.size(); i++){
        outstream << '\n';
        outstream << unrepactor[actors[i].first];
        outstream << '\t';
        outstream << unrepactor[actors[i].second];
        outstream << '\t';
        outstream << storeyear[i];
    }
    outstream << '\n';
}
        
           
        
        
