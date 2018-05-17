#include <iostream>
#include "Graph.h"
#include "Graph.cpp"
#include "matcher.h"
#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <cstddef>
#include <utility>  
#include <algorithm>
#include <map>

#define MAX_X 1920
#define MAX_Y 1028

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
	void removeEdges(int numEdges);
	int addVertexes();
	bool labelWasUsed(vector<string> labelsUsed, string word);
};

Main::Main()
{
	gv = new GraphViewer(800, 800, false);
	gv->createWindow(800, 800);
	gv->defineVertexColor("red");
	gv->defineEdgeColor("black");
	gv->defineEdgeCurved(false);
}

Main::Main(ifstream &node_in, ifstream &edge_in, ifstream &poi_in, ifstream &edge_poi_in)
{
	gv = new GraphViewer(MAX_X, MAX_Y, false);
	gv->createWindow(MAX_X, MAX_Y);
	gv->defineVertexColor("red");
	gv->defineEdgeColor("black");
	gv->defineEdgeCurved(false);
	Graph<int> g(node_in, edge_in, poi_in, edge_poi_in);
	g.generateParks(10);
	graph = g;
	int numEdges = addVertexes();
	getchar();
	Sleep(10000);
	removeEdges(numEdges);
}

int Main::addVertexes()
{
	vector<vector<Edge<int>> > edges;
	string b = "Bomba de Gasolina", r = "Rua", p = "Parque";
	vector<Vertex<int> *> vertexSet = graph.getVertexSet();
	for(size_t i = 0; i < vertexSet.size(); i++)
	{
		Vertex<int> *v = vertexSet.at(i);
		gv->addNode(v->getInfo(), v->getX(), v->getY());
		if(v->getType() == b)
			gv->setVertexColor(v->getInfo(), "yellow");
		else if(v->getType() == r)
			gv->setVertexColor(v->getInfo(), "gray");
		else if(v->getType() == p)
			gv->setVertexColor(v->getInfo(), "blue");
		vector<Edge<int> > e = v->getEdges();
		edges.push_back(e);
		gv->setVertexSize(v->getInfo(), 10);
	}
	vector<string> labelsUsed;
	int numEdges = 0;
	gv->defineEdgeCurved(false);
	for(size_t i = 0; i < edges.size(); i++)
	{
		for(size_t j = 0; j < edges.at(i).size(); j++){
			if (edges.at(i).at(j).getByCar()) {
				gv->addEdge(numEdges, vertexSet.at(i)->getInfo(),
						edges.at(i).at(j).getDest()->getInfo(),
						EdgeType::UNDIRECTED);
				if (!labelWasUsed(labelsUsed, edges.at(i).at(j).getName())) {
					gv->setEdgeLabel(numEdges, edges.at(i).at(j).getName());
					labelsUsed.push_back(edges.at(i).at(j).getName());
				}
				numEdges++;
			}
		}
	}
	cout << endl << "Ruas disponiveis:" << endl;
	for(size_t i = 0; i < labelsUsed.size(); i++)
	{
		if(labelsUsed.at(i) != "")
			cout << labelsUsed.at(i) << endl;
	}
	cout << endl;
	return numEdges-1;
}

bool Main::labelWasUsed(vector<string> labelsUsed, string word)
{
	for(size_t i = 0; i < labelsUsed.size(); i++)
	{
		if(labelsUsed.at(i) == word)
			return true;
	}
	return false;
}

void Main::removeEdges(int numEdges)
{
	for(int i = 0; i <= numEdges; i++)
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
		gv->setVertexSize(idNode, 0.1);
	}
	in.close();
	getchar();
	Sleep(3000);
	removeEdges(55);
	return 0;
}

int convertLongitudeToX(double longitude) {
	return floor((longitude - smallestLong) * MAX_X / (largestLong - smallestLong));
}

int convertLatitudeToY(double latitude) {
	return MAX_Y - floor((latitude - smallestLat) * MAX_Y / (largestLat - smallestLat));
}

void convertFile(ifstream &in){
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
        longi = atof(aux.c_str());
        getline(in, aux, '\n');
        lat =atof(aux.c_str());
        x = convertLongitudeToX(longi);
        y = convertLatitudeToY(lat);

        converted << id << ";" << x << ";" << y << endl;
    }


    converted.close();

}

void getMaxMinLatLong(ifstream &in){
    string aux;
    double lat, longi;

    getline(in, aux, ';');
    getline(in, aux, ';');
    getline(in, aux, ';');

    //initialization of the initial values
    getline(in, aux, ';');
    smallestLong = atof(aux.c_str());
    largestLong =atof(aux.c_str());
    getline(in, aux, '\n');
    smallestLat =atof(aux.c_str());
    largestLat = atof(aux.c_str());

    while(!in.eof()){
        //ignoring  id and values in degrees
        getline(in, aux, ';');
        getline(in, aux, ';');
        getline(in, aux, ';');
        //--------------------------------
        getline(in, aux, ';');
        longi = atof(aux.c_str());
        getline(in, aux, '\n');
        lat =atof(aux.c_str());

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

void turnRoadsToGraySimples(Main &m)
{
	string r = "Rua";
	for(int i = 0; i < 19; i++)
	{
		Vertex<int> *v = m.graph.findVertex(i);
		if(v->getType() == r)
			m.gv->setVertexColor(i, "gray");
	}
}

int showPath(Main &m, int distance, bool fuel, int origin, int dest, int park, bool wantPark)
{
	cout << "wantPark = " << wantPark << endl;
	vector<int> v, v1, v2;
	string r = "Rua";
	int p;
	if (distance == -1 && !wantPark)
		p = m.graph.dijkstraClosestPark(dest);
	else if(!wantPark)
		p = m.graph.dijkstraCheapestPark(dest, distance);
	else
		p = park;
	if(p == -1)
	{
		cout << "Nao ha nenhum parque disponivel." << endl;
		return -1;
	}
	Vertex<int> *parque = m.graph.findVertex(p);
	cout << "O parque mais indicado e o " << parque->getName()
			<< " cuja distancia ao seu destino e " << parque->getDist()
			<< " e cujo preco e " << parque->getPrice() << " euros." << endl << endl;
	if(wantPark)
		m.graph.dijkstraShortestPath(dest, p);
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
	int nEdges = 0;
	for(size_t i = 0; i < v1.size(); i++)
	{
		if(m.graph.findVertex(v1.at(i))->getType() == r)
			m.gv->setVertexColor(v1.at(i), "pink");
		if (i != 0)
		{
			m.gv->addEdge(n, x,v1.at(i), EdgeType::DIRECTED);
			m.gv->setEdgeDashed(n, false);
			nEdges++;
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
				m.gv->setEdgeDashed(n, false);
				nEdges++;
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
			nEdges++;
			n++;
		}
		Sleep(600);
		x = v.at(i);
		m.gv->rearrange();
	}
	getchar();
	Sleep(2000);
	return nEdges;
}

int complexo(bool aprox) {
	ifstream in1("A.txt");
	if (!in1) {
		cerr << "A.txt not loaded" << endl;
		return 1;
	}
	getMaxMinLatLong(in1);
	in1.clear();
	in1.seekg(0, ios::beg);
	convertFile(in1); //creates a file called converted.txt

	in1.close();
	ifstream in5("converted.txt");
	if (!in5) {
		cerr << "converted.txt not loaded" << endl;
		return 1;
	}

	ifstream in2("BeC.txt");
	if (!in2) {
		cerr << "BeC.txt not loaded" << endl;
		return 1;
	}
	ifstream in3("D.txt");
	if (!in3) {
		cerr << "D.txt not loaded" << endl;
		return 1;
	}
	ifstream in4("E.txt");
	if (!in4) {
		cerr << "E.txt not loaded" << endl;
		return 1;
	}
	Main m(in5, in2, in3, in4);
	in2.close();
	in3.close();
	in4.close();
	in5.close();

	cout << "Destinos disponiveis:" << endl;
	cout << "Restaurante Tainha" << endl << "Lidl" << endl << "Pingo Doce"
			<< endl << "Minipreco" << endl << "Locanda Real" << endl
			<< "Restaurante 5 Amigos" << endl << endl;

	long origin, dest, park3;
	string ori, desti, park1;
	char wantFuel, wantPark;
	int park, nEdges;
	double maxDistance;
	double distance = -1; //-1 -> by distance; else -> by price
	bool fuel = false, park2 = false;
	string r = "Rua", b = "Bomba de Gasolina", p = "Parque";
	while (1) {
		while (1) {
			cout << "Qual a origem? ";
			getline(cin, ori);
			Vertex<int> *v = m.graph.findVertex(ori);
			if (v == NULL || v->getType() != r)
				cout << "Origem invalida. Tente novamente." << endl;
			else {
				origin = v->getInfo();
				break;
			}
		}
		while (1) {
			cout << "Quer escolher a rua do parque onde quer estacionar? (S/N) "
					<< endl;
			cin >> wantPark;
			cin.ignore(100, '\n');
			if (wantPark == 'S' || wantPark == 's') {
				park2 = true;
				break;
			} else if (wantPark == 'N' || wantPark == 'n') {
				park2 = false;
				break;
			}
			cout << "Resposta invalida. Tente novamente." << endl;
		}
		if(park2 && !aprox)
		{
			cout << "Qual a rua do parque? ";
			getline(cin, park1);
			Vertex<int> *v = m.graph.findPark(park1);
			if (v == NULL)
			{
				cout << "Rua ou parque nao existente." << endl;
				wantPark = false;
			}
			else
				park3 = v->getInfo();
		} else if(park2 && aprox)
		{
			int num_iter = 0;
			while(num_iter < 3)
			{
			vector<string> names;
			cout << "Qual a rua do parque? ";
			getline(cin, park1);
			vector<Vertex<int> *> vertexes = m.graph.findAproximatePark(park1);
			if(park1 == vertexes.at(0)->getName())
			{
				park3 = vertexes.at(0)->getInfo();
				break;
			}
			else{
				if(num_iter == 2)
				{
					cout << "Numero de tentativas excedido. Prosseguindo." << endl;
					wantPark = false;
					break;
				}
				cout << "Rua nao encontrada. Opcoes alternativas:" << endl;
			for(size_t i = 0; i < vertexes.size(); i++)
			{
				bool found = false;
				Vertex<int> *a = vertexes.at(i);
				for(size_t j = 0; j < names.size(); j++)
				{
					if(names.at(j) == a->getName())
						found = true;
				}
				if(!found)
				{
					cout << a->getName() << endl;
					names.push_back(a->getName());
				}
			}
			cout << "Tente novamente." << endl;
			}
			}
		}

		while (1) {
			cout << "Qual o destino? ";
			getline(cin, desti);
			Vertex<int> *v = m.graph.findVertex(desti);
			if (v == NULL || v->getType() == r || v->getType() == b
					|| v->getType() == p)
				cout << "Destino invalido. Tente novamente." << endl;
			else {
				dest = v->getInfo();
				break;
			}
		}

		while (1) {
			cout << "Necessita de abastecer o seu carro? (S/N) " << endl;
			cin >> wantFuel;
			if (wantFuel == 'S' || wantFuel == 's') {
				fuel = true;
				break;
			} else if (wantFuel == 'N' || wantFuel == 'n') {
				fuel = false;
				break;
			}
			cout << "Resposta invalida. Tente novamente." << endl;
		}
		if(!wantPark)
		{
		while (1) {
			cout
					<< "Quer o parque de estacionamento mais barato(1) ou mais proximo(2)?"
					<< endl;
			cin >> park;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(100, '\n');
			} else if (park == 1) {
				while (1) {
					cout << "Qual a distancia maxima que esta disposto a andar?"
							<< endl;
					cin >> maxDistance;
					if (cin.fail() || maxDistance <= 0)
						cout << "Resposta invalida. Tente novamente." << endl;
					else
						break;

				}
				distance = maxDistance;
				break;
			} else if (park == 2) {
				distance = -1;
				break;
			}
			cout << "Resposta invalida. Tente novamente." << endl;
		}
		}
		nEdges = showPath(m, distance, fuel, origin, dest, park3, wantPark);
		while (1) {
			int answer;
			cout
					<< "Quer sair do programa(1) ou encontrar um novo trajeto(2)? ";
			cin >> answer;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(100, '\n');
				cout << "Resposta invalida. Tente novamente." << endl;
			} else if (answer == 1)
				return 0;
			else if (answer == 2) {
				m.removeEdges(nEdges - 1);
				cin.ignore(100, '\n');
				break;
			}
			cout << "Resposta invalida. Tente novamente." << endl;
		}

	}
	return 0;
}

int simples()
{
	Main m;
	if (m.buildGraphs() != 0)
		return 1;
	int origin, dest;
	string r = "Rua", b = "Bomba de Gasolina", p = "Parque";
	m.graph.findVertex(11)->setPrice(13);
	m.graph.findVertex(13)->setPrice(9);
	m.graph.findVertex(12)->setPrice(5);
	m.graph.findVertex(10)->setPrice(12);
	int nEdges;
	while(1)
	{
	while (1) {
		cout << "Qual a origem? ";
		cin >> origin;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(100, '\n');
			cout << "Destino invalido. Tente novamente." << endl;
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
	int park, maxDistance;
	char wantFuel;
	int distance = -1; //-1 -> by distance; else -> by price
	bool fuel = true; //the car needs fuel
	 while (1) {
			cout << "Necessita de abastecer o seu carro? (S/N) " << endl;
			cin >> wantFuel;
			if (wantFuel == 'S' || wantFuel == 's') {
				fuel = true;
				break;
			} else if (wantFuel == 'N' || wantFuel == 'n') {
				fuel = false;
				break;
			}
			 cout << "Resposta invalida. Tente novamente." << endl;
		}
		while(1)
		{
	    cout << "Quer o parque de estacionamento mais barato(1) ou mais proximo(2)?" << endl;
	    cin >> park;
	    if(cin.fail())
	    {
	    	cin.clear();
	    	cin.ignore(100, '\n');
	    }
	    else if (park == 1) {
	    	while(1)
	    	{
	        cout << "Qual a distancia maxima que esta disposto a andar?" << endl;
	        cin >> maxDistance;
	        if(cin.fail() || maxDistance <= 0)
	        	cout << "Resposta invalida. Tente novamente." << endl;
	        else break;

	    	}
	        distance = maxDistance;
	        break;
	    }
	    else if (park == 2)
	    {
	    	distance = -1;
	    	break;
	    }
	    cout << "Resposta invalida. Tente novamente." << endl;
	    }
	 	nEdges = showPath(m, distance, fuel, origin, dest, -1, false);
		while (1) {
			int answer;
			cout
					<< "Quer sair do programa(1) ou encontrar um novo trajeto(2)? ";
			cin >> answer;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(100, '\n');
				cout << "Resposta invalida. Tente novamente." << endl;
			} else if (answer == 1)
				return 0;
			else if (answer == 2)
			{
				turnRoadsToGraySimples(m);
				m.removeEdges(nEdges - 1);
				break;
			}
			cout << "Resposta invalida. Tente novamente." << endl;
		}

	}
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
			int answer;
			bool aprox = false;
			while (1) {
				cout << "Quer pesquisa aproximada(1) ou exata(2) " << endl;
				cin >> answer;
				if (cin.fail() || answer < 1 || answer > 2) {
					cin.clear();
					cin.ignore(100,'\n');
					cout << "Resposta invalida. Tente novamente." << endl;
				} else if(answer == 2){
					aprox = false;
					break;
				}
				else if(answer == 1)
					{
					aprox = true;
					break;
					}
			}
			complexo(aprox);
			return 0;
		}
		cout << "Exemplo invalido. Tente novamente." << endl;
	}
	return 0;
}
