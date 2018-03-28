/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <cstddef>
#include <string>
#include "MutablePriorityQueue.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // outgoing edges
	bool visited;          // auxiliary field
	bool visited2;			//auxiliary field for bidirectional dijkstra
	double dist = 0;
	Vertex<T> *path = NULL;
	Vertex<T> *path2 = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue
	string type;
	string name;
	bool processing = false;
	void addEdge(Vertex<T> *dest, double w);

public:
	Vertex(T in);
	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	void setName(string name);
	string getName() const;
	void setType(string type);
	string getType() const;
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};

/********************** Edge  ****************************/

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set

public:
	Vertex<T> *findVertex(const T &in) const;
	Vertex<T> *findVertex(const string &local) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;
	void dijkstraShortestPath(const T &s);
	vector<T> getPath(const T &origin, const T &dest) const;
	vector<T> getPath2(const T &origin, const T &dest) const;
	T getClosestPark() const; //TODO
	T dijkstraBidirectionalPath(const T &origin, const T &dest); //TODO
};

#endif /* GRAPH_H_ */
