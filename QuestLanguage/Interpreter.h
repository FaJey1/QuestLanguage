#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <map>
#include <stack>
#include <vector>
#include <cstring>
#include <iostream>
#include "UnitClass.h"

using namespace std;

class Interpreter 
{
private:
	vector<vector<UnitClass>>* code;
	vector<UnitClass>::iterator line_it;

	map<string, int> integer_variables;
	map<string, double> double_variables;
	map<string, string> string_variables;
	map<string, bool> boolean_variables;
public:
	Interpreter(vector<vector<UnitClass>>*);
	~Interpreter();
	void exec_code();
	void show_variables();
private:
	UnitClass do_binary_op(UnitClass, UnitClass, UnitClass);
	void do_assign_op(string, UnitClass);
	void specify_variable(UnitClass*);
	void normalize_boolean(UnitClass *);
	void normalize_string(UnitClass*);
	template<typename T>
	T normalize_value(UnitClass);
};

#endif
