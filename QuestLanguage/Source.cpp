#include "Lexer.h"

int main()
{
    /*Lexer lexer;
    string str;

    getline(cin , str);

    lexer.set_line(str);
    lexer.start_analysis();*/

    ArrayByFaJey<string> array;

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

    array.add_element("lox", 3);

    for (int i = 0; i < array.get_size(); i++) {
        cout << array[i] << endl;
    }

    return 0;
}
