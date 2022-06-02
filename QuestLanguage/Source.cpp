#include "Lexer.h"
#include "Parser.h"
#include "StackMachine.h"
#include "Interpreter.h"

int main()
{
    Lexer lexer("testfinal.txt");

    lexer.show_token_list();
    vector<UnitClass> token_list = lexer.get_token_list();

    Parser parser(&token_list);
    int parse_code = parser.lang();
    if (parse_code == 1) {
        return 1;
    }

    StackMachine stackMachine(&token_list);
    stackMachine.to_postfix();
    auto code = stackMachine.get_code();

    cout << "\nRPN\n\n";
    for (const auto& line : *code) {
        for (const auto& token : line) {
            cout << token.value << " ";
        }
        cout << "\n";
    }

    cout << "\nEXECUTION\n\n";

    Interpreter interpreter(code);
    interpreter.exec_code();
    interpreter.show_variables();

    /*ArrayByFaJey<string> array;

    array.add_tail("5");
    array.add_tail("6");
    array.add_tail("7");
    array.add_tail("8");
    array.add_tail("9");

    for (int i = 0; i < array.get_size(); i++) {
        cout << array[i] << endl;
    }
    cout << "\n";
    array.add_head("0");
    array.add_head("1");
    array.add_head("2");
    array.add_head("3");
    array.add_head("4");

    array.add_element("Pavel", 3);

    for (int i = 0; i < array.get_size(); i++) {
        cout << array[i] << endl;
    }*/

    return 0;
}
