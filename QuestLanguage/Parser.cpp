#include "Parser.h"

Parser::Parser(vector<UnitClass>* token_list) : token_list(*token_list), token_list_it(this->token_list.begin()) {}

void Parser::add_token_list(vector<UnitClass>* token_list) {
	this->token_list = *token_list;
}

int Parser::lang() {
	if (token_list_it == token_list.end()) {
		return 0;
	}

	while (token_list_it->type != "EOF") {
		if (!this->expr()) {
			return 1;
		}
	}

	return 0;
}

bool Parser::expr() {
	vector<bool(Parser::*)(int)> non_terminals = {
		&Parser::declaration, &Parser::initialization, &Parser::assignment, &Parser::function_call, &Parser::expression, &Parser::if_op, &Parser::while_op, &Parser::for_op
	};

	auto fixed_it = token_list_it;
	while (token_list_it->type != "EOF") {
		for (auto non_terminal : non_terminals) {
			bool res = (this->*(non_terminal))(1);
			if (res) {
				return res;
			}
			else {
				token_list_it = fixed_it;
				continue;
			}
		}
		return false;
	}

	return false;
}

bool Parser::declaration(int depth) {
	if (!this->identifier()) {
		return false;
	}

	if (!this->variable()) {
		return false;
	}

	while (token_list_it->type != "EOF") {
		if (!this->comma()) {
			break;
		}

		if (!this->variable()) {
			return false;
		}
	}

	if (this->dot()) {
		return true;
	}
	else {
		return false;
	}
}

bool Parser::initialization(int depth) {
	if (!this->identifier()) {
		return false;
	}

	if (!this->variable()) {
		return false;
	}

	if (!this->assignment_op()) {
		return false;
	}

	if (!this->math_expression()) {
		if (!this->cond_expression()) {
			return false;
		}
	}
	
	while (token_list_it->type != "EOF") {
		if (!this->comma()) {
			break;
		}

		if (!this->variable()) {
			return false;
		}
		if (!this->assignment_op()) {
			return false;
		}
		if (!this->math_expression()) {
			if (!this->cond_expression()) {
				return false;
			}
		}
	}

	if (this->dot()) {
		return true;
	}
	else {
		return false;
	}
}

bool Parser::assignment(int depth) {
	if (!this->variable()) {
		return false;
	}
	if (!this->assignment_op()) {
		return false;
	}
	if (!this->math_expression()) {
		if (!this->cond_expression()) {
			return false;
		}
	}

	while (token_list_it->type != "EOF") {
		if (!this->comma()) {
			break;
		}

		if (!this->variable()) {
			return false;
		}
		if (!this->assignment_op()) {
			return false;
		}
		if (!this->math_expression()) {
			if (!this->cond_expression()) {
				return false;
			}
		}
	}

	if (this->dot()) {
		return true;
	}
	else {
		return false;
	}
}

bool Parser::function_call(int depth) {
	if (!this->function()) {
		return false;
	}
	if (!this->left_bracket()) {
		return false;
	}
	if (this->value()) {
		while (token_list_it->type != "EOF") {
			if (!this->comma()) {
				break;
			}

			if (!this->value()) {
				return false;
			}
		}
	}
	if (!this->right_bracket()) {
		return false;
	}

	if (this->dot()) {
		return true;
	}
	else {
		return false;
	}
}

bool Parser::expression(int depth) {
	if (!this->math_expression()) {
		if (!this->cond_expression()) {
			return false;
		}
	}
	if (this->dot()) {
		return true;
	}
	else {
		return false;
	}
}

bool Parser::math_expression() {
	if (this->left_bracket()) {
		if (!this->math_expression()) {
			return false;
		}
		if (!this->right_bracket()) {
			return false;
		}

		else if (this->math_op()) {
			if (this->math_expression()) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return true;
		}
	}

	if (!this->value()) {
		return false;
	}
	if (this->math_op()) {
		if (this->math_expression()) {
			return true;
		}
		else {
			return false;
		}
	}
	/*else if (this->dot()) {
		return true;
	}*/

	return true;
}

bool Parser::cond_expression() {
	if (this->left_bracket()) {
		if (!this->cond_expression()) {
			return false;
		}
		if (!this->right_bracket()) {
			return false;
		}

		else if (this->logical_op()) {
			if (this->cond_expression()) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	if (!this->value()) {
		return false;
	}
	if (this->logical_op()) {
		if (this->cond_expression()) {
			return true;
		}
		else {
			return false;
		}
	}
	/*else if (this->dot()) {
		return true;
	}*/

	return true;
}

bool Parser::if_op(int depth) {
	if (!this->if_()) {
		return false;
	}
	if (!this->cond_expression()) {
		return false;
	}
	if (!this->then()) {
		return false;
	}
	if (!this->body(depth)) {
		return false;
	}
	while (token_list_it->type != "EOF") {
		if (!this->elif_op(depth)) {
			break;
		}
	}
	this->else_op(depth);
	if (!this->end()) {
		return false;
	}
	if (this->dot()) {
		return true;
	}
	else {
		return false;
	}
}

bool Parser::elif_op(int depth) {
	if (!this->elif_()) {
		return false;
	}
	if (!this->cond_expression()) {
		return false;
	}
	if (!this->then()) {
		return false;
	}
	if (!this->body(depth)) {
		return false;
	}
	return true;
}

bool Parser::else_op(int depth) {
	if (!this->else_()) {
		return false;
	}
	if (!this->body(depth)) {
		return false;
	}
	return true;
}

bool Parser::while_op(int depth) {
	if (!this->while_()) {
		return false;
	}
	if (!this->cond_expression()) {
		return false;
	}
	if (!this->do_()) {
		return false;
	}
	if (!this->body(depth)) {
		return false;
	}
	if (!this->end()) {
		return false;
	}
	if (this->dot()) {
		return true;
	}
	else {
		return false;
	}
}

bool Parser::for_op(int depth) {
	if (!this->for_()) {
		return false;
	}
	if (!this->variable()) {
		return false;
	}
	if (!this->range()) {
		return false;
	}
	if (!this->do_()) {
		return false;
	}
	if (!this->body(depth)) {
		return false;
	}
	if (!this->end()) {
		return false;
	}
	if (this->dot()) {
		return true;
	}
	else {
		return false;
	}
}

bool Parser::range() {
	if (!this->range_()) {
		return false;
	}
	if (!this->left_square_bracket()) {
		return false;
	}
	if (!this->value()) {
		return false;
	}
	if (!this->comma()) {
		return false;
	}
	if (!this->value()) {
		return false;
	}
	if (!this->right_square_bracket()) {
		return false;
	}
	return true;
}

bool Parser::body(int depth) {
	vector<bool(Parser::*)(int)> non_terminals = {
		&Parser::declaration, &Parser::initialization, &Parser::assignment, &Parser::function_call, &Parser::expression, &Parser::if_op, &Parser::while_op, &Parser::for_op
	};

	auto fixed_it = token_list_it;
	while (token_list_it->type != "EOF") {
		for (int i = 0; i < depth - 1; i++) {
			if (!this->tab()) {
				return false;
			}
		}
		if (!this->tab()) {
			return true;
		}
		fixed_it = token_list_it;
		int i = 0;
		for (auto non_terminal : non_terminals) {
			bool res = (this->*(non_terminal))(depth+1);
			if (res) {
				fixed_it = token_list_it;
				break;
			}
			else {
				i++;
				token_list_it = fixed_it;
				continue;
			}
		}
		if (i == 8) {
			return false;
		}
	}
	return false;
}

bool Parser::value() {
	vector<bool(Parser::*)()> terminals = {
		&Parser::variable, &Parser::bool_value, &Parser::string_value, &Parser::integer_value, &Parser::double_value
	};

	for (auto terminal : terminals) {
		bool res = (this->*(terminal))();
		if (res) {
			return res;
		}
		else {
			continue;
		}
	}
	return false;
}

bool Parser::identifier() {
	if (token_list_it->type == "type_operand") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::variable() {
	if (token_list_it->type == "operand_name") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::bool_value() {
	if (token_list_it->type == "bool") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::string_value() {
	if (token_list_it->type == "string") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::integer_value() {
	if (token_list_it->type == "digit") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::double_value() {
	if (token_list_it->type == "double") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::logical_op() {
	if (token_list_it->type == "comparisons" || token_list_it->type == "logOperation") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::math_op() {
	if (token_list_it->type == "mathOperation") {
		token_list_it++;
		return true;
	}
	return false;
}

// doesn't work lol
bool Parser::unary_op() {
	return true;
}

bool Parser::assignment_op() {
	if (token_list_it->type == "assignment") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::left_bracket() {
	if (token_list_it->type == "left_bracket") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::right_bracket() {
	if (token_list_it->type == "right_bracket") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::if_() {
	if (token_list_it->value == "IF") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::elif_() {
	if (token_list_it->value == "ELIF") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::else_() {
	if (token_list_it->value == "ELSE") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::do_() {
	if (token_list_it->value == "DO") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::then() {
	if (token_list_it->value == "THEN") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::end() {
	if (token_list_it->value == "END") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::while_() {
	if (token_list_it->value == "WHILE") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::for_() {
	if (token_list_it->value == "FOR") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::range_() {
	if (token_list_it->value == "RANGE") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::left_square_bracket() {
	if (token_list_it->type == "left_bracket_p") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::right_square_bracket() {
	if (token_list_it->type == "right_bracket_p") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::tab() {
	if (token_list_it->type == "tab") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::comma() {
	if (token_list_it->type == "comma") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::dot() {
	if (token_list_it->type == "dot") {
		token_list_it++;
		return true;
	}
	return false;
}

bool Parser::function() {
	if (token_list_it->type == "funtion_name") {
		token_list_it++;
		return true;
	}
	return false;
}