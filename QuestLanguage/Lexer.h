#ifndef _LEXER_
#define _LEXER_

#include <iostream>
#include <iomanip>
#include <fstream>
#include "Patterns.h"
#include "ArrayByFaJey.h"
#include "UnitClass.h"

using namespace std;

class Lexer {
private:
	
	Patterns patterns;
	vector <UnitClass> token_list;

public:

	Lexer(string filename = "test.txt");

	void string_for_analysis(string line_for_analysis = "", int line = 0);
	void write_in_token_list(string word);

	void read_in_file(string name);
	void read_in_commandline();

	vector <UnitClass> get_token_list();
	void show_token_list();
};
#endif
