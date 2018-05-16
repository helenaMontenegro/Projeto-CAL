#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
using namespace std;

vector<int> prefixFunction(string pattern);

int kmpMatcher(string text, string pattern);

int numStringMatching(string filename,string toSearch);

int editDistance(string pattern, string text);

vector<string> approxStringMatching(string input, vector<string> streettown);
