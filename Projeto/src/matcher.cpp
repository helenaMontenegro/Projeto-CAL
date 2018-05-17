#include "matcher.h"

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

vector<string> splitSentence(string sentence) {
	istringstream iss(sentence);
	vector<string> tokens{ istream_iterator<string>{iss},
		istream_iterator<string>{} };

	if (tokens.size() == 0) {
		tokens.push_back("");
	}
	return tokens;
}

string to_lower(string &text) {
	for (size_t i = 0; i < text.size(); i++)
	{
		text[i] = tolower(text[i]);
	}
	return text;
}

void eliminateRedudantWords(vector<string> &text){
	for (auto it = text.begin(); it!=text.end();it++){
		string current = (*it);
		if (current == "Rua" || current == "Avenida" || current == "Praca" || current == "Largo" || current == "Patio" || current == "Beco" || current == "Tunel"){
			text.erase(it);
			it--;
		}
		else if (current == "de" || current == "do" || current == "da" || current == "dos" || current == "das"){
			text.erase(it);
			it--;
		}
	}
}

int aproximate_matching(string pattern,string text){

	int totalEditDistance = 0;
  int currentEditDistance;
	int currentTotalDistance = 0;
  vector<string> textSplitted = splitSentence(text);
  vector<string> patternSplitted = splitSentence(pattern);
	if (textSplitted.size() != patternSplitted.size()){
		totalEditDistance += abs((int)(textSplitted.size() - patternSplitted.size()));
	}
	eliminateRedudantWords(textSplitted);
	eliminateRedudantWords(patternSplitted);
  for (vector<string>::iterator itP = patternSplitted.begin(); itP != patternSplitted.end(); itP++){
    for (vector<string>::iterator itT = textSplitted.begin(); itT != textSplitted.end(); itT++ ){
      currentEditDistance = editDistance(*itP,*itT);
			if (currentEditDistance == 0){
				currentTotalDistance = currentTotalDistance/2;
				break;
			}
			currentTotalDistance += currentEditDistance;
    }
		totalEditDistance+=currentTotalDistance;
  }

  return totalEditDistance;
}


int editDistance(string pattern, string text) {
	int n = text.length();
	vector<int> d(n + 1);
	int old, neww;
	for (int j = 0; j <= n; j++)
		d[j] = j;
	int m = pattern.length();
	for (int i = 1; i <= m; i++)
	{
		old = d[0];
		d[0] = i;
		for (int j = 1; j <= n; j++)
		{
			if (pattern[i - 1] == text[j - 1]) neww = old;
			else
			{
				neww = min(old, d[j]);
				neww = min(neww, d[j - 1]);
				neww = neww + 1;
			}
			old = d[j];
			d[j] = neww;
		}
	}
	return d[n];
}


