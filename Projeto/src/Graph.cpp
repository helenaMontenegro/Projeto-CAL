#include "Graph.h"
#include <iostream>
#define M_PI  3.141592653589793238462643383279502884
using namespace std;

/*********** VERTEX ***********/
template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
Vertex<T>::Vertex(ifstream &in){
		string aux;

		getline(in, aux, ';');
		this->info = atoi(aux.c_str());

		getline(in, aux, ';');
		setX(atoi(aux.c_str()));
		getline(in, aux, '\n');
		setY(atoi(aux.c_str()));
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
	return dist;
}

template <class T>
double Vertex<T>::getDist2() const {
	return dist2;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

template <class T>
void Vertex<T>::setType(string type){
	this->type = type;
}

template <class T>
string Vertex<T>::getName() const {
	return name;
}

template <class T>
void Vertex<T>::setName(string name){
	this->name = name;
}

template <class T>
void Vertex<T>::setPrice(double price)
{
	this->price = price;
}

template <class T>
double Vertex<T>::getPrice() const
{
	return price;
}

template <class T>
string Vertex<T>::getType() const {
	return type;
}

template <class T>
int Vertex<T>::getX() const {
		return x;
}

template <class T>
void Vertex<T>::setX(int x){
		this->x = x;
}

template <class T>
int Vertex<T>::getY() const {
		return y;
}

template <class T>
void Vertex<T>::setY(int y){
		this->y = y;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Edge<T> *edg) {
	adj.push_back(*edg);
}

template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

/*********** EDGE ***********/
template <class T>
Edge<T>::Edge(Vertex<T> *d, double w) {
	dest = d;
	weight = w;
}

template <class T>
Edge<T>::Edge(int id, Vertex<T> *vDest, string name, string twoWay, double w){
		setId(id);
		this->dest=vDest;
		setName(name);
		setTwoWay(twoWay);
		setWeight(w);
}

template <class T>
int Edge<T>::getId() const {
		return id;
}

template <class T>
void Edge<T>::setId(int id){
		this->id = id;
}

template <class T>
string Edge<T>::getName() const{
		return name;
}

template <class T>
void Edge<T>::setName(string name){
		this->name = name;
}

template <class T>
void Edge<T>::setTwoWay(string val){
	if(val == "True"){
		this->isTwoWay = true;
	}else{
		this->isTwoWay = false;
	}
}

template <class T>
bool Edge<T>::getTwoWay(){
	return isTwoWay;
}

template <class T>
void Edge<T>::setWeight(double w){
		this->weight = w;
}

/*********** GRAPH ***********/
template <class T>
Graph<T>::Graph(){}

template <class T>
Graph<T>::Graph(ifstream &node_in, ifstream &edge_in, ifstream &poi_in, ifstream &edge_poi_in){
	//Vertex:
	while (!node_in.eof()) {
		Vertex<T> *temp = new Vertex<T>(node_in);
		vertexSet.push_back(temp);
	}

	//Edge:
	while (!edge_in.eof()) {
		string aux, name, twoWay;

		getline(edge_in, aux, ';');
		int id = atoi(aux.c_str());
		getline(edge_in, aux, ';');
		int idOri = atoi(aux.c_str());
		getline(edge_in, aux, ';');
		int idDest = atoi(aux.c_str());
		getline(edge_in, name, ';');
		getline(edge_in, twoWay, ';');

		Vertex<T> *vDest = findVertex(idDest);
		if(vDest->getName() == "")
			vDest->setName(name);
		Vertex<T> *vOri = findVertex(idOri);
		if(vOri->getName() == "")
			vOri->setName(name);

		//Calculo do peso da edge
		double w = 1;//peso(vOri, vDest);

			Edge<T> *temp = new Edge<T>(id, vDest, name, "True", w);
			Edge<T> *temp2 = new Edge<T>(id, vOri, name, twoWay, w);
			vOri->addEdge(temp);
			vDest->addEdge(temp2);
	}

	//Points of interest:
	string aux;
	while (!poi_in.eof()) {

		getline(poi_in, aux, ';');
		int id = atoi(aux.c_str());
		getline(poi_in, aux, ';');
		int x = atoi(aux.c_str());
		getline(poi_in, aux, ';');
		int y = atoi(aux.c_str());
		getline(poi_in, aux, ';');
		string type = aux;
		getline(poi_in, aux, ';');
		string name = aux;
		getline(poi_in, aux, '\n');
		double price = atof(aux.c_str());

		Vertex<T> *v = findVertex(id);
		if (v == NULL) {
			v = new Vertex<T>(id);
			v->setX(x);
			v->setY(y);
			vertexSet.push_back(v);
		}
		v->setName(name);
		v->setType(type);
		v->setPrice(price);
	}

	while(!edge_poi_in.eof())
	{
		getline(edge_poi_in, aux, ';');
		int id = atoi(aux.c_str());
		getline(edge_poi_in, aux, ';');
		int ori = atoi(aux.c_str());
		getline(edge_poi_in, aux, '\n');
		int dest = atoi(aux.c_str());
		Vertex<T> *v = findVertex(ori);
		Vertex<T> *v1 = findVertex(dest);
		double weight = 1;//calcDist(v->getLat(),v->getLon(),v1->getLat(),v1->getLon());
		Edge<T> *temp = new Edge<T>(id, v1, "", "True", weight);
		v->addEdge(temp);
		Edge<T> *temp1 = new Edge<T>(id, v, "", "True", weight);
		v1->addEdge(temp1);
		//set edge as true
	}
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

template <class T>
bool Graph<T>::addVertex(const T &in) {
	if ( findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

template <class T>
bool Graph<T>::addEdge(const int id, const T &sourc, const T &dest, double w) {
	//int id, Vertex<T> *vDest, string name, string twoWay, double w
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	Edge<T> *e = new Edge<T>(id, v2, "", "True", w);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(e);
	return true;
}

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
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
T Graph<T>::dijkstraClosestPark(const T &origin) {
	double minDist = INF;
	T park;
	string parque = "Parque";
	MutablePriorityQueue<Vertex<T> > q;
	Vertex<T> *v1 = findVertex(origin);
	q.insert(v1);
	for(size_t j = 0; j < vertexSet.size(); j++)
	{
		vertexSet.at(j)->visited = false;
		vertexSet.at(j)->dist = INF;
		vertexSet.at(j)->path = NULL;
	}
	v1->visited = true;
	v1->dist = 0;
	while (!q.empty()) {
		v1 = q.extractMin();
		for (size_t i = 0; i < v1->adj.size(); i++) {
			Vertex<T> *v = v1->adj.at(i).dest;
			if (v->info != origin) {
				if (!v->visited) {
					if(v->dist > v1->dist + v1->adj.at(i).weight) {
						v->dist = v1->dist + v1->adj.at(i).weight;
						v->path = v1;
					}
					if(v->type == parque)
					{
						v->visited = true;
						if(v->dist < minDist)
						{
							minDist = v->dist;
							park = v->info;
						}
					}
					else if(v->dist < minDist)
					{
						v->visited = true;
						q.insert(v);
					}
				} else {
					if (v->dist > v1->dist + v1->adj.at(i).weight) {
						v->dist = v1->dist + v1->adj.at(i).weight;
						v->path = v1;
						if(v->type == parque)
						{
							if(v->dist < minDist)
							{
								minDist = v->dist;
								park = v->info;
							}
						}
						q.decreaseKey(v);
					}
				}
			}
		}
	}
	return park;
}

template<class T>
T Graph<T>::dijkstraCheapestPark(const T &origin, double maxDist) {
	double minPrice = -1;
	T park;
	string parque = "Parque";
	MutablePriorityQueue<Vertex<T> > q;
	Vertex<T> *v1 = findVertex(origin);
	q.insert(v1);
	for(size_t j = 0; j < vertexSet.size(); j++)
	{
		vertexSet.at(j)->visited = false;
		vertexSet.at(j)->dist = INF;
		vertexSet.at(j)->path = NULL;
	}
	v1->visited = true;
	v1->dist = 0;
	while (!q.empty()) {
		v1 = q.extractMin();
		for (size_t i = 0; i < v1->adj.size(); i++) {
			Vertex<T> *v = v1->adj.at(i).dest;
			if (v->info != origin) {
				if (!v->visited) {
					if(v->dist > v1->dist + v1->adj.at(i).weight) {
						v->dist = v1->dist + v1->adj.at(i).weight;
						v->path = v1;
					}
					if(v->type == parque)
					{
						v->visited = true;
						if((v->price < minPrice || minPrice == -1) && v->dist <= maxDist)
						{
							minPrice = v->price;
							park = v->info;
						}
					}
					else if(v->dist <= maxDist)
					{
						v->visited = true;
						q.insert(v);
					}
				} else {
					if (v->dist > v1->dist + v1->adj.at(i).weight) {
						v->dist = v1->dist + v1->adj.at(i).weight;
						v->path = v1;
						if(v->type == parque)
						{
							if ((v->price < minPrice || minPrice == -1) && v->dist <= maxDist) {
								minPrice = v->price;
								park = v->info;
							}
						}
						q.decreaseKey(v);
					}
				}
			}
		}
	}
	if(minPrice != -1)
		return park;
	return -1;
}

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin, const T &dest)
{
	double minDist = INF;
	MutablePriorityQueue<Vertex<T> > q;
	Vertex<T> *v1 = findVertex(origin);
	q.insert(v1);
	for(size_t j = 0; j < vertexSet.size(); j++)
	{
		vertexSet.at(j)->visited = false;
		vertexSet.at(j)->dist = INF;
		vertexSet.at(j)->path = NULL;
	}
	v1->visited = true;
	v1->dist = 0;
	while (!q.empty()) {
		v1 = q.extractMin();
		for (size_t i = 0; i < v1->adj.size(); i++) {
			/*if( v1->adj.at(i).getTwoWay()){*/
			Vertex<T> *v = v1->adj.at(i).dest;
			if (v->info != origin) {
				if (!v->visited) {
					if(v->dist > v1->dist + v1->adj.at(i).weight) {
						v->dist = v1->dist + v1->adj.at(i).weight;
						v->path = v1;
					}
					if(v->info == dest)
					{
						v->visited = true;
						if(v->dist < minDist)
							minDist = v->dist;
					}
					else if(v->dist < minDist)
					{
						v->visited = true;
						q.insert(v);
					}
				} else {
					if (v->dist > v1->dist + v1->adj.at(i).weight) {
						v->dist = v1->dist + v1->adj.at(i).weight;
						v->path = v1;
						if(v->info == dest)
						{
							if(v->dist < minDist)
								minDist = v->dist;
						}
						q.decreaseKey(v);
					}
				}
			}
			//}
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
		//cout << v1->getInfo() << endl;
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
		vertexSet.at(j)->dist2 = 0;
		vertexSet.at(j)->path = NULL;
		vertexSet.at(j)->path2 = NULL;
	}
	v1->visited = true;
	v2->visited2 = true;
	while (!q.empty()) {
		v1 = q.extractMin();
		v2 = q1.extractMin();
		for (size_t i = 0; i < v1->adj.size() || i < v2->adj.size(); i++) {
			if (i < v1->adj.size() && v1->adj.at(i).getTwoWay()) {
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
			if (i < v2->adj.size() && v2->adj.at(i).getTwoWay()) {
				Vertex<T> *v3 = v2->adj.at(i).dest;
				if (v3->info != dest) {
					if (!v3->visited2) {
						v3->visited2 = true;
						v3->dist2 = v2->dist2 + v2->adj.at(i).weight;
						v3->path2 = v2;
						q1.insert(v3);
					} else {
						if (v3->dist2 > v2->dist2 + v2->adj.at(i).weight) {
							v3->dist2 = v2->dist2 + v2->adj.at(i).weight;
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
	return -1;
}
