/*
 * ActorGraph.cpp
 * Author: Bananannn
 * Date:   Dec. 7, 2017
 *
 * This file is meant to exist as a container for starter code that you can 
 * use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.h"
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

ActorGraph::ActorGraph() {

}

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
bool ActorGraph::loadFromFile(std::string in_filename, bool use_weighted_edges,
     std::vector<std::string>& act, std::vector<pair<std::string, int>>& mov) {
    // Initialize the file stream
    ifstream infile(in_filename);
    std::cout<<"load from file 1"<<std::endl;
    bool have_header = false;

    std::vector<std::string> actor;
    std::vector<std::pair<std::string, int>> movie;
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;

        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put it 
            // in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        // we have an actor/movie relationship, now what?
        actor.push_back(actor_name);
        std::pair<std::string, int> temp;
        temp.first = movie_title;
        temp.second = movie_year;
        movie.push_back(temp);
    }
    // Organizing the data in the input file


    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    act = actor;
    mov = movie;
    
    infile.close();

    return true;
}

/**
 * Function name: BFSTraverse
 * Description: This function use BFS to traverse a graph built
 * and update the information in the nodes
 *
 * The code is provided by the professor
 * Website: https://drive.google.com/file/d/0B2-tLcxjwLzOMEhUTVN4UjVtckU/view
 *
 */
bool ActorGraph::BFSTraverse( vector<ActorNode*>& graph, int from, int to, 
bool connection){
//std::string output, vector<std::string> unrepactor ){
    queue<ActorNode*> explore;
    ActorNode* start = graph[from];
    start->dist = 0;
    explore.push(start);
    // go through all the points that the start can reach
    while( !explore.empty() ){
        ActorNode* next = explore.front();
        explore.pop();
        
        // if the bfs is used in the actorconnections
        if( connection == true ){
            // instead of using next->adj, we use next->adjall
            unordered_set<int>::iterator got = next->adjall.begin();
            for( ; got!=next->adjall.end(); ++got ){
                ActorNode* neighbor = graph[*got];
                // update the dist and prev in the node
                if( next->dist+1 < neighbor->dist ){
                    neighbor->dist = next->dist+1;
                    neighbor->prev = next->index;
                    explore.push(neighbor);
                }
            }
        }
        else{
            // if the method is used in the pathfinder
            // we use next->adj
            vector<int>::iterator it = next->adj.begin();
            for( ; it!= next->adj.end(); ++it ){
                 ActorNode* neighbor = graph[*it];
                // update the dist and prev in the node
                if( next->dist+1 < neighbor->dist ){
                    neighbor->dist = next->dist+1;
                    neighbor->prev = next->index;
                    explore.push(neighbor);
                }
            }
        }
        /*if( next->index == to ){
            break;
        }*/
    }
    // if there is no prev node, there has an error 
    // return false 
    if( graph[from]->prev == -1 ){
        return false;
    }
    return true;
    
}

/**
 * Helper method: used everytime that we have a new pair
 */
void ActorGraph::clearNode( vector<ActorNode*>& graph ){
    for( int i = 0; i<graph.size(); i++){
        ActorNode* temp = graph[i];
        //temp->relatededge.clear();
        //temp->adj.clear();
        temp->dist = numeric_limits<int>::max();
        //temp->index = -1;
        temp->prev = -1;
        temp->done = false;
    }    
}



/**
 * Function name: Dijkstra()
 * Description: this function is used to find the shortest path between two
 * points in a connected weighted graph.
 * It is basically based on the code provided by professor in the lecture
 * and the algorithm
 * Website: https://drive.google.com/file/d/0B2-tLcxjwLzOMEhUTVN4UjVtckU/view
 */
void ActorGraph::Dijkstra(vector<ActorNode*> graph, int from, int to){
    priority_queue<ActorNode*, vector<ActorNode*>, compare>
    priQueue;
    ActorNode* start = graph[from];
    start->dist = 0;
    priQueue.push(start);
    
    while( !priQueue.empty() ){
        ActorNode* next = priQueue.top();
        priQueue.pop();
        // while PQ is not empty
        if( next->done != true ){
            next->done = true;
            // for each of next's neighbors, w
            for( auto it = next->relatededge.begin(); it != 
            next->relatededge.end(); ++it ){
                ActorNode* neighbor = (*it)->node2;
                int currentd = next->dist;
                int add = (*it)->weight;
                int ndist = currentd + add;
                // if the distance to w through v is less than w.dist
                if( ndist < neighbor->dist){
                    neighbor->dist = ndist;
                    neighbor->prev = next->index;
                    priQueue.push(neighbor);
                }
            }
        }
    }          
    
}


/**
 * Function name: createGraph
 * Function description: this function is used to create a graph related to the
 * actors and movies based on the data given by the load file function
 * Parameter: vecotr<ActorNode*>& graph -- the graph created
 *            vector<string>& unrepactor -- the vector with all the unrepeated 
 *            actors
 *            vector<string> actor -- the actors in the film
 *            vector<pair<string, int> mov -- the movies and years combined
 * Return: void
 */
void ActorGraph::createGraph( vector<ActorNode*>& graph, vector<string>& 
unrepactor, vector<string>& actor, vector<pair<string, int>>& mov){
    vector<string> movie;
    // construct the movie string by converting the integers into strings
    for( int i = 0; i<mov.size(); i++ ){
        int temp = mov[i].second;
        string year = to_string(temp);
        string onemovie = mov[i].first;
        // add the #@ based on the format
        string sign = "#@";
        onemovie += sign;
        onemovie += year;
        // add the string back into the string vector
        movie.push_back(onemovie);
    }
    set<string> checkactor;
    // get all the actors without repeating
    for( int i = 0; i<actor.size(); i++ ){
        checkactor.insert(actor[i]);
    }
    // get the unrepeated list of actors which indicate their index
    for ( set<string>::iterator it = checkactor.begin(); it!=checkactor.end();
     ++it ){
        unrepactor.push_back(*it);
    }
    // all the actors/actresses act in one movie
    // the key string indicates the movie's name
    // the unordered_set saves all the unrepeated name of the actors who act in
    // the film
    unordered_map<string, unordered_set<string>> mymap;
    for( int i = 0; i<mov.size(); i++ ){
        unordered_map<string, unordered_set<string>>::iterator got;
        got = mymap.find(movie[i]);
        if( got == mymap.end() ){
            unordered_set<string> newone;
            newone.insert(actor[i]);
            pair<std::string, unordered_set<string>> onepair(movie[i], newone );
            mymap.insert( onepair );
        }
        else{
            (got->second).insert(actor[i]);
        }
    }
    
    // construct the graph
    // need to change 
    int i = 0;
    // create a node for each of the unrepeated actors or actresses
    for(i=0; i<unrepactor.size(); i++){
        //std::cout<<unrepactor[i]<<std::endl;
        ActorNode* onenode = new ActorNode(unrepactor[i]);
        // index based on their relative position in the vector
        onenode->index = i;
        
        // update its neighbors
        unordered_set<int> neighbor;
        // find all the movies the person involved
        for( int j = 0; j<actor.size(); j++ ){
           if( actor[j] == unrepactor[i] ){
               unordered_map<string, unordered_set<string>>::iterator got;
               got = mymap.find(movie[j]); 
               for(const auto& ele: got->second){
               //while(ele = got->second != nullptr ){
                   //if( ele != unrepactor[j] ) {
                   // the edge consist: current node, the node linked to 
                   // the movie name
                   ActorEdge* oneedge = new ActorEdge(actor[j],ele,got->first);
                   oneedge->weight = 1 + (2015 - mov[j].second);
                   onenode->relatededge.push_back(oneedge);
                   // update the adj vector for the BFS algorithm
                   for(int k = 0; k<unrepactor.size(); k++ ){
                       if( ele == unrepactor[k] ){
                           (onenode->adj).push_back(k);
                       }
                   }
               }
           }
        }
        // we got all the neighbors
        graph.push_back(onenode);
    }
    // update the ActorNode in each of the edge of each node
    for( int i = 0; i<unrepactor.size(); i++){
        
        for( int j = 0; j<graph[i]->relatededge.size(); j++ ){    
            ActorEdge* oneedge = graph[i]->relatededge[j];
            oneedge->node1 = graph[i];
            oneedge->node2 = graph[graph[i]->adj[j]];
        }
    }  
    
}

/**
 * Function name: emptyGraph
 * Description: This graphs created have no edges but only
 * all the valid actor nodes
 */
void ActorGraph::emptyGraph(vector<ActorNode*>& graph, vector<string>& 
     unrepactor, vector<string>& actor, vector<pair<string, int>>& mov ){
     
    set<string> checkactor;
    // get all the actors without repeating
    for( int i = 0; i<actor.size(); i++ ){
        checkactor.insert(actor[i]);
    }
    // get the unrepeated list of actors which indicate their index
    for ( set<string>::iterator it = checkactor.begin(); it!=checkactor.end();
     ++it ){
        unrepactor.push_back(*it);
    }

    // all the actors/actresses act in one movie
    // the key string indicates the movie's name
    // the unordered_set saves all the unrepeated name of the actors who act in
    // the film
    for( int i = 0; i<mov.size(); i++ ){
        unordered_map<string, unordered_set<string>>::iterator got;
        int temp = mov[i].second;
        string year = to_string(temp);
        string onemovie = mov[i].first;
        // add the #@ based on the format
        string sign = "#@";
        onemovie += sign;
        onemovie += year;
        got = mymap.find(onemovie);
        // if there is no such movie
        if( got == mymap.end() ){
            unordered_set<string> newone;
            newone.insert(actor[i]);
            pair<std::string, unordered_set<string>> onepair(onemovie, newone );
            mymap.insert( onepair );
        }
        // update the actors who cast in
        else{
            (got->second).insert(actor[i]);
        }
    }
    
    // construct the graph
    // need to change 
    int i = 0;
    // create a node for each of the unrepeated actors or actresses
    for(i=0; i<unrepactor.size(); i++){
        //std::cout<<unrepactor[i]<<std::endl;
        ActorNode* onenode = new ActorNode(unrepactor[i]);
        // index based on their relative position in the vector
        onenode->index = i;
        
        // we got all the neighbors
        graph.push_back(onenode);
    }
    
}

/*
 * Function name: ActorGraph::sortMovie
 * Parameter: vector<pair<string, string>>& earlyActor: the first element in the
 * pair indicates the name of the actor, the second indicate the movie.
 * return type: none
 */
void ActorGraph::sortMovie( unordered_map<int, unordered_set<string>>& sorted, 
 vector<pair<string, int>>& mov){
    // Use sorted to store all the movies
    // also update the earlist movie_year 
    for( int i = 0; i<mov.size(); i++ ){
        if( mov[i].second < early ){
            early = mov[i].second;
        }
        if( mov[i].second > latest ){
            latest = mov[i].second;
        }
        unordered_map<int, unordered_set<string>>::iterator got;
        int temp = mov[i].second;
        //string year = to_string(temp);
        //string onemovie = mov[i].first;
        // add the #@ based on the format
        //string sign = "#@";
        //onemovie += sign;
        //onemovie += year;
        got = sorted.find(temp);
        // if there is no such year
        if( got == sorted.end() ){
            unordered_set<string> newone;
            newone.insert(mov[i].first);
            pair<int, unordered_set<string>> onepair(mov[i].second, newone );
            sorted.insert( onepair );
        }
        else{
            // update the movie name
            (got->second).insert(mov[i].first);
        }
    }
}
    
        
/**
 * Function name: updateNode
 * This method used to update all the nodes when the user wants to
 * use bfs to find the actor connections.
 * The graph used here only has actor nodes
 */ 
int ActorGraph::updateNode( int year,  unordered_map<int, 
     unordered_set<string>>& sorted, int from, int to, 
     vector<ActorNode*>& graph, vector<string>& unrepactor ){
     unordered_map<int, unordered_set<string>>::iterator it;
     it = sorted.find(year);
     // if there is no such year
     if( it == sorted.end() ){
         return 0;
     }
     
     unordered_set<string> setmovies = it->second;
     unordered_set<string>::iterator movit;
     vector<string> movies;
     // find all the movies in the certain year
     for( movit = setmovies.begin(); movit != setmovies.end(); ++movit ){
         movies.push_back(*movit);
     }
     
     // each of the movie has an actor list
     for(int j = 0; j<movies.size(); j++){
         unordered_map<string, unordered_set<string>>::iterator got;
         
         string syear = to_string(year);
         string onemovie = movies[j];
         // add the #@ based on the format
         string sign = "#@";
         onemovie += sign;
         onemovie += syear;
         // find the actor list in mymap unordered map
         got = mymap.find(onemovie);
         unordered_set<string> actors = got->second;
         vector<int> each;
         int flag = 0;
         // change the strinf name into number representations
         for( int k = 0; k<unrepactor.size(); k++){
             if( actors.find(unrepactor[k]) != actors.end() ){
                 // the k is the index we want
                 // each.push_back(k);
                 if(k == from || k == to ){
                     flag += 1;
                 }
                 if( flag == 2 ){
                     return year;
                 }
                 
             }
         }
         
         // then update the adjall based on the list we find
         //for( int i = 0; i<each.size(); i++ ){
             //vector<int> temp = each;
             //temp.erase(each.begin()+i);
           //  for( int k = 0; k<temp.size(); k++){
                 //graph[each[i]]->adj.push_back(temp[k]);
             //}
             //    if(temp[k] == to && each[i] == from){
             //        return year;
             //    }
             //}
             
         //}
    }
    return 0;
}


/**
 * Function name: UFupdate
 * Description: This method also update the nodes in one graph when
 * the user chooses to use ufind to find the actor connections
 * The graph used here only has actor nodes
 */
int ActorGraph::UFupdate(int year, unordered_map<int, unordered_set<string>>& 
    sorted,int nodei, int nodej, int parents[],  vector<string>& unrepactor){
    //std::cout<<"UFupdate "<<year<<std::endl;
    //for( int i = 0; i<sorted.size(); i++ ){
    unordered_map<int, unordered_set<string>>::iterator it;
    it = sorted.find(year);
    if( it == sorted.end()){
        return 0;
    }
    unordered_set<string> movies;
    movies = it->second;
    unordered_set<string>::iterator got;
    //std::cout<<movies.size()<<std::endl;
    for( got = movies.begin(); got!=movies.end(); ++got){
        string syear = to_string(year);
        string onemovie = *got;
        // add the #@ based on the format
        string sign = "#@";
        onemovie += sign;
        onemovie += syear;
        unordered_map<string, unordered_set<string>>::iterator mapit;
        mapit = mymap.find(onemovie);
        unordered_set<string> relatedactor = mapit->second;
        int nodei = 0;
        int nodej = 0;
        vector<int> each;
        for( int k = 0; k<unrepactor.size(); k++){
            if( relatedactor.find(unrepactor[k]) != relatedactor.end() ){
                // the k is the index we want
                each.push_back(k);
            }
        }
        //std::cout<<each.size()<<std::endl;
        for( int i = 0; i<each.size()-1; i++){ 
            int j = i+1;
            //for( int j = 0 ; j<each.size(); j++ ){
                if( parents[each[i]] == -1 && parents[each[j]] == -1 ){
                    Union(each[i],each[j], parents);
                }
                if( find(each[i], parents) != find(each[j], parents) ){
                    Union(each[i], each[j], parents);
                }
                /*if( relatedactor.find(unrepactor[i]) != relatedactor.end() &&
                    relatedactor.find(unrepactor[j]) != relatedactor.end() ){
                    return year;
                }*/
        }
    }
    return 0;
}
            
    
             
/**
 * Function name: Union
 * Description: This function will union to actor nodes
 */
void ActorGraph::Union( int nodei, int nodej, int parents[]){
    // find the i in the array
    int indexi = find(nodei, parents);
    
    // find the j in the array
    int indexj = find(nodej, parents);
 
    // union the nodes
    parents[indexi] = indexj;
}

/**
 * Function name: Find
 * Description: This function will find the root of the node 
 * in the array
 */   
int ActorGraph::find( int nodei, int parents[] ){
    //std::cout<<"infind"<<parents[0]<<std::endl;
    if( parents[nodei] == -1 ){
    //std::cout<<"infind"<<nodei<<std::endl;
        return nodei;
    }
    // find the root of the node
    /*while( parents[temp] != -1 ){
    //std::cout<<"infind"<<std::endl;
        temp = parents[temp];
    }*/
    int temp = find(parents[nodei], parents);
    // update the value of the nodei
    parents[nodei] = temp;
    return parents[nodei];
}
         
         
    
         
