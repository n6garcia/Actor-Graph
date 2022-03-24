#include "ActorGraph.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

ActorGraph::ActorGraph() {}

/* Build the actor graph from dataset file.
 * Each line of the dataset file must be formatted as:
 * ActorName <tab> MovieName <tab> Year
 * Two actors are connected by an undirected edge if they have worked in a movie
 * before.
 */
bool ActorGraph::buildGraph(istream& is) {
    bool readHeader = false;

    // vector of movies for graph building
    vector<ActorList> ActorLists;

    while (is) {
        string s;
        if (!getline(is, s)) break;

        // skip the header of the file
        if (!readHeader) {
            readHeader = true;
            continue;
        }

        // read each line of the dataset to get the movie actor relation
        istringstream ss(s);
        vector<string> record;
        while (ss) {
            string str;
            if (!getline(ss, str, '\t')) break;
            record.push_back(str);
        }

        // if format is wrong, skip current line
        if (record.size() != 3) {
            continue;
        }

        // extract the information
        string actor(record[0]);
        string title(record[1]);
        int year = stoi(record[2]);

        // TODO: we have an actor/movie relationship to build the graph

        // actor's node
        Node* n = nullptr;

        // search through nodes for already existing node
        for (auto it = forest.begin(); it != forest.end(); it++) {
            if ((*it)->Actor == actor) {
                n = *it;
                break;
            }
        }

        // if node doesn't already exist, create it
        if (n == nullptr) {
            n = new Node(actor);
            forest.push_back(n);
        }

        // create movie from record
        Movie m = Movie(title, year);

        // bool for logic below
        bool found = false;

        // find movie from list of movies and add actor to it
        for (auto it = ActorLists.begin(); it != ActorLists.end(); it++) {
            if (m == (*it).movie) {
                (*it).actors.push_back(n);
                found = true;
            }
        }

        // if movie not found add new ActorList from movie and add actor to it
        if (!found) {
            ActorLists.push_back(ActorList(m));
            ActorLists.back().actors.push_back(n);
        }
    }

    // build edges from ActorLists
    for (auto it = ActorLists.begin(); it != ActorLists.end(); it++) {
        for (auto itA = (*it).actors.begin(); itA != (*it).actors.end();
             itA++) {
            for (auto itC = (*it).actors.begin(); itC != (*it).actors.end();
                 itC++) {
                if ((*itA) == (*itC)) continue;
                (*itA)->Edges.push_back(Edge((*itC), (*it).movie));
            }
        }
    }

    // if failed to read the file, clear the graph and return
    if (!is.eof()) {
        // TODO: delete the graph
        for (auto it = forest.begin(); it != forest.end(); it++) {
            delete (*it);
        }
        return false;
    }

    return true;
}

/* TODO */
void ActorGraph::BFS(const string& fromActor, const string& toActor,
                     string& shortestPath) {
    // Edge Case: from Actor is the same as to Actor
    if (fromActor == toActor) return;

    // Node we are starting from, containing fromActor
    Node* fromNode = nullptr;
    Node* toNode = nullptr;

    // search entire forest for fromActors Node
    for (auto it = forest.begin(); it != forest.end(); it++) {
        if ((*it)->Actor == fromActor) {
            fromNode = *it;
            break;
        }
    }

    // search entire forest for toActors Node
    for (auto it = forest.begin(); it != forest.end(); it++) {
        if ((*it)->Actor == toActor) {
            toNode = *it;
            break;
        }
    }

    // if node isn't found, return
    if (fromNode == nullptr) return;
    if (toNode == nullptr) return;

    // a vector<Node *> represents a single path, paths and temps are vectors of
    // paths
    vector<vector<Node*>> paths;
    vector<vector<Node*>> temp;

    // a variable to hold our shortest path
    vector<Node*> shortest;

    // set base node as searched
    fromNode->searched = true;

    // populate initial paths
    paths.push_back({fromNode});

    // maximum distance of a path is amount of nodes in graph, no need to
    // compute paths bigger than this
    for (int i = 0; i < forest.size(); i++) {
        // iterate through every path in paths
        for (auto it = paths.begin(); it != paths.end(); it++) {
            // iterate through every edge connected to last node in path
            // note: vectors in paths are ordered
            for (auto itE = (*it).back()->Edges.begin();
                 itE != (*it).back()->Edges.end(); itE++) {
                // if child on other end of edge hasn't already been searched by
                // BFS
                if ((*itE).child->searched == false) {
                    // add new path = path + child to temp
                    temp.push_back(*it);
                    temp.back().push_back((*itE).child);

                    // set child as searched
                    (*itE).child->searched = true;

                    // if child is our destination break for loop and set
                    // shortest path as current path
                    if ((*itE).child->Actor == toActor) {
                        if (shortest.size() == 0) {
                            shortest = (temp.back());
                            break;
                        }
                    }
                }
            }
            // break loop
            if (shortest.size() != 0) break;
        }
        // break loop
        if (shortest.size() != 0) break;

        // update paths
        paths = temp;
    }

    // clear the searched variable from every Node
    for (auto it = forest.begin(); it != forest.end(); it++) {
        (*it)->searched = false;
    }

    // if we didn't find a path, exit function
    if (shortest.size() == 0) return;

    //(actor)--[movie#@year]-->(actor)--...
    shortestPath = shortestPath + "(" + fromNode->Actor + ")";
    for (auto it = shortest.begin(); it != shortest.end() - 1; it++) {
        shortestPath = shortestPath + "--[";

        for (auto itE = (*it)->Edges.begin(); itE != (*it)->Edges.end();
             itE++) {
            if ((*itE).child->Actor == (*(it + 1))->Actor) {
                shortestPath = shortestPath + (*itE).movie.title + "#@" +
                               to_string((*itE).movie.year);
                shortestPath = shortestPath + "]-->(";
                shortestPath = shortestPath + (*itE).child->Actor + ")";
                break;
            }
        }
    }
}

/* TODO */
ActorGraph::~ActorGraph() {
    for (auto it = forest.begin(); it != forest.end(); it++) {
        delete (*it);
    }
}