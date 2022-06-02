#include "Lexer.h"

Lexer::Lexer(string filename) {
	this->read_in_file(filename);
}

void Lexer::string_for_analysis(string line_for_analysis, int line) {
	string word = "", w, w2;
	int quotation_marks_check = 0;
	int inc_dec_check = 0;
	int check_simple_inc_dec = 0;

	if (line_for_analysis == "") {
		//cout << "Empty line";
		return;
	}
	line_for_analysis += " ";
	for (int i = 0; i < line_for_analysis.size()-1; i++) {
		word += line_for_analysis[i];
		w = line_for_analysis[i+1];
		w2 = line_for_analysis[i];
		if (line_for_analysis[i] == '\"') {
			quotation_marks_check++;
		}
		if ((w == "+" || w == "-" || patterns.get_type(w) == "digit") && (w2 == "+" || w2 == "-") && check_simple_inc_dec == 0) {
			inc_dec_check = 1;
			check_simple_inc_dec = 1;
			continue;
		}

		if (line_for_analysis[i] == '"') {
			//cout << word << endl;
		}

		if (patterns.get_type(word) != "typeIsInvalid" &&
			(patterns.get_type(w) == "comma" || patterns.get_type(w) == "assignment" || 
			patterns.get_type(w) == "space" || patterns.get_type(w) == "dot" || patterns.get_type(w) == "comparisons" || 
			patterns.get_type(w) == "mathOperation" || patterns.get_type(w) == "logOperation" || patterns.get_type(w) == "left_bracket_p" || 
			patterns.get_type(w) == "right_bracket_p" || patterns.get_type(w) == "right_bracket" || patterns.get_type(w) == "left_bracket") &&
			(patterns.get_type(w2) != "comma" || patterns.get_type(w2) != "assignment" || patterns.get_type(w2) != "space" ||
			 patterns.get_type(w2) != "dot" || patterns.get_type(w2) != "comparisons" || patterns.get_type(w2) != "mathOperation" ||
		     patterns.get_type(w2) != "logOperation" || patterns.get_type(w2) != "right_bracket" || patterns.get_type(w2) != "left_bracket") && 
			(quotation_marks_check == 2 || quotation_marks_check == 0) && inc_dec_check == 0) {

			if (std::strcmp(word.c_str(), " ") != 0) { 
				this->write_in_token_list(word); 
			}
			word = "";
			w2 = "";
			w = "";
			quotation_marks_check = 0;
		}
		if ((patterns.get_type(w2) == "comma" || patterns.get_type(w2) == "assignment" || patterns.get_type(w2) == "space" ||
			patterns.get_type(w2) == "dot" || patterns.get_type(w2) == "comparisons" || patterns.get_type(w2) == "mathOperation" ||
			patterns.get_type(w2) == "logOperation") && (quotation_marks_check == 2 || quotation_marks_check == 0) && inc_dec_check == 0) {
			if (std::strcmp(w2.c_str(), " ") != 0) { 
				this->write_in_token_list(w2); 
			}
			w2 = "";
			w = "";
			word = "";
		}
		if (inc_dec_check == 1 && word != "") {
			if (std::strcmp(w2.c_str(), " ") != 0) {
				this->write_in_token_list(w2); 
			}
			word = "";
			w2 = "";
			w = "";
			inc_dec_check = 0;
			check_simple_inc_dec = 0;
		}
		if (patterns.get_type(word) == "right_bracket_p" || patterns.get_type(word) == "left_bracket_p") {
			if (std::strcmp(word.c_str(), " ") != 0) { 
				this->write_in_token_list(w2); 
			}
			word = "";
			w2 = "";
			w = "";
		}
		if (patterns.get_type(word) == "tab") {
			if (std::strcmp(w2.c_str(), " ") != 0) { 
				this->write_in_token_list(w2); 
			}
			word = "";
			w2 = "";
			w = "";
		}
		if (patterns.get_type(word) == "right_bracket" || patterns.get_type(word) == "left_bracket") {
			if (std::strcmp(w2.c_str(), " ") != 0) { 
				this->write_in_token_list(w2); 
			}
			word = "";
			w2 = "";
			w = "";
		}
	}
}

void Lexer::write_in_token_list(string word)
{
	UnitClass unit(patterns.get_type(word), word);
	token_list.push_back(unit);
}

void Lexer::read_in_file(string name)
{
	string text;
	int line = 1;
	ifstream file(name);
	while (true) {
		getline(file, text);
		if (!file.eof()) {
			this->string_for_analysis(text, line);
			line++;
			//cout << text << endl;
		}
		else break;
	}
	token_list.push_back({"EOF", "END OF FILE"});
}

void Lexer::read_in_commandline()
{
}

vector <UnitClass> Lexer::get_token_list()
{
	return this->token_list;
}

void Lexer::show_token_list() {
	cout << setw(15) << "Number of tokens: " << setw(15) << this->token_list.size() << endl;
	for (int i = 0; i < this->token_list.size(); i++) {
		cout << setw(15) << this->token_list[i].type << setw(15) << "{" << this->token_list[i].value << "}" << endl;
	}
}
