#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cmath>
#include "Graph.h"
using namespace std;

typedef pair<int, Vertex<int> * > APR;

vector<int> prefixFunction(string pattern);

int kmpMatcher(string text, string pattern);

int numStringMatching(string filename,string toSearch);

int editDistance(string pattern, string text);

int aproximate_matching(string pattern,string text);
