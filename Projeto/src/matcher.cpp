#include <string>
#include <fstream>
#include "matcher.h"
using namespace std;

vector<int> prefixFunction(string pattern) {
	vector<int> prefix;
	const char* pat = pattern.c_str();

	for(size_t i = 1; i <= pattern.size(); i++)
	{
		int k = i-1;
		bool found_k= false;
		while(!found_k)
		{
			bool equals = true;
			if(k == 0)
				break;
			for(int j = 1; j <= k; j++)
			{
				if(pat[j-1] != pat[i-k+j-1])
				{
					equals = false;
					break;
				}
			}
			if(equals == true)
				found_k = true;
			else
				k--;
		}
		prefix.push_back(k);
	}

	return prefix;
}

int kmpMatcher(string text, string pattern) {
	int numStrings = 0;
	const char* pat = pattern.c_str();
	const char* txt = text.c_str();
	bool found;

	vector<int> prefix = prefixFunction(pattern);

	for(size_t i = 0; i < text.size() - pattern.size() + 1; i++)
	{
		found = true;
		for(size_t j = 0; j < pattern.size(); j++)
		{
			if(pat[j] != txt[i + j])
			{
				i += prefix.at(j);
				found = false;
				break;
			}
		}
		if(found == true)
			numStrings++;
	}

	return numStrings;
}


int numStringMatching(string filename,string toSearch) {
	int numStrings = 0;

	ifstream in;
	in.open(filename);
	if (!in) {
		cout << "error opening file: " << filename << endl;
		return 1;
	}

	while (!in.eof()) {
		string line;
		getline(in, line, '\n');
		if (line.size() < toSearch.size())
			continue;
		numStrings += kmpMatcher(line, toSearch);
	}

	return numStrings;
}

/*
int editDistance(string pattern, string text) {

	int m = pattern.length();
	int n = text.length();
	int old, neW;
	vector<int> d(n + 1);

	for (int j = 0; j <= n; j++)
		d[j] = j;

	for (int i = 1; i <= m; i++) {
		old = d[0];
		d[0] = i;
		for (int j = 1; j <= n; j++) {
			if (pattern[i - 1] == text[j - 1])
				neW = old;
			else {
				neW = min(old, d[j]);
				neW = min(neW, d[j - 1]);
				neW = neW + 1;
			}
			old = d[j];
			d[j] = neW;
		}
	}
	return d[n];
}

vector<string> approxStringMatching(string input, vector<string> streettown){

	vector<string> inputs = names(input);
	vector<map<string, int>> mapVecs;

	for (int i = 0; i < inputs.size(); i++){

		map<string, int> mapWord;

		for (int j = 0; j < streettown.size(); i++){
			int diff = 0;
			vector<string> words = names(streettown.at(j));

			for (int k = 0; k < words.size(); k++){

				if ((words.at(k).size() < inputs.at(i).size()) && (words.at(k).size() < 3)){
					continue;
				}

				int diffTemp = editDistance(inputs.at(i), words.at(k));

				if (diff == 0 || diffTemp < diff){
					diff = diffTemp;
				}
			}

			pair<string, int> diffInput = make_pair(streettown.at(j), diff);
			mapWord.insert(diffInput);
		}

		mapVecs.push_back(mapWord);
	}

	multimap<int string> updatedMap;

	for (int i = 0; i < streettown.size(); i++){
		int diff = 0;

		for (int j = 0; j < mapVecs.size(); j++){
			diff += mapVecs[j][streettown.at(i)];
		}
		pair<int, string> p = make_pair(diff, streettown.at(i));
		updatedMap.insert(p);
	}

	vector<string> finalVec;

	for (multimap<int, string>::iterator it = updatedMap.begin();
				it != updatedMap.end(); it++) {


			if (it->first <= (4 * inputs.size()))
				finalVec.push_back(it->second);
		}

		return finalVec;

}*/
