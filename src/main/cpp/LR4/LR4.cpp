#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>

#include "boolevi.h"
#include "lecsic.h"
#include "syntax.h"
#include "lr-4.h"

#define GREEN_TEXT "\033[1;32m"
#define RED_TEXT "\033[1;31m"
#define RESET_COLOR "\033[0m"

using namespace std;

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int line = 0, position = 0;
    string str = "";
    vector<Lexeme> SpisLex;
    vector<Err> SpisErr;
    vector<int> Flag(9, 0); /*0-bool prevDelimiter, 1-prevMark, 2-prevIdentifier, 3-prevLetter, 4-int prevBracket, 5-openBrackets, 6-Brackets, 7-commentBrackets, 8-commendEnd*/
    string file = "C:\\Users\\Wladislaw\\Desktop\\SPO_LR4\\src\\main\\resources\\input.txt";
    ifstream inputFile(file);
    stringstream buffer;
    buffer << inputFile.rdbuf();
    string code = buffer.str(); // Весь текст находится в code
    inputFile.close();

    for (int i = 0; i < code.size();) {
        line++;
        position = 0;
        str = "";
        str += code[i];
        for (int j = i; j + 1 < code.size() && code[j + 1] != '\n'; j++) {
            str += code[j + 1];
        } // считал строку
        if (Flag[7] == 1) continue;

        i += str.size();

        if (str[position] == '\n') {
            string str_2 = "";
            for (int gh = 1; gh < str.size(); gh++) str_2 += str[gh];
            str = str_2;
        }
        while (position < str.size()) {
            char currentChar = str[position];
            if (isComment(currentChar)) { Comment(SpisErr, Flag, code, str, line, position, i); continue; }  // комментарии 
            if (isBracket(currentChar)) { Bracket(SpisLex, SpisErr, Flag, str, line, position); continue; };
            if (isspace(currentChar)) { position++; continue; } // Проверка на пробел
            if (isDelimiter(currentChar)) { Delimiter(SpisLex, SpisErr, Flag, str, line, position); continue; }
            if (isMark(currentChar)) { Mark(SpisLex, SpisErr, Flag, str, line, position); continue; }
            if (isSpec(currentChar)) { Spec(SpisErr, str, line, position); continue; }
            if (isOperator(currentChar)) { Operator(SpisLex, SpisErr, Flag, str, line, position); continue; }
            if (isRim(string(1, currentChar))) { Rim(SpisLex, SpisErr, Flag, str, line, position); continue; }
            if (isLetter(currentChar) || isDigit(currentChar)) { Letter(SpisLex, SpisErr, Flag, str, line, position); continue; }
        }
    }


    // Проверка на ошибки
    if (!SpisErr.empty()) {
        cout << "\n" << setw(20) << "error type" << "\tLine \tPostiton\tValue" << endl;
        cout << endl;
        for (const auto& lexeme : SpisErr)  cout << setw(20) << lexeme.Errtype << "\t" << lexeme.Errline << "\t" << lexeme.Errposition << "\t" << lexeme.Errvalue << endl;
        cout << endl;
    }

    // Вывод таблицы с лексемами
 /*  if (!SpisLex.empty()) {
        cout << "\n" << setw(20) << "Lexeme type" << "\tLine\tPosition\tValue" << endl;
        cout << endl;
        for (const auto& lexeme : SpisLex)  cout << setw(20) << lexeme.type << "\t  " << lexeme.line << "\t  " << lexeme.position << "\t          " << lexeme.value << endl;
    }
 */  
    cout << endl;
    if (SpisErr.empty()) std::cout << GREEN_TEXT << "Lexical analyzer executed successfully." << RESET_COLOR << std::endl;


    //system(file.c_str());
    cout << endl;
    if (!SpisErr.empty()) cout << RED_TEXT <<  "Lexical analyzer has detected an error in the input text and the parser will not run. \n";
    // else shift_reduce(SpisLex);
    if (SpisErr.empty()) {
        cout << GREEN_TEXT << "The program has successfully completed syntax analysis.\n\n " << RESET_COLOR;
        Sleep(1400);
        lr4(SpisLex);
    }
    cout << endl;
    system("pause");

    return 0;
}
