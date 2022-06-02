#ifndef _UNITCLASS_
#define _UNITCLASS_

#include <string>

using namespace std;

class UnitClass
{
public:
	string type;
	string value;
	UnitClass(string t, string n) {
		type = t;
		value = n;
	}
};

#endif

