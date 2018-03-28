#include "Graph.h"
#include <iostream>

using namespace std;

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
void Vertex<T>::setName(string name)
{
	this->name = name;
}

template <class T>
string Vertex<T>::getName() const
{
	return name;
}

template <class T>
void Vertex<T>::setType(string type)
{
	this->type = type;
}

template <class T>
string Vertex<T>::getType() const
{
	return type;
}

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

template <class T>
Vertex<T> * Graph<T>::findVertex(const string &local) const {
	for (auto v : vertexSet)
		if (v->name == local)
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
					if (v->dist > v1->dist + v1->adj.at(i).weight) {
						v->dist = v1->dist + v1->adj.at(i).weight;
						v->path = v1;
						q.decreaseKey(v);
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

template<class T>
vector<T> Graph<T>::getPath2(const T &origin, const T &dest) const{
	vector<T> inverted_res;
	Vertex<T> *v1 = findVertex(dest);
	inverted_res.push_back(dest);
	while(v1->info != origin)
	{
		v1 = v1->path2;
		inverted_res.push_back(v1->info);
	}
	return inverted_res;
}

template<class T>
T Graph<T>::getClosestPark() const{
	T info;
	double dist = INF;
	for(size_t i = 0; i < vertexSet.size(); i++)
	{
		if(vertexSet.at(i)->type == "Parque")
		{
			if(dist > vertexSet.at(i)->dist)
			{
				dist = vertexSet.at(i)->dist;
				info = vertexSet.at(i)->info;
			}
		}
	}
	return info;
}

template<class T>
T Graph<T>::dijkstraBidirectionalPath(const T &origin, const T &dest) {
	string b = "Bomba de Gasolina";
	MutablePriorityQueue<Vertex<T> > q, q1;
	Vertex<T> *v1 = findVertex(origin);
	Vertex<T> *v2 = findVertex(dest);
	q.insert(v1);
	q1.insert(v2);
	for (size_t j = 0; j < vertexSet.size(); j++) {
		vertexSet.at(j)->visited = false;
		vertexSet.at(j)->visited2 = false;
		vertexSet.at(j)->dist = 0;
		vertexSet.at(j)->path = NULL;
		vertexSet.at(j)->path2 = NULL;
	}
	v1->visited = true;
	v2->visited2 = true;
	while (!q.empty()) {
		v1 = q.extractMin();
		v2 = q1.extractMin();
		for (size_t i = 0; i < v1->adj.size() || i < v2->adj.size(); i++) {
			if (i < v1->adj.size()) {
				Vertex<T> *v = v1->adj.at(i).dest;
				if (v->info != origin) {
					if (!v->visited) {
						v->visited = true;
						v->dist = v1->dist + v1->adj.at(i).weight;
						v->path = v1;
						q.insert(v);
					} else {
						if (v->dist > v1->dist + v1->adj.at(i).weight) {
							v->dist = v1->dist + v1->adj.at(i).weight;
							v->path = v1;
							q.decreaseKey(v);
						}
					}
					if (v->visited2 && v->type == b)
						return v->info;
				}
			}
			if (i < v2->adj.size()) {
				Vertex<T> *v3 = v2->adj.at(i).dest;
				if (v3->info != dest) {
					if (!v3->visited2) {
						v3->visited2 = true;
						v3->dist = v2->dist + v2->adj.at(i).weight;
						v3->path2 = v2;
						q1.insert(v3);
					} else {
						if (v3->dist > v2->dist + v2->adj.at(i).weight) {
							v3->dist = v2->dist + v2->adj.at(i).weight;
							v3->path2 = v2;
							q1.decreaseKey(v3);
						}
					}
					if (v3->visited && v3->type == b)
						return v3->info;
				}
			}
		}
	}
}
