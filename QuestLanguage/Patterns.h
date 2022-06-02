#ifndef __PATTERNS__
#define __PATTERNS__

#include "ArrayByFaJey.h"
#include <regex>
#include <vector>

struct KeyValue
{
	string key;
	string regexp;

	void pair(string k, string r){
		key = k;
		regexp = r;
	}
};

class Patterns
{
private:
	vector<KeyValue> patterns;

public:
	Patterns();
	string get_type(string lexem); 
	void print_patterns();
};

#endif
