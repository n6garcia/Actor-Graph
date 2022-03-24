#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <vector>

using namespace std;

class Node;

class Movie {
  public:
    string title;
    int year;

    Movie(string title, int year) : title(title), year(year) {}

    Movie() : title(""), year(0) {}

    bool operator==(const Movie& rhs) const {
        if (rhs.title == title && rhs.year == year) return true;
        return false;
    }
};

class ActorList {
  public:
    Movie movie;
    vector<Node*> actors;

    ActorList(Movie movie) : movie(movie) {}
};

class Edge {
  public:
    Node* child;
    Movie movie;

    Edge(Node* child, Movie movie) : child(child), movie(movie) {}
};

class Node {
  public:
    string Actor;
    vector<Edge> Edges;
    bool searched = false;

    Node(string Actor) { this->Actor = Actor; }
};

class ActorGraph {
  protected:
    // TODO: add data structures used in actor graph
    vector<Node*> forest;

  public:
    /* TODO */
    ActorGraph();

    /* TODO */
    bool buildGraph(istream& is);

    /* TODO */
    void BFS(const string& fromActor, const string& toActor,
             string& shortestPath);

    /* TODO */
    ~ActorGraph();

    void printEdges() {
        for (auto itF = forest.begin(); itF != forest.end(); itF++) {
            cerr << (*itF)->Actor << " $ ";
            for (auto itE = (*itF)->Edges.begin(); itE != (*itF)->Edges.end();
                 itE++) {
                cerr << " | " << itE->child->Actor << " " << itE->movie.title
                     << " " << itE->movie.year;
            }
            cerr << endl;
        }
    }
};

#endif  // ACTORGRAPH_HPP
