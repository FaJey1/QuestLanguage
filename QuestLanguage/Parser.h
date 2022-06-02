#ifndef _PARSER_
#define _PARSER_

#include <iostream>
#include <iomanip>
#include <fstream>
#include "Patterns.h"
#include "ArrayByFaJey.h"
#include "UnitClass.h"

using namespace std;

class Parser
{
private:
	vector <UnitClass> token_list;
	vector<UnitClass>::iterator token_list_it;
	
	// non-terminals
	bool expr();
	bool declaration(int depth = 1);
	bool initialization(int depth = 1);
	bool assignment(int depth = 1);
	bool function_call(int depth = 1);
	bool expression(int depth = 1);
	bool math_expression();
	bool cond_expression();
	bool if_op(int depth = 1);
	bool elif_op(int depth = 1);
	bool else_op(int depth = 1);
	bool while_op(int depth = 1);
	bool for_op(int depth = 1);
	bool range();
	bool body(int depth = 1);
	bool value();

	// terminals
	bool identifier();
	bool variable();
	bool bool_value();
	bool string_value();
	bool integer_value();
	bool double_value();
	bool logical_op();
	bool function();
	bool math_op();
	bool unary_op();
	bool assignment_op();
	bool left_bracket();
	bool right_bracket();
	bool if_();
	bool elif_();
	bool else_();
	bool do_();
	bool then();
	bool end();
	bool while_();
	bool for_();
	bool range_();
	bool left_square_bracket();
	bool right_square_bracket();
	bool tab();
	bool comma();
	bool dot();
public:
	Parser(vector<UnitClass> *token_list = nullptr);
	void add_token_list(vector<UnitClass>* token_list);
	int lang();
};

#endif

// GRAMMAR
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// lang -> expr*
// expr -> (declaration | initialization | assignment | function_call | expression | if_op | while_op | for_op)
// declaration -> identifier variable (coma variable)* dot
// initialization -> identifier variable assignment_op (math_expression | cond_expression) (comma variable assignment_op (math_expression | cond_expression))* dot
// assignment -> variable assignment_op (math_expression | cond_expression) (comma variable assignment_op (math_expression | cond_expression))* dot
// function_call -> function left_bracket (value (comma value)*)? right_bracket dot
// expression -> (math_expression | cond_expression) dot
// math_expression -> (value | (value math_op math_expression) | (left_bracket math_expression right_bracket) | (left_bracket math_expression right_bracket math_op math_expression))
// cond_expression -> (value | (value logical_op cond_expression) | (left_bracket cond_expression right_bracket) | (left_bracket cond_expression right_bracket logical_op cond_expression))
// if_op -> if_ cond_expression then body elif_op* else_op? end dot
// elif_op -> elif_ cond_expression then body
// else_op -> else_ body
// while_op -> while_ cond_expression do_ body end dot
// for_op -> for_ variable range do_ body end dot
// range -> range_ left_square_bracket value right_square_bracket
// body -> (tab (declaration | initialization | assignment | function_call | expression | if_op | while_op | for_op))*
// value -> (variable | bool_value | string_value | integer_value | double_value)
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// identifier -> type: "type_operand"
// variable -> type: "operand_name"
// bool_value -> type: "bool"
// string_value -> type: "string"
// integer_value -> type: "digit"
// double_value -> type: "double"
// logical_op -> (type: "comparisons" | type: "logOperation")
// math_op -> type: "mathOperation"
// assignment_op -> type: "assignment"
// left_bracket -> type: "left_bracket"
// right_bracket -> type: "right_bracket"
// if_ -> value: "IF"
// elif_ -> value: "ELIF"
// else_ -> value: "ELSE"
// do_ -> value: "DO"
// then -> value: "THEN"
// end -> value: "END"
// while_ -> value: "WHILE"
// for_ -> value: "FOR"
// range_ -> value: "RANGE"
// left_square_bracket -> type: "left_bracket_p"
// right_square_bracket -> type: "right_bracket_p"
// tab -> type: "tab"
// comma -> type: "comma"
// dot -> type: "dot"
// function -> type: "funtion_name"
// -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
