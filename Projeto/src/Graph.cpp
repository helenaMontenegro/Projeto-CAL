#include "Graph.h"
#include <iostream>

using namespace std;

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if ( findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2,w);
	return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
	MutablePriorityQueue<Vertex<T> > q;
	Vertex<T> *v1 = findVertex(origin);
	q.insert(v1);
	for(size_t j = 0; j < vertexSet.size(); j++)
	{
		vertexSet.at(j)->visited = false;
		vertexSet.at(j)->dist = 0;
		vertexSet.at(j)->path = NULL;
	}
	v1->visited = true;
	while (!q.empty()) {
		v1 = q.extractMin();
		for (size_t i = 0; i < v1->adj.size(); i++) {
			Vertex<T> *v = v1->adj.at(i).dest;
			if (v->info != origin) {
				if (!v->visited) {
					v->visited = true;
					v->dist = v1->dist + v1->adj.at(i).weight;
					v->path = v1;
					q.insert(v);
				} else {
					//q.decreaseKey(v);
					if (v->dist > v1->dist + v1->adj.at(i).weight) {
						v->dist = v1->dist + v1->adj.at(i).weight;
						v->path = v1;
					}
				}
			}
		}
	}
}

template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
	vector<T> res;
	vector<T> inverted_res;
	Vertex<T> *v1 = findVertex(dest);
	inverted_res.push_back(dest);
	while(v1->info != origin)
	{
		v1 = v1->path;
		inverted_res.push_back(v1->info);
	}
	for(int i = inverted_res.size()-1; i >= 0; i--)
		res.push_back(inverted_res.at(i));
	return res;
}
