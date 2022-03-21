#include "Lexer.h"
Lexer::Lexer() {

}
void Lexer::set_line(string str) {
	this->line_for_analysis = str;
}
void Lexer::start_analysis() {
	string word;
	vector <string> words_array;

	cout << "input line is:" << this->line_for_analysis << endl;

	if (line_for_analysis == " " || line_for_analysis == "\n") {
		cout << "Empty line";
	}

	for (int i = 0; i < line_for_analysis.length(); i++) {
		if (line_for_analysis[i] == ' ') {
			words_array.push_back(word);
			//cout << word << "\n"; // отладка

			word = "";
		}
		else if (line_for_analysis[i] == '\n') {
			//cout << word; // отладка

			word = "";
		}
		else {
			word += line_for_analysis[i];
		}
	}
}
void Lexer::show_elements_in_line() {

}
