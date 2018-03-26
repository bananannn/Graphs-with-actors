/*
 * Name: Bananannn
 */
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <utility>
//#include "ActorGraph.h"
#include <unordered_map>
#include <unordered_set>

/*
 * Function name: main()
 * Function prototype: int main(int argc, char** argv );
 * Description: This function will find number of unique neighbors
 * for each actor/actress. The movies are edges, and the actors
 * are nodes.
 */
int main( int argc, char** argv ){
    if( argc != 4 ){
        std::cout<<"not enough arugments"<<std::endl;
    }
    std::string movieFile = argv[1];
    std::string actorFile = argv[2];
    std::string output = argv[3];
    //ActorGraph graph;
    //graph.ActorGraph::loadFromFile(movie, false);
    bool have_header = false;

    std::ifstream infile(movieFile);
    std::vector<std::string> actor;
    //std::vector<std::pair<std::string, int>> movie;
    std::vector<std::string> movie;
    // keep reading lines until the end of file is reached
    while (infile) {
        std::string s;
        // get the next line
        if (!std::getline( infile, s )) break;
        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }
        std::istringstream ss( s );
        std::vector <std::string> record;
        while (ss) {
            std::string next;
            if (!std::getline( ss, next, '\t' )) break;
            record.push_back( next );
        }
        if( record.size() != 3){
            continue;
        }
        std::string actor_name(record[0]);
        std::string movie_title(record[1]);
        //int movie_year = stoi(record[2]);
        std::string movie_year = record[2];
        
        movie_title += movie_year; 
        actor.push_back(actor_name);
        /*std::pair<std::string,int>temp;
        temp.first = movie_title;
        temp.second = movie_year;*/
        movie.push_back(movie_title);
    }
    if( !infile.eof() ){
        std::cerr<<"Failed"<<" !\n";
    }
    
         
    /*std::string next;
    std::vector<std::string> act;
    std::vector<std::pair<std::string, int>> mov;
    std::getline(movies, next);*/
    // Organize the data in the file movie_casts.csv
    /*while( std::getline(movies, next) ){
        std::stringstream nexts(next);
        std::string name;
        int while1 = 0;
        while(std::getline(nexts, name, '\t')){
            if (while1 == 0){
                act.push_back(name);
            }
            std::pair<std::string, int> each;
            if (while1 == 1){
                each.first = name;
            }
            if (while1 == 2){
                std::stringstream geek(name);
                int year = 0;
                geek>>year;
                each.second = year;
                mov.push_back(each);
            } 
            std::cout<<"name "<<name<<std::endl;
            std::cout<< "while1 "<<while1<<std::endl;
            while1++;            
        } 
        //std::cout<<"getline "<< next<<std::endl;
    }
    */
    
    // Store the names in the second argument in a vector
    std::vector<std::string> secondAct;
    std::ifstream actors;
    actors.open(actorFile);
    std::string actorNext;
    
    // get all the lines inside
    std::getline(actors, actorNext);
    while( std::getline(actors, actorNext) ){
        secondAct.push_back(actorNext);
    }
    
    std::ofstream outstream;
    outstream.open(output);
    std::string buffer = "Number of Neighbors";
    outstream << buffer;
    //outstream.write(buffer, sizeof(buffer)); 
    std::unordered_map<std::string, std::unordered_set<std::string>> mymap;
    
    //for( int j = 0; j<secondAct.size(); j++){
    for( int i = 0; i<movie.size(); i++){
        std::unordered_map<std::string, std::unordered_set<std::string>>::
        iterator got;
        // find a certain movie
        got = mymap.find(movie[i]);
        // if there is no this movie until now, update the movie
        if( got == mymap.end() ){
            std::unordered_set<std::string> newone;
            newone.insert(actor[i]);
            std::pair<std::string, std::unordered_set<std::string>> 
            onePair(movie[i], newone);
            mymap.insert(onePair);
        }
        // if it exists, we add the actors 
        else{
            (got->second).insert(actor[i]);
        } 
            
    }
   
    // Construct the graph based on the data in the movie_casts file
    for(int i = 0; i<secondAct.size(); i++){
        std::unordered_set<std::string> neighbor;
        for( int j = 0; j<actor.size(); j++){
            if( actor[j] == secondAct[i] ){
                std::unordered_map<std::string, std::unordered_set<std::string
                >>::iterator got;
                got = mymap.find(movie[j]);
                // update each of the actor in the list
                for( const auto& ele: got->second){
                    neighbor.insert(ele);
                }
            }
        }
        // All the neighbors (exclude the actor himself)
        int neighbors = neighbor.size()-1;
        std::cout<<"neighbors: "<< neighbors<<std::endl;
        //outstream.write((char*)&neighbors, sizeof(neighbors));
        outstream << '\n';
        outstream << neighbors;
     }
     outstream << '\n';
     
} 
    
 
