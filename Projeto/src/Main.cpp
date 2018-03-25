#include <iostream>
#include "Graph.h"
#include "Graph.cpp"
#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class Main {
public:
	GraphViewer *gv;
	Graph<int> onFoot; //string instead of int
	Graph<int> byCar; //string instead of int
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

int main(){
	Main m;
	if(m.buildGraphs() != 0)
		return 1;
	m.onFoot.dijkstraShortestPath(0);
	vector<int> v = m.onFoot.getPath(0, 13);
	for(size_t i = 0; i < v.size(); i++)
	{
		m.gv->setVertexColor(v.at(i), "green");
		Sleep(800);
		m.gv->rearrange();
	}
	getchar();
	return 0;
}

/**Missing: insert information on respective graphs**/
int Main::buildGraphs()
{
	ifstream in("nos.txt");
	if (!in){
		cerr << "nos.txt not loaded" << endl;
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

	in.open("arestas.txt");
	if (!in)
	{
		cerr << "arestas.txt not loaded" << endl;
		return 1;
	}
	while (!in.eof()) {
		getline(in, c, ';');
		idEdge = atoi(c.c_str());
		getline(in, c, ';');
		x = atoi(c.c_str());
		getline(in, c, '\n');
		y = atoi(c.c_str());
		gv->addEdge(idEdge, x, y, EdgeType::DIRECTED);
		onFoot.addEdge(x, y, idEdge); //only for tests
	}
	in.close();
	return 0;
}
