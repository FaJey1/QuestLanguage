#include "Interpreter.h"

Interpreter::Interpreter(vector<vector<UnitClass>>* code) : code(code) {}

Interpreter::~Interpreter() {
	delete code;
}

void Interpreter::exec_code() {
	stack<UnitClass> current_stack;
	for (auto& line : *code) {
		this->line_it = line.begin();
		int position = 0;

		while (this->line_it!=line.end()) {
			UnitClass element = *line_it;

			if (element.type == "operand_name" || element.type == "bool" ||
				element.type == "string" || element.type == "digit" ||
				element.type == "double" || element.type == "funtion_name") {
				current_stack.push(element);
			}
			else if (element.type == "comparisons" || element.type == "logOperation" ||
				element.type == "mathOperation") {
				auto el2 = current_stack.top();
				current_stack.pop();
				auto el1 = current_stack.top();
				current_stack.pop();

				auto res = this->do_binary_op(el1, el2, element);
				current_stack.push(res);
			}
			else if (element.type == "type_operand") {
				auto variable = current_stack.top();
				current_stack.pop();
				if (element.value == "BOOLEAN") {
					this->boolean_variables.insert({ variable.value, false });
				}
				else if (element.value == "STRING") {
					this->string_variables.insert({ variable.value, "" });
				} 
				else if (element.value == "INTEGER") {
					this->integer_variables.insert({ variable.value, 0 });
				}
				else if (element.value == "DOUBLE") {
					this->double_variables.insert({ variable.value, 0.0 });
				}
				current_stack.push(UnitClass("operand_name", variable.value));
			}
			else if (element.type == "assignment") {
				auto new_variable = current_stack.top();
				current_stack.pop();
				auto value = current_stack.top();
				current_stack.pop();

				this->do_assign_op(new_variable.value, value);
			}
			else if (element.type == "pFalse") {
				auto condition = current_stack.top();
				current_stack.pop();
				this->normalize_boolean(&condition);

				int p_false_pos = stoi(element.value.substr(0, element.value.find('!')));
				if ((bool)stoi(condition.value)) {
					line_it++;
					position++;
					continue;
				}

				while (position != p_false_pos - 1) {
					line_it++;
					position++;
				}
			}
			else if (element.type == "p" || element.type == "pEnd") {
				int p_pos = stoi(element.value.substr(0, element.value.find('!')));

				if (position <= p_pos) {
					while (position != p_pos - 1) {
						line_it++;
						position++;
					}
				}
				else {
					while (position != p_pos - 1) {
						if (line_it != line.begin()) {
							line_it--;
						}
						position--;
					}
				}
			}
			else if (element.type == "function_call") {
				vector<UnitClass> args;
				UnitClass function("", "");
				while (!current_stack.empty()) {
					auto el = current_stack.top();
					current_stack.pop();
					if (el.type != "funtion_name") {
						args.push_back(el);
					}
					else {
						function = el;
						break;
					}
				}

				if (function.value == "OUTPUT") {
					auto arg = args.back();
					if (arg.type == "operand_name") {
						this->specify_variable(&arg);
					}
					if (args.size() == 1) {
						printf(":-> %s\n", arg.value.c_str());
					}
				}
			}
			if (position != -1) {
				line_it++;
			}
			position++;
		}
		while (!current_stack.empty()) {
			current_stack.pop();
		}
	}
}

UnitClass Interpreter::do_binary_op(UnitClass el1, UnitClass el2, UnitClass op) {
	if (el1.type == "operand_name") {
		this->specify_variable(&el1);
	}
	if (el2.type == "operand_name") {
		this->specify_variable(&el2);
	}

	string type = el1.type;

	if (strcmp(type.c_str(), el2.type.c_str()) != 0 || el1.type.empty() || el2.type.empty()) {
		return UnitClass("undefined", "");
	}

	string res;
	if (type == "digit") {
		if (op.value == "+") {
			res = to_string(stoi(el1.value) + stoi(el2.value));
		}
		else if (op.value == "*") {
			res = to_string(stoi(el1.value) * stoi(el2.value));
		}
		else if (op.value == "-") {
			res = to_string(stoi(el1.value) - stoi(el2.value));
		}
		else if (op.value == "/") {
			res = to_string(stoi(el1.value) / stoi(el2.value));
		}
		else if (op.value == "%") {
			res = to_string(stoi(el1.value) % stoi(el2.value));
		}
		else if (op.value == "==") {
			res = to_string(stoi(el1.value) == stoi(el2.value));
		}
		else if (op.value == "!=") {
			res = to_string(stoi(el1.value) != stoi(el2.value));
		}
		else if (op.value == ">") {
			res = to_string(stoi(el1.value) > stoi(el2.value));
		}
		else if (op.value == "<") {
			res = to_string(stoi(el1.value) < stoi(el2.value));
		}
		else if (op.value == ">=") {
			res = to_string(stoi(el1.value) >= stoi(el2.value));
		}
		else if (op.value == "<=") {
			res = to_string(stoi(el1.value) <= stoi(el2.value));
		}
	}
	else if (type == "bool") {
		this->normalize_boolean(&el1);
		this->normalize_boolean(&el2);

		if (op.value == "==") {
			res = to_string(stoi(el1.value) == stoi(el2.value));
		}
		else if (op.value == "!=") {
			res = to_string(stoi(el1.value) != stoi(el2.value));
		}
		else if (op.value == ">") {
			res = to_string(stoi(el1.value) > stoi(el2.value));
		}
		else if (op.value == "<") {
			res = to_string(stoi(el1.value) < stoi(el2.value));
		}
		else if (op.value == ">=") {
			res = to_string(stoi(el1.value) >= stoi(el2.value));
		}
		else if (op.value == "<=") {
			res = to_string(stoi(el1.value) <= stoi(el2.value));
		}
		else if (op.value == "||") {
			res = to_string(stoi(el1.value) || stoi(el2.value));
		}
		else if (op.value == "&&") {
			res = to_string(stoi(el1.value) && stoi(el2.value));
		}
	}
	else if (type == "double") {
		if (op.value == "+") {
			res = to_string(stol(el1.value) + stol(el2.value));
		}
		else if (op.value == "*") {
			res = to_string(stol(el1.value) * stol(el2.value));
		}
		else if (op.value == "-") {
			res = to_string(stol(el1.value) - stol(el2.value));
		}
		else if (op.value == "/") {
			res = to_string(stol(el1.value) / stol(el2.value));
		}
		else if (op.value == "%") {
			res = to_string(stol(el1.value) % stol(el2.value));
		}
		else if (op.value == "==") {
			res = to_string(stol(el1.value) == stol(el2.value));
		}
		else if (op.value == "!=") {
			res = to_string(stol(el1.value) != stol(el2.value));
		}
		else if (op.value == ">") {
			res = to_string(stol(el1.value) > stol(el2.value));
		}
		else if (op.value == "<") {
			res = to_string(stol(el1.value) < stol(el2.value));
		}
		else if (op.value == ">=") {
			res = to_string(stol(el1.value) >= stol(el2.value));
		}
		else if (op.value == "<=") {
			res = to_string(stol(el1.value) <= stol(el2.value));
		}
	}
	else if (type == "string") {
		this->normalize_string(&el1);
		this->normalize_string(&el2);

		if (op.value == "+") {
			res = el1.value + el2.value;
		}
		else if (op.value == "==") {
			res = to_string(el1.value == el2.value);
		}
		else if (op.value == "!=") {
			res = to_string(el1.value != el2.value);
		}
	}
	if (op.type == "comparisons" || op.type == "logOperation") {
		type = "bool";
	}

	return UnitClass(type, res);
}

void Interpreter::do_assign_op(string var_name, UnitClass value) {
	if (value.type == "operand_name") {
		this->specify_variable(&value);
	}

	if (value.type == "digit") {
		auto var_it = this->integer_variables.find(var_name);
		if (var_it != this->integer_variables.end()) {
			var_it->second = stoi(value.value);
		}
	}
	else if (value.type == "bool") {
		this->normalize_boolean(&value);
		auto var_it = this->boolean_variables.find(var_name);
		if (var_it != this->boolean_variables.end()) {
			var_it->second = (bool)stoi(value.value);
		}
	}
	else if (value.type == "double") {
		auto var_it = this->double_variables.find(var_name);
		if (var_it != this->double_variables.end()) {
			var_it->second = stol(value.value);
		}
	}
	else if (value.type == "string") {
		this->normalize_string(&value);
		auto var_it = this->string_variables.find(var_name);
		if (var_it != this->string_variables.end()) {
			var_it->second = value.value;
		}
	}
}

void Interpreter::normalize_boolean(UnitClass* value) {
	if (strcmp(value->value.c_str(), "false") == 0) { value->value = "0"; }
	else if (strcmp(value->value.c_str(), "true") == 0) { value->value = "1"; }
}

void Interpreter::normalize_string(UnitClass* value) {
	if (value->value.at(0) == '"') {
		value->value = value->value.substr(1, value->value.size() - 2);
	}
}

template<typename T>
T Interpreter::normalize_value(UnitClass value) {
	if (value.type == "VARIABLE") { this->specify_variable(&value); }

	if (value.type == "INTEGER") { return std::stol(value.value); }
	else if (value.type == "STRING") { return value.value; }
	else if (value.type == "FLOAT") { return std::stod(value.value); }
	else if (value.type == "CONSTANT_KW") { return (bool)std::stoi(value.value); }
	return nullptr;
}

void Interpreter::specify_variable(UnitClass *variable) {
	string type, value;

	if (this->boolean_variables.contains(variable->value.data()))
	{
		type = "bool";
		auto buff = this->boolean_variables.find(variable->value.c_str());
		value = to_string(buff->second);
	}
	else if (this->double_variables.contains(variable->value.data()))
	{
		type = "double";
		auto buff = this->double_variables.find(variable->value.c_str());
		value = to_string(buff->second);
	}
	else if (this->integer_variables.contains(variable->value.data()))
	{
		type = "digit";
		auto buff = this->integer_variables.find(variable->value.c_str());
		value = to_string(buff->second);
	}
	else if (this->string_variables.contains(variable->value.data()))
	{
		type = "string";
		auto buff = this->string_variables.find(variable->value.c_str());
		value = buff->second;
	}

	variable->value = value;
	variable->type = type;
}

void Interpreter::show_variables() {
	cout << "Variables:\n";
	cout << "Boolean Variables:\n";
	for (const auto& [key, value] : this->boolean_variables) {
		cout << key << "\t" << value<<"\n";
	}
	cout << "Integer Variables:\n";
	for (const auto& [key, value] : this->integer_variables) {
		cout << key << "\t" << value << "\n";
	}
	cout << "Double Variables:\n";
	for (const auto& [key, value] : this->double_variables) {
		cout << key << "\t" << value << "\n";
	}
	cout << "String Variables:\n";
	for (const auto& [key, value] : this->string_variables) {
		cout << key << "\t" << value << "\n";
	}
}

