#include "StackMachine.h"

StackMachine::StackMachine(vector<UnitClass> *token_list) : token_list(*token_list), token_list_it(this->token_list.begin()), current_line(nullptr),
	code(new vector<vector<UnitClass>>()) {}

void StackMachine::set_token_list(vector<UnitClass>* token_list) {
	this->token_list = *token_list;
	this->token_list_it = this->token_list.begin();
}

void StackMachine::show_current_line() {
	for (const auto& token : *current_line) {
		cout << token.value << " ";
	}
	cout << "\n";
}

vector<vector<UnitClass>>* StackMachine::get_code() {
	return code;
}

void StackMachine::to_postfix() {
	while (token_list_it->type != "EOF") {
		current_line = new vector<UnitClass>();
		if (token_list_it->type == "type_operand") {
			token_list_it++;
			token_list_it++;
			auto what = *token_list_it;
			token_list_it--;
			token_list_it--;
			if (what.type == "comma" || what.type == "dot") {
				this->declaration_to_postfix();
			}
			else {
				this->initialization_to_postfix();
			}
		}
		else if (token_list_it->type == "operand_name") {
			token_list_it++;
			auto what = *token_list_it;
			token_list_it--;
			if (what.type == "assignment") {
				this->assignment_to_postfix();
			}
			else {
				this->expression_to_postfix();
				token_list_it++;
			}
		}
		else if (token_list_it->type == "funtion_name") {
			this->function_call_to_postfix();
		}
		else if (token_list_it->value == "IF") {
			this->if_op_to_postfix();
		}
		else if (token_list_it->value == "FOR") {
			this->for_op_to_postfix();
		}
		else if (token_list_it->value == "WHILE") {
			this->while_op_to_postfix();
		}

		current_line->push_back(UnitClass("end", "_|_"));
		code->push_back(*current_line);
		current_line->clear();
		delete current_line;
		current_line = nullptr;
	}
}

void StackMachine::declaration_to_postfix() {
	// identifier
	auto id = *token_list_it;
	token_list_it++;
	// variable
	auto var = *token_list_it;
	token_list_it++;
	current_line->push_back(var);
	current_line->push_back(id);

	while (token_list_it->type != "dot") {
		token_list_it++;

		current_line->push_back(*token_list_it);
		current_line->push_back(id);
		token_list_it++;
	}
	token_list_it++;
}

void StackMachine::initialization_to_postfix() {
	// identifier
	auto id = *token_list_it;
	token_list_it++;
	// variable
	auto current_var = *token_list_it;
	token_list_it++;
	// = 
	auto assignment_op = *token_list_it;
	token_list_it++;
	// value
	this->expression_to_postfix();
	current_line->push_back(current_var);
	current_line->push_back(id);
	current_line->push_back(assignment_op);

	while (token_list_it->type != "dot") {
		token_list_it++;

		current_var = *token_list_it;
		token_list_it++;

		assignment_op = *token_list_it;
		token_list_it++;

		this->expression_to_postfix();
		current_line->push_back(current_var);
		current_line->push_back(id);
		current_line->push_back(assignment_op);
	}
	token_list_it++;
}

void StackMachine::assignment_to_postfix() {
	// variable
	auto current_var = *token_list_it;
	token_list_it++;
	// = 
	auto assignment_op = *token_list_it;
	token_list_it++;
	// value
	this->expression_to_postfix();
	current_line->push_back(current_var);
	current_line->push_back(assignment_op);

	while (token_list_it->type != "dot") {
		token_list_it++;

		current_var = *token_list_it;
		token_list_it++;

		assignment_op = *token_list_it;
		token_list_it++;

		this->expression_to_postfix();
		current_line->push_back(current_var);
		current_line->push_back(assignment_op);
		token_list_it++;
	}
	token_list_it++;
}

void StackMachine::function_call_to_postfix() {
	// function_name
	current_line->push_back(*token_list_it);
	token_list_it++;
	token_list_it++;
	
	// args
	if (token_list_it->type != "right_bracket") {
		current_line->push_back(*token_list_it);
		token_list_it++;
	}
	while (token_list_it->type != "right_bracket") {
		token_list_it++;
		current_line->push_back(*token_list_it);
		token_list_it++;
	}
	current_line->push_back(UnitClass("function_call", "callee"));
	token_list_it++;
	token_list_it++;
}

void StackMachine::expression_to_postfix() {
	stack<UnitClass> stack;
	while (token_list_it->type != "dot" && token_list_it->type != "comma" && 
		   token_list_it->value != "DO" && token_list_it->value != "THEN") {
		if (token_list_it->type == "operand_name" || token_list_it->type == "bool" || token_list_it->type == "string" ||
			token_list_it->type == "double" || token_list_it->type == "digit") {
			current_line->push_back(*token_list_it);
		}
		else if (token_list_it->type == "left_bracket") {
			stack.push(*token_list_it);
		}
		else if (token_list_it->type == "right_bracket") {
			while (stack.top().type != "left_bracket") {
				current_line->push_back(stack.top());
				stack.pop();
			}
			stack.pop();
		}
		else if (token_list_it->type == "mathOperation" || token_list_it->type == "logOperation" || token_list_it->type == "comparisons") {
			int cur_priority = priorities.at(token_list_it->value);
			if (!stack.empty()) {
				while (stack.top().type == "mathOperation" || stack.top().type == "logOperation" || stack.top().type == "comparisons") {
					int other_priority = priorities.at(stack.top().value);
					if (other_priority <= cur_priority) {
						current_line->push_back(stack.top());
						stack.pop();
					}
					else {
						break;
					}
					if (stack.empty()) {
						break;
					}
				}
			}
			stack.push(*token_list_it);
		}
		token_list_it++;
	}
	while (!stack.empty()) {
		current_line->push_back(stack.top());
		stack.pop();
	}
}

void StackMachine::if_op_to_postfix() {
	vector<int> jumps;
	vector<int>::iterator jumpIt;
	const int start = current_line->empty() ? 0 : current_line->size();

	// IF ... THEN
	token_list_it++;
	this->expression_to_postfix();
	token_list_it++;
	current_line->push_back(UnitClass("pFalse", "p!F"));

	// body
	this->body_to_postfix();
	current_line->push_back(UnitClass("pEnd", "p!"));
	jumps.push_back(current_line->size());

	// ELIF ... ELSE ... END .
	while (token_list_it->value == "ELIF") {
		this->elif_op_to_postfix();
	}
	if (token_list_it->value == "ELSE") {
		this->else_op_to_postfix();
	}

	jumps.push_back(current_line->size());

	jumpIt = jumps.begin();
	for (int i = start; i < current_line->size(); i++) {
		if ((*current_line)[i].value == "p!F") {
			string pos = std::to_string(*jumpIt);
			(*current_line)[i].value.replace(0, 1, pos);
			jumpIt++;
		}
		else if ((*current_line)[i].type == "pEnd") {
			string pos = std::to_string(current_line->size());
			(*current_line)[i].value.replace(0, 1, pos);
		}
	}

	token_list_it++;
	token_list_it++;
}

void StackMachine::elif_op_to_postfix() {
	// ELIF ... THEN
	token_list_it++;
	this->expression_to_postfix();
	token_list_it++;
	current_line->push_back(UnitClass("pFalse", "p!F"));
	token_list_it++;

	// body
	this->body_to_postfix();
	current_line->push_back(UnitClass("pEnd", "p!"));}

void StackMachine::else_op_to_postfix() {
	token_list_it++;

	//body
	this->body_to_postfix();
}

void StackMachine::while_op_to_postfix() {
	vector<int> jumps;
	vector<int>::reverse_iterator jumpIt;
	const int start = current_line->empty() ? 0 : current_line->size();

	jumps.push_back(current_line->size());

	// WHEN .. DO
	token_list_it++;
	this->expression_to_postfix();
	token_list_it++;
	current_line->push_back(UnitClass("pFalse", "p!F"));

	// body
	this->body_to_postfix();
	current_line->push_back(UnitClass("p", "p!"));
	jumps.push_back(current_line->size());

	jumpIt = jumps.rbegin();
	for (int i = start; i < current_line->size(); i++) {
		if (((*current_line)[i].type == "pFalse" || (*current_line)[i].type == "p") && (*current_line)[i].value[0] == 'p') {
			string pos = std::to_string(*jumpIt);
			(*current_line)[i].value.replace(0, 1, pos);
			jumpIt++;
		}
	}
	
	token_list_it++;
	token_list_it++;
}

void StackMachine::for_op_to_postfix() {
	vector<int> jumps;
	vector<int>::reverse_iterator jumpIt;
	const int start = current_line->empty() ? 0 : current_line->size();

	// FOR VAR RANGE [VAL,
	token_list_it++;
	auto var = *token_list_it;
	token_list_it++;
	token_list_it++;
	auto assignment = UnitClass("assignment", "=");
	token_list_it++;
	auto val = *token_list_it;
	token_list_it++;
	current_line->push_back(val);
	current_line->push_back(var);
	current_line->push_back(assignment);
	jumps.push_back(current_line->size());

	// VAL]
	token_list_it++;
	val = *token_list_it;
	auto is_less = UnitClass("comparisons", "<");
	token_list_it++;
	current_line->push_back(var);
	current_line->push_back(val);
	current_line->push_back(is_less);

	token_list_it++;
	current_line->push_back(UnitClass("pFalse", "p!F"));
	token_list_it++;

	this->body_to_postfix();
	current_line->push_back(UnitClass("digit", "1"));
	current_line->push_back(var);
	current_line->push_back(UnitClass("mathOperation", "+"));
	current_line->push_back(var);
	current_line->push_back(assignment);

	current_line->push_back(UnitClass("p", "p!"));
	jumps.push_back(current_line->size());

	jumpIt = jumps.rbegin();
	for (int i = start; i < current_line->size(); i++) {
		if (((*current_line)[i].type == "pFalse" || (*current_line)[i].type == "p") && (*current_line)[i].value[0] == 'p') {
			string pos = std::to_string(*jumpIt);
			(*current_line)[i].value.replace(0, 1, pos);
			jumpIt++;
		}
	}

	token_list_it++;
	token_list_it++;
}

void StackMachine::body_to_postfix() {
	while (token_list_it->value != "END" && token_list_it->value != "ELIF" && token_list_it->value != "ELSE") {
		if (token_list_it->type == "type_operand") {
			token_list_it++;
			token_list_it++;
			auto what = *token_list_it;
			token_list_it--;
			token_list_it--;
			if (what.type == "comma" || what.type == "dot") {
				this->declaration_to_postfix();
			}
			else {
				this->initialization_to_postfix();
			}
		}
		else if (token_list_it->type == "operand_name") {
			token_list_it++;
			auto what = *token_list_it;
			token_list_it--;
			if (what.type == "assignment") {
				this->assignment_to_postfix();
			}
			else {
				this->expression_to_postfix();
				token_list_it++;
			}
		}
		else if (token_list_it->type == "funtion_name") {
			this->function_call_to_postfix();
		}
		else if (token_list_it->value == "IF") {
			this->if_op_to_postfix();
		}
		else if (token_list_it->value == "FOR") {
			this->for_op_to_postfix();
		}
		else if (token_list_it->value == "WHILE") {
			this->while_op_to_postfix();
		}
		else if (token_list_it->type == "tab") {
			token_list_it++;
		}
	}
}