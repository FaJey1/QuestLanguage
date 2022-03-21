#ifndef __PATTERNS__
#define __PATTERNS__

#include "ArrayByFaJey.h"
#include <regex>

class Patterns
{
private:
	ArrayByFaJey<string> patterns;

public:
	void get_type(string lexem);
};

#endif
