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
 * File name: pathfinder()
 * Function prototype: int main(int argc, char** argv );
 * Description: This function will find number of unique neighbors
 * for each actor/actress. The movies are edges, and the actors
 * are nodes.
 */
int main( int argc, char** argv ){
    if( argc != 5 ){
        std::cout<<"not enough arugments"<<std::endl;
    }
    // get all the input arguments
    std::string movieFile = argv[1];
    std::string unweighted = argv[2];
    std::string pairs = argv[3];
    std::string output = argv[4];
    ActorGraph myGraph;
    // revise the second argument to indicate
    // whether we want a weighted graph or unweighted
    bool unweight = true;
    if( unweighted == "w" ){
        unweight = false;
    }
    // create all the storages needed
    std::vector<std::string> act;
    std::vector<std::pair<std::string, int>> mov;

    // load the graph to get all the actors and movies
    myGraph.loadFromFile(movieFile, unweight, act, mov);


    std::vector<ActorNode*> graph;
    std::vector<std::string> unrepactor;
    std::vector<std::pair<int, int>> actors;

    // create the graph based on the provided vectors
    myGraph.createGraph(graph, unrepactor, act, mov);
    
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
        std::cout<<"checkpoint 1"<<std::endl;
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
    std::string buffer = "(actor)--[movie#@year]-->(actor)--...";
    outstream << buffer;
    outstream << '\n';
    
    // find the shortest path for each of the actor pairs
    for( int i = 0; i<actors.size(); i++){
        // go through the graph by using the BFS
        if( unweight == true ){
            myGraph.BFSTraverse(graph,actors[i].second, actors[i].first, false);
        }
        else{
            myGraph.Dijkstra(graph, actors[i].second, actors[i].first);
        }
        
        // FIRST represents the starter node
        // SECOND represents the ending node
        ActorNode* temp = graph[actors[i].first];
        // std::cout<<"actors[i].second "<<actors[i].second<<std::endl;
        // std::cout<<"actors[i].first "<<actors[i].first<<std::endl;
        outstream << '(';
        outstream << unrepactor[temp->index];
        outstream << ')';
        int nodeindex = temp->index;
        
        /*for(int k = 0; k<(graph[1]->relatededge).size(); k++ ){
            std::cout<<graph[1]->relatededge[k]->second<<std::endl;
        }*/
        // print all the actors on the path
        while( nodeindex != actors[i].second ){
            ActorNode* current = temp;
            // find the previous node again and again
            temp = graph[temp->prev];
            nodeindex = temp->index;
            //std::cout<<"nodeindex2 "<< temp->actorname <<std::endl;
            outstream << '-';
            outstream << '-';
            outstream << '[';
            // print the movie name
            for ( int j = 0; j<(current->relatededge).size(); j++){
                if( (((current->relatededge)[j])->second) == temp->actorname ){
                    outstream << (current->relatededge)[j] -> movie;
		    break;
                }
            }
            outstream << ']';
            outstream << '-';
            outstream << '-';
            outstream << '>'; 
            outstream << '(';
            outstream << unrepactor[temp->index];
            outstream << ')';
        }
       
        outstream << '\n';
        myGraph.clearNode(graph);
    }

    // delete the graph TODO the test cases too
    for( int i = 0; i<graph.size(); i++){
        delete(graph[i]);
    }
    outstream.close();
}
         
                     
           
