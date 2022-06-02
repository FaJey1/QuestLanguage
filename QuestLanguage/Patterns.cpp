#include "Patterns.h"

Patterns::Patterns()
{
	KeyValue key_value;

	// Операции
	key_value.pair("string", "(\".{0,150}\")");
	this->patterns.push_back(key_value);

	key_value.pair("increment", "([+][1-9]+)|(\\+\\+)");
	this->patterns.push_back(key_value);

	key_value.pair("decrement", "([-][1-9]+)|(\\-\\-)");
	this->patterns.push_back(key_value);

	key_value.pair("assignment", "(\\=)");
	this->patterns.push_back(key_value);

	key_value.pair("comparisons", "(\\=\\=)");
	this->patterns.push_back(key_value);

	key_value.pair("comparisons", "(\\!\\=)");
	this->patterns.push_back(key_value);

	key_value.pair("comparisons", "(\\>)");
	this->patterns.push_back(key_value);

	key_value.pair("comparisons", "(\\<)");
	this->patterns.push_back(key_value);

	key_value.pair("comparisons", "(\\>\\=)");
	this->patterns.push_back(key_value);

	key_value.pair("comparisons", "(\\<\\=)");
	this->patterns.push_back(key_value);

	key_value.pair("mathOperation", "(\\+)");
	this->patterns.push_back(key_value);

	key_value.pair("mathOperation", "(\\*)");
	this->patterns.push_back(key_value);

	key_value.pair("mathOperation", "(\\-)");
	this->patterns.push_back(key_value);

	key_value.pair("mathOperation", "(\\/)");
	this->patterns.push_back(key_value);

	key_value.pair("mathOperation", "(\\%)");
	this->patterns.push_back(key_value);

	key_value.pair("increment", "([\\+][1\\+9]\\+)|(\\+\\+)");
	this->patterns.push_back(key_value);

	key_value.pair("decrement", "([\\-][1\\-9]\\+)|(\\-\\-)");
	this->patterns.push_back(key_value);

	key_value.pair("logOperation", "(\\|\\|)");
	this->patterns.push_back(key_value);

	key_value.pair("logOperation", "(\\&\\&)");
	this->patterns.push_back(key_value);

	//key_value.pair("logOperation", "(\\~)");
	//this->patterns.push_back(key_value);
	//Простые типы данных
	key_value.pair("type_operand", "(^BOOLEAN)");
	this->patterns.push_back(key_value);

	key_value.pair("type_operand", "(^STRING)");
	this->patterns.push_back(key_value);

	key_value.pair("type_operand", "(^INTEGER)");
	this->patterns.push_back(key_value);

	key_value.pair("type_operand", "(^DOUBLE)");
	this->patterns.push_back(key_value);

	key_value.pair("operand_name", "([^|A-Z][a\-z|_]\{1\,3\}[_|aA\-zZ|0\-9]\{4,50})");
	this->patterns.push_back(key_value);

	key_value.pair("digit", "[1-9]{0}[0-9]{1,100}");
	this->patterns.push_back(key_value);

	key_value.pair("double", "(^[0-9]|[1-9][0-9]{0,49})\\.[0-9]{50,100}");
	this->patterns.push_back(key_value);

	key_value.pair("bool", "(\^true\|\^false)");
	this->patterns.push_back(key_value);

	key_value.pair("tab", "(	)");
	this->patterns.push_back(key_value);

	//Составные типы данных



	//Условные оператор и циклы
	key_value.pair("log", "(IF)");
	this->patterns.push_back(key_value);

	key_value.pair("log", "(THEN)");
	this->patterns.push_back(key_value);

	key_value.pair("log", "(ELIF)");
	this->patterns.push_back(key_value);

	key_value.pair("log", "(ELSE)");
	this->patterns.push_back(key_value);

	key_value.pair("vet", "(SWITCH)");
	this->patterns.push_back(key_value);

	key_value.pair("vet", "(CASE)");
	this->patterns.push_back(key_value);

	key_value.pair("vet", "(DEFAULT)");
	this->patterns.push_back(key_value);

	key_value.pair("cyc", "(WHILE)");
	this->patterns.push_back(key_value);

	key_value.pair("cyc", "(FOR)");
	this->patterns.push_back(key_value);

	key_value.pair("cyc", "(DO)");
	this->patterns.push_back(key_value);

	key_value.pair("cyc", "(END)");
	this->patterns.push_back(key_value);

	key_value.pair("cyc", "(RANGE)");
	this->patterns.push_back(key_value);

	key_value.pair("cyc", "(STEP)");
	this->patterns.push_back(key_value);

	key_value.pair("right_bracket_p", "(\\])");
	this->patterns.push_back(key_value);

	key_value.pair("left_bracket_p", "(\\[)");
	this->patterns.push_back(key_value);

	//функции
	key_value.pair("funtion_name", "[A\-Z][\_\|A\-Z\|0\-9]\{1\,5\}[A\-Z][\_\|A\-Z\|0\-9]\+");
	this->patterns.push_back(key_value);

	//Прочее
	key_value.pair("right_bracket", "(\\))");  
	this->patterns.push_back(key_value);

	key_value.pair("left_bracket", "(\\()");
	this->patterns.push_back(key_value);

	key_value.pair("comma", "(\\,)");
	this->patterns.push_back(key_value);

	key_value.pair("dot", "(\\.)");
	this->patterns.push_back(key_value);

	key_value.pair("space", "( )");
	this->patterns.push_back(key_value);

	key_value.pair("EOF", "(END OF FILE)");
	this->patterns.push_back(key_value);
}

string Patterns::get_type(string lexem)
{
	for (int i = 0; i < this->patterns.size(); i++) {
		if (regex_search(lexem, regex(this->patterns[i].regexp))){
			return this->patterns[i].key;
		}
	}
	return "typeIsInvalid";
}

void Patterns::print_patterns()
{
	for (int i = 0; i < this->patterns.size(); i++) {
		cout << patterns[i].key << " " << patterns[i].regexp << " }" << endl;
	}
}
