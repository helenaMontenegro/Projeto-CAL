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
	Graph<int> onFoot; //string instead of int
	//Graph<int> byCar; //string instead of int
	Main();
	int buildGraphs();
};

Main::Main()
{
	gv = new GraphViewer(600, 600, false);
	gv->createWindow(600, 600);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");
}

//TODO: User interaction interface: ask origin, destination and if the car needs fuel
int main(){
	Main m;
	if(m.buildGraphs() != 0)
		return 1;
	/*TESTS!*/
	m.onFoot.dijkstraShortestPath(0); // dest
	cout << m.onFoot.getClosestPark() << endl;
	int g = m.onFoot.dijkstraBidirectionalPath(0,10);
	vector<int> v = m.onFoot.getPath(0, g); //to delete
	vector<int> v1 = m.onFoot.getPath2(10, g); //to delete
	int x;
	for(size_t i = 0; i < v.size(); i++)
	{
		m.gv->setVertexColor(v.at(i), "green");
		if(i!= 0)
			m.gv->addEdge(i-1, x, v.at(i), EdgeType::DIRECTED);
		Sleep(600);
		x = v.at(i);
		m.gv->rearrange();
	}
	for(size_t i = 0; i < v1.size(); i++)
	{
		m.gv->setVertexColor(v1.at(i), "green");
		if (i != 0)
			m.gv->addEdge(i +100, x,v1.at(i), EdgeType::DIRECTED);
		Sleep(600);
		x = v1.at(i);
		m.gv->rearrange();
	}
	getchar();
	return 0;
}

/**inserts information on respective graphs**/
int Main::buildGraphs()
{
	/*THE FOLLOWING IS FOR TESTS ONLY*/
	ifstream in("nos1.txt");
	if (!in){
		cerr << "nos1.txt not loaded" << endl;
		return 1;
	}
	string c;
	int idNode, x, y, idEdge;

	while (!in.eof()) {
		getline(in, c, ';');
		idNode = atoi(c.c_str());
		getline(in, c, ';');
		x = atoi(c.c_str());
		getline(in, c, '\n');
		y = atoi(c.c_str());
		gv->addNode(idNode, x, y);
		onFoot.addVertex(idNode); //only for tests
	}
	in.close();

	in.open("arestas1.txt");
	if(!in)
	{
		cerr << "arestas1.txt not loaded" << endl;
		return 1;
	}
	while (!in.eof()) {
		getline(in, c, ';');
		idEdge = atoi(c.c_str());
		getline(in, c, ';');
		x = atoi(c.c_str());
		getline(in, c, ';');
		y = atoi(c.c_str());
		getline(in, c, '\n');
		//gv->addEdge(idEdge, x, y, EdgeType::DIRECTED);
		onFoot.addEdge(x, y, atoi(c.c_str())); //only for tests
	}
	in.close();

	in.open("nos_nome1.txt");
	if(!in)
	{
		cerr << "nos_nome1.txt not loaded" << endl;
		return 1;
	}
	string b = "Bomba de Gasolina", r = "Rua", p = "Parque";
	while (!in.eof()) {
		getline(in, c, ';');
		idNode = atoi(c.c_str());
		getline(in, c, '\n');
		//gv->addEdge(idEdge, x, y, EdgeType::DIRECTED);
		Vertex<int> *v = onFoot.findVertex(idNode);
		if(v != NULL)
			v->setType(c);
		if(v->getType() == b)
			gv->setVertexColor(idNode, "yellow");
		else if(v->getType() == r)
			gv->setVertexColor(idNode, "gray");
		else if(v->getType() == p)
			gv->setVertexColor(idNode, "red");
	}
	in.close();

	return 0;
}
