#ifndef STACKMACHINE_H
#define STACKMACHINE_H

#include "UnitClass.h"
#include <iostream>
#include <vector>
#include <stack>
#include <map>

using namespace std;

class StackMachine 
{
private:
	 const map<string, int> priorities = {
		{"*", 0}, {"/", 0}, {"%", 0}, {"+", 1}, {"-", 1},
		{">", 2}, {"<", 2}, {">=", 2}, {"<=", 2}, {"==", 3}, {"!=", 3},
		{"&&", 4}, {"||", 4}
	};
	vector<UnitClass> token_list;
	vector<UnitClass>::iterator token_list_it;
	vector<UnitClass>* current_line;
	vector<vector<UnitClass>> *code;

	void declaration_to_postfix();
	void initialization_to_postfix();
	void assignment_to_postfix();
	void function_call_to_postfix();
	void expression_to_postfix();
	void if_op_to_postfix();
	void elif_op_to_postfix();
	void else_op_to_postfix();
	void while_op_to_postfix();
	void for_op_to_postfix();
	void body_to_postfix();

	void show_current_line();
public:
	StackMachine(vector<UnitClass> *token_list = nullptr);
	void set_token_list(vector<UnitClass>*);
	void to_postfix();
	vector<vector<UnitClass>>* get_code();
};

#endif // !

