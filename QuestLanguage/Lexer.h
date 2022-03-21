#ifndef _LEXER_
#define _LEXER

#include "Patterns.h"
#include "ArrayByFaJey.h"

using namespace std;

class Lexer {
private:
	string line_for_analysis = "";
public:
	Lexer();
	void set_line(string str = "");
	void start_analysis();
	void show_elements_in_line();
};
#endif
