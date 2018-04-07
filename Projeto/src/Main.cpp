#include <iostream>
#include "Graph.h"
#include "Graph.cpp"
#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <cstddef>
using namespace std;

class Main {
public:
	GraphViewer *gv;
	Graph<int> graph;
	Main(ifstream &node_in, ifstream &edge_in, ifstream &poi_in, ifstream &edge_poi_in);
};

Main::Main(ifstream &node_in, ifstream &edge_in, ifstream &poi_in, ifstream &edge_poi_in)
{
	gv = new GraphViewer(600, 600, false);
	gv->createWindow(600, 600);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");
	Graph<int> g(node_in, edge_in, poi_in, edge_poi_in);
	graph = g;
}

int showPath(Main m, int distance, bool fuel, int origin, int dest)
{
	vector<int> v, v1, v2;
	string r = "Rua";
	int p;
	if (distance == -1)
		p = m.graph.dijkstraClosestPark(dest);
	else
		p = m.graph.dijkstraCheapestPark(dest, distance);
	if(p == -1)
	{
		cout << "There is no park available" << endl;
		return -1;
	}
	v = m.graph.getPath(dest,p);

	if(fuel)
	{
		int g = m.graph.dijkstraBidirectionalPath(origin,p);
		v1 = m.graph.getPath(origin, g);
		v2 = m.graph.getPath2(p, g);
	}
	else
	{
		m.graph.dijkstraShortestPath(origin,p);
		v1 = m.graph.getPath(origin,p);
	}
	int x, n = 0;
	for(size_t i = 0; i < v1.size(); i++)
	{
		if(m.graph.findVertex(v1.at(i))->getType() == r)
			m.gv->setVertexColor(v1.at(i), "pink");
		if (i != 0)
		{
			m.gv->addEdge(n, x,v1.at(i), EdgeType::DIRECTED);
			n++;
		}
		Sleep(600);
		x = v1.at(i);
		m.gv->rearrange();
	}
	if (fuel) {
		for (size_t i = 0; i < v2.size(); i++) {
			if(m.graph.findVertex(v2.at(i))->getType() == r)
				m.gv->setVertexColor(v2.at(i), "pink");
			if (i != 0)
			{
				m.gv->addEdge(n, x, v2.at(i), EdgeType::DIRECTED);
				n++;
			}
			Sleep(600);
			x = v2.at(i);
			m.gv->rearrange();
		}
	}
	for(int i = v.size()-1; i >= 0; i--)//size_t i = 0; i < v.size(); i++)
	{
		if(m.graph.findVertex(v.at(i))->getType() == r)
			m.gv->setVertexColor(v.at(i), "green");
		if(i != (int) v.size()-1)
		{
			m.gv->addEdge(n, x, v.at(i), EdgeType::DIRECTED);
			m.gv->setEdgeDashed(n, true);
			n++;
		}
		Sleep(600);
		x = v.at(i);
		m.gv->rearrange();
	}
	getchar();
	return 0;
}

int main(){
	ifstream in1("A.txt");
	if (!in1){
		cerr << "A.txt not loaded" << endl;
		return 1;
	}
	ifstream in2("BeC.txt");
	if (!in1) {
		cerr << "BeC.txt not loaded" << endl;
		return 1;
	}
	ifstream in3("D.txt");
	if (!in1) {
		cerr << "D.txt not loaded" << endl;
		return 1;
	}
	ifstream in4("E.txt");
	if (!in1) {
		cerr << "E.txt not loaded" << endl;
		return 1;
	}

	Main m(in1,in2,in3,in4);
	/*TESTES*/
	Vertex<int> *v = m.graph.findVertex(269392602);
	if(v == NULL)
		cout << "error" << endl;
	else
	{
		cout << v->getLat() << endl;
		cout << v->getName() << endl;
		cout << v->getType() << endl;
		cout << v->getLon() << endl;
	}
	v = m.graph.findVertex(1155452262);
	cout << "next Vertex" << endl;
	if (v == NULL)
		cout << "error" << endl;
	else {
		cout << v->getLat() << endl;
		cout << v->getName() << endl;
		cout << v->getType() << endl;
		cout << v->getLon() << endl;
	}
	v = m.graph.findVertex("Lidl");
	cout << "next Vertex" << endl;
	if (v == NULL)
		cout << "error" << endl;
	else {
		cout << v->getLat() << endl;
		cout << v->getInfo() << endl;
		cout << v->getType() << endl;
		cout << v->getLon() << endl;
	}
	return 0;
}

