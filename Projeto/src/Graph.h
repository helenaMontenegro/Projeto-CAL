/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <fstream>
#include <cstdlib>
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
	T info;               				//Contents
	vector<Edge<T> > adj; 				//Outgoing edges
	bool visited = false;         //Auxiliary field
	bool visited2 = false;				//Auxiliary field for bidirectional dijkstra
	double dist = 0;
	double dist2 = 0;
	Vertex<T> *path = NULL;
	Vertex<T> *path2 = NULL;
	int queueIndex = 0; 					//Required by MutablePriorityQueue
	string type = "Rua";
	string name = "";
	double price;
	bool processing = false;									//Id attributed by the parser
	float lat = 0;								//Latitude in radians
	float lon = 0;								//Longitude in radians
	void addEdge(Vertex<T> *dest, double w);
public:
	Vertex(T in);
	Vertex(ifstream &in);
	bool operator<(Vertex<T> & vertex) const;								//Required by MutablePriorityQueue
	T getInfo() const;
	double getDist() const;
	double getDist2() const;
	Vertex *getPath() const;
	void setType(string type);
	string getType() const;
	void setName(string name);
	string getName() const;
	void setPrice(double price);
	double getPrice() const;
	float getLat() const;
	void setLat(float lat);
	float getLon() const;
	void setLon(float lon);
	void addEdge(Edge<T> *edg);
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};

/********************** Edge  ****************************/

template <class T>
class Edge {
	Vertex<T> * dest;     		 			//Destination vertex
	double weight = 0;         			//Edge weight
	string name;										//Name of the street
	int id = 0;											//Id attributed by the parser
	bool isTwoWay = false;					//Road is two ways or not
public:
	Edge(Vertex<T> *d, double w);
	Edge(int id, Vertex<T> *vDest, string name, string twoWay, double w);
	//Edge(ifstream &in);
	int getId() const;
	void setId(int id);
	string getName() const;
	void setName(string name);
	void setTwoWay(string val);
	bool getTwoWay();
	void setWeight(double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    //Vertex set
public:
	Graph();
	Graph(ifstream &node_in, ifstream &edge_in, ifstream &poi_in, ifstream &edge_poi_in);
	Vertex<T> *findVertex(const T &in) const;
	Vertex<T> *findVertex(const string &local) const;
	bool addVertex(const T &in);
	bool addEdge(const int id, const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;
	void dijkstraShortestPath(const T &s);
	void dijkstraShortestPath(const T &s, const T &d);
	T dijkstraClosestPark(const T &s);
	T dijkstraCheapestPark(const T &s, double maxDist);
	vector<T> getPath(const T &origin, const T &dest) const;
	vector<T> getPath2(const T &origin, const T &dest) const;
	T getClosestPark() const;
	T dijkstraBidirectionalPath(const T &origin, const T &dest);
};

#endif /* GRAPH_H_ */
