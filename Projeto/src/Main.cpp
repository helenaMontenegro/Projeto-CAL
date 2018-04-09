#include <iostream>
#include "Graph.h"
#include "Graph.cpp"
#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <cstddef>

#define MAX_X 1920
#define MAX_Y 1080

double largestLat, largestLong, smallestLat, smallestLong;
string convertedFilepath;

using namespace std;

class Main {
public:
	GraphViewer *gv;
	Graph<int> graph;
	Main();
	Main(ifstream &node_in, ifstream &edge_in, ifstream &poi_in, ifstream &edge_poi_in);
	int buildGraphs();
	void removeEdges();
	double largestLat, largestLong, smallestLat, smallestLong;
	void getMaxMinLatLong(ifstream &in);
	int convertLongitudeToX(double longitude);
	int convertLatitudeToY(double latitude);
	void convertFile(ifstream &in);
};

Main::Main()
{
	gv = new GraphViewer(600, 600, false);
	gv->createWindow(600, 600);
	gv->defineVertexColor("red");
	gv->defineEdgeColor("black");
	gv->defineEdgeCurved(false);
}

Main::Main(ifstream &node_in, ifstream &edge_in, ifstream &poi_in, ifstream &edge_poi_in)
{
	gv = new GraphViewer(600, 600, false);
	/*gv->createWindow(600, 600);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");*/
	Graph<int> g(node_in, edge_in, poi_in, edge_poi_in);
	graph = g;
}

void Main::removeEdges()
{
	for(int i = 0; i <= 55; i++)
	{
		gv->removeEdge(i);
	}
}

int Main::buildGraphs()
{
	ifstream in("nos1.txt");
	if (!in){
		cerr << "nos1.txt not loaded" << endl;
		return 1;
	}
	string c;
	int idNode, x, y, idEdge;

	cout << "Cinzento - Estradas" << endl << "Azul - Parques" << endl
			<< "Vermelho - Pontos de Interesse" << endl
			<< "Amarelo - Bombas de Gasolina" << endl << endl;

	while (!in.eof()) {
		getline(in, c, ';');
		idNode = atoi(c.c_str());
		getline(in, c, ';');
		x = atoi(c.c_str());
		getline(in, c, '\n');
		y = atoi(c.c_str());
		gv->addNode(idNode, x, y);
		graph.addVertex(idNode);
		Vertex<int> *v = graph.findVertex(idNode);
		stringstream ss;
		ss << idNode;
		string str = ss.str();
		v->setName(str);
		gv->setVertexLabel(idNode, str);
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
		if (idEdge % 2 == 0) {
			gv->addEdge(idEdge, x, y, EdgeType::UNDIRECTED);
			gv->setEdgeLabel(idEdge, c);
		}
		graph.addEdge(idEdge, x, y, atoi(c.c_str())); //only for tests
	}
	in.close();

	in.open("nos_nome2.txt");
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
		Vertex<int> *v = graph.findVertex(idNode);
		if(c == b)
		{
			v->setType(c);
			gv->setVertexColor(idNode, "yellow");
		}
		else if(c == r)
		{
			v->setType(c);
			gv->setVertexColor(idNode, "gray");
		}
		else if(c == p)
		{
			v->setType(c);
			gv->setVertexColor(idNode, "blue");
		}
		else
		{
			v->setType("Ponto de Interesse");
			v->setName(c);
		}
	}
	in.close();
	getchar();
	Sleep(10000);
	removeEdges();
	return 0;
}


void Main::convertFile(ifstream &in){
    ofstream converted;
    convertedFilepath = "converted.txt";

    converted.open(convertedFilepath);

    string aux;
    double lat, longi;
    int x, y;
    string id;


    while(!in.eof()){

        getline(in, id, ';');
        getline(in, aux, ';');
        getline(in, aux, ';');
        //--------------------------------
        getline(in, aux, ';');
        lat = atof(aux.c_str());
        getline(in, aux, '\n');
        longi =atof(aux.c_str());
        x = convertLongitudeToX(longi);
        y = convertLatitudeToY(lat);

        converted << id << ";" << x << ";" << y << endl;
    }


    converted.close();

}


void Main::getMaxMinLatLong(ifstream &in){
    string aux;
    double lat, longi;

    getline(in, aux, ';');
    getline(in, aux, ';');
    getline(in, aux, ';');

    //initialization of the initial values
    getline(in, aux, ';');
    smallestLat = atof(aux.c_str());
    largestLat =atof(aux.c_str());
    getline(in, aux, '\n');
    smallestLong =atof(aux.c_str());
    largestLong = atof(aux.c_str());

    while(!in.eof()){
        //ignoring  id and values in degrees
        getline(in, aux, ';');
        getline(in, aux, ';');
        getline(in, aux, ';');
        //--------------------------------
        getline(in, aux, ';');
        lat = atof(aux.c_str());
        getline(in, aux, '\n');
        longi =atof(aux.c_str());

        if(lat > largestLat) {
            largestLat = lat;
        } else if(lat < smallestLat) {
            smallestLat = lat;
        }
        if(longi > largestLong) {
            largestLong = longi;
        } else if(longi < smallestLong) {
            smallestLong = longi;
        }
    }
}

int Main::convertLongitudeToX(double longitude) {
	return floor((longitude - smallestLong) * MAX_X / (largestLong - smallestLong));
}

int Main::convertLatitudeToY(double latitude) {
	return floor((latitude - smallestLat) * MAX_Y / (largestLat - smallestLat));
}


int showPath(Main &m, int distance, bool fuel, int origin, int dest)
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
		cout << "Nao ha nenhum parque disponivel." << endl;
		return -1;
	}
	Vertex<int> *parque = m.graph.findVertex(p);
	cout << "O parque mais indicado e o " << parque->getName()
			<< " cuja distancia ao seu destino e " << parque->getDist()
			<< " e cujo preco e " << parque->getPrice() << " euros." << endl << endl;
	v = m.graph.getPath(dest,p);

	if(fuel)
	{
		int g = m.graph.dijkstraBidirectionalPath(origin,p);
		Vertex<int> *fuel = m.graph.findVertex(g);
		cout << "A bomba de gasolina mais indicada e: " << fuel->getName()
				<< " que se situa a " << fuel->getDist()
				<< " de distancia a origem e a " << fuel->getDist2()
				<< " de distancia ao parque." << endl << endl;
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
		/*if(m.graph.findVertex(v1.at(i))->getType() == r)
			m.gv->setVertexColor(v1.at(i), "pink");
		if (i != 0)
		{
			m.gv->addEdge(n, x,v1.at(i), EdgeType::DIRECTED);
			n++;
		}
		Sleep(600);
		x = v1.at(i);
		m.gv->rearrange();*/
		cout << m.graph.findVertex(v1.at(i))->getName() << endl;
	}
	if (fuel) {
		for (size_t i = 0; i < v2.size(); i++) {
			cout << m.graph.findVertex(v2.at(i))->getName() << endl;
			/*
			if(m.graph.findVertex(v2.at(i))->getType() == r)
				m.gv->setVertexColor(v2.at(i), "pink");
			if (i != 0)
			{
				m.gv->addEdge(n, x, v2.at(i), EdgeType::DIRECTED);
				n++;
			}
			Sleep(600);
			x = v2.at(i);
			m.gv->rearrange();*/
		}
	}
	for(int i = v.size()-1; i >= 0; i--)//size_t i = 0; i < v.size(); i++)
	{
		cout << m.graph.findVertex(v.at(i))->getName() << endl;
		/*
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
		m.gv->rearrange();*/
	}
	//getchar();
	return 0;
}

int showPath2(Main &m, int distance, bool fuel, int origin, int dest)
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
		cout << "Nao ha nenhum parque disponivel." << endl;
		return -1;
	}
	Vertex<int> *parque = m.graph.findVertex(p);
	cout << "O parque mais indicado e o " << parque->getName()
			<< " cuja distancia ao seu destino e " << parque->getDist()
			<< " e cujo preco e " << parque->getPrice() << " euros." << endl << endl;
	v = m.graph.getPath(dest,p);

	if(fuel)
	{
		int g = m.graph.dijkstraBidirectionalPath(origin,p);
		Vertex<int> *fuel = m.graph.findVertex(g);
		cout << "A bomba de gasolina mais indicada e: " << fuel->getName()
				<< " que se situa a " << fuel->getDist()
				<< " de distancia a origem e a " << fuel->getDist2()
				<< " de distancia ao parque." << endl << endl;
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

int complexo()
{
	Main m;
	if (m.buildGraphs() != 0)
		return 1;
	int origin, dest;
	char petrolStation;
	string r = "Rua", b = "Bomba de Gasolina", p = "Parque";
	while (1) {
		cout << "Qual a origem? ";
		cin >> origin;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "Origem invalida. Tente novamente." << endl;
		} else {
			Vertex<int> *v = m.graph.findVertex(origin);
			if (v == NULL || v->getType() != r)
				cout << "Origem invalida. Tente novamente." << endl;
			else
				break;
		}
	}
	while (1) {
		cout << "Qual o destino? ";
		cin >> dest;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(100, '\n');
			cout << "Origem invalida. Tente novamente." << endl;
		} else {
			Vertex<int> *v = m.graph.findVertex(dest);
			if (v == NULL || v->getType() == r || v->getType() == b || v->getType() == p)
				break;
		}
	}

	int distance = -1; //-1 -> by distance; else -> by price
	bool fuel = true; //the car needs fuel

	m.graph.findVertex(11)->setPrice(13);
	m.graph.findVertex(13)->setPrice(9);
	m.graph.findVertex(12)->setPrice(5);
	m.graph.findVertex(10)->setPrice(12);
	showPath(m, distance, fuel, origin, dest);
	getchar();
	return 0;
}

int simples()
{
	Main m;
	if (m.buildGraphs() != 0)
		return 1;
	int origin, dest;
	string r = "Rua", b = "Bomba de Gasolina", p = "Parque";
	while (1) {
		cout << "Qual a origem? ";
		cin >> origin;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "Origem invalida. Tente novamente." << endl;
		} else {
			Vertex<int> *v = m.graph.findVertex(origin);
			if (v == NULL || v->getType() != r)
				cout << "Origem invalida. Tente novamente." << endl;
			else
				break;
		}
	}
	while (1) {
		cout << "Qual o destino? ";
		cin >> dest;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(100, '\n');
			cout << "Origem invalida. Tente novamente." << endl;
		} else {
			Vertex<int> *v = m.graph.findVertex(dest);
			if (v == NULL || v->getType() == r || v->getType() == b || v->getType() == p)
				cout << "Origem invalida. Tente novamente." << endl;
			else
				break;
		}
	}
	int distance = -1; //-1 -> by distance; else -> by price
	bool fuel = true; //the car needs fuel

	m.graph.findVertex(11)->setPrice(13);
	m.graph.findVertex(13)->setPrice(9);
	m.graph.findVertex(12)->setPrice(5);
	m.graph.findVertex(10)->setPrice(12);
	showPath2(m, distance, fuel, origin, dest);
	getchar();
	return 0;
}


int main(){
	string exemplo, s = "simples", c = "complexo";
	while (1) {
		cout << "Que exemplo abrir? ('complexo' -> openStreetMaps; 'simples') ";
		cin >> exemplo;
		if(exemplo == s)
		{
			simples();
			return 0;
		}
		if(exemplo == c){
			//complexo();
			return 0;
		}
			break;
	}

	ifstream in1("A.txt");
	if (!in1){
		cerr << "A.txt not loaded" << endl;
		return 1;
	}
	getMaxMinLatLong(in1);
    in1.clear();
    in1.seekg(0, ios::beg);
    convertFile(in1);//creates a file called converted.txt
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
	//m.getMaxMinLatLong(in1);
	/*TESTES*/
	Vertex<int> *dest = m.graph.findVertex("Lidl");
	Vertex<int> *ori = m.graph.findVertex("Avenida Doutor Moreira de Sousa");
	/*//int p = m.graph.dijkstraClosestPark(dest->getInfo());
	//int p = m.graph.dijkstraCheapestPark(dest->getInfo(), 100);
	int p = m.graph.dijkstraCheapestPark(dest->getInfo(), 7);
	cout << p << endl;
	if (p != -1) {
		Vertex<int> *parque = m.graph.findVertex(p);
		cout << parque->getName() << endl;
		cout << parque->getType() << endl;
		cout << parque->getDist() << endl;
	}*/
	showPath(m,-1,true,ori->getInfo(), dest->getInfo());
	return 0;
}
