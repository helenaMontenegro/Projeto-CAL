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

int showPath(Main m, int distance, bool fuel, int origin, int dest)
{
	vector<int> v, v1, v2;
	string r = "Rua";
	int p;
	if (distance == -1)
		p = m.onFoot.dijkstraClosestPark(dest);
	else
		p = m.onFoot.dijkstraCheapestPark(dest, distance);
	if(p == -1)
	{
		cout << "There is no park available" << endl;
		return -1;
	}
	v = m.onFoot.getPath(dest,p);

	if(fuel)
	{
		int g = m.onFoot.dijkstraBidirectionalPath(origin,p);
		v1 = m.onFoot.getPath(origin, g);
		v2 = m.onFoot.getPath2(p, g);
	}
	else
	{
		m.onFoot.dijkstraShortestPath(origin,p);
		v1 = m.onFoot.getPath(origin,p);
	}
	int x, n = 0;
	for(size_t i = 0; i < v1.size(); i++)
	{
		if(m.onFoot.findVertex(v1.at(i))->getType() == r)
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
			if(m.onFoot.findVertex(v2.at(i))->getType() == r)
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
		if(m.onFoot.findVertex(v.at(i))->getType() == r)
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
}

int main(){
	return 0;
}

