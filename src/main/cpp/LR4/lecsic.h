#pragma once
#ifndef _ANALIZ_
#define _ANALIZ_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>

#include "boolevi.h"

void Delimiter(vector<Lexeme>& SpisLex, vector<Err>& SpisErr, vector<int>& Flag, const string str, int line, int& position)
{
    char currentChar = str[position];
    Lexeme lexeme;
    lexeme.type = "Delimiter";
    lexeme.value = (1, currentChar);
    lexeme.line = line;
    lexeme.position = position + 1;
    SpisLex.push_back(lexeme);
    position++;
    if (Flag[5] != 0) {
        Err lexeme;
        lexeme.Errtype = "Parenthesis";
        lexeme.Errvalue = ")";
        lexeme.Errline = line;
        lexeme.Errposition = position;
        SpisErr.push_back(lexeme);
    }
}

void Mark(vector<Lexeme>& SpisLex, vector<Err>& SpisErr, vector<int>& Flag, const string str, int line, int& position) {
    char currentChar = str[position];
    if (currentChar == ':') {
        if (position + 1 < str.size() && str[position + 1] == '=') {
            Lexeme lexeme;
            lexeme.type = "Assignment sign";
            lexeme.value = ":=";
            lexeme.line = line;
            lexeme.position = position + 1;
            SpisLex.push_back(lexeme);
            position = position + 2;
            Flag[0] = 0; // prevDelimiter
            Flag[1] = 0; // prevMark
        }
        else {
            Err lexeme;
            lexeme.Errtype = "Assignment sign";
            lexeme.Errvalue = ":";
            lexeme.Errline = line;
            lexeme.Errposition = position + 1;
            SpisErr.push_back(lexeme);
            position++;
        }
    }
    else if ((position - 1 > str.size() || str[position - 1] != ':') && currentChar == '=') {
        Err lexeme;
        lexeme.Errtype = "Assignment sign";
        lexeme.Errvalue = "=";
        lexeme.Errline = line;
        lexeme.Errposition = position + 1;
        SpisErr.push_back(lexeme);
        position++;
    }
}

void Spec(vector<Err>& SpisErr, const string str, int line, int& position) {
    Err lexeme;
    lexeme.Errtype = "Недопустимый символ";
    lexeme.Errvalue = (1, str[position]);
    lexeme.Errline = line;
    lexeme.Errposition = position + 1;
    SpisErr.push_back(lexeme);
}

void Operator(vector<Lexeme>& SpisLex, vector<Err>& SpisErr, vector<int>& Flag, const string str, int line, int& position) {
    char currentChar = str[position]; // доработать if 
    if (Flag[5] > 0) Flag[6] = 1;
    if ((position - 1 > 0 && position + 1 < str.size()) &&
        (isRim(string(1, str[position - 1])) || isLetter(str[position - 1]) || isDigit(str[position - 1]) || (str[position - 1] == ')')) &&
        (isRim(string(1, str[position + 1])) || isLetter(str[position + 1]) || isDigit(str[position + 1]) || (str[position + 1] == '('))) {
        Lexeme lexeme;
        lexeme.type = "Operator";
        lexeme.value = (1, str[position]);
        lexeme.line = line;
        lexeme.position = position + 1;
        SpisLex.push_back(lexeme);
    }
    else if (str[position] == '-' && (isRim(string(1, str[position + 1])) || isLetter(str[position + 1]) || isDigit(str[position + 1]))) {
        Lexeme lexeme;
        lexeme.type = "Operator";
        lexeme.value = (1, str[position]);
        lexeme.line = line;
        lexeme.position = position + 1;
        SpisLex.push_back(lexeme);
    }
    else {
        if (position - 1 <= 0 ||
            !isRim(string(1, str[position - 1])) && !isLetter(str[position - 1]) && (str[position - 1] != ')')) {
            Err lexeme;
            lexeme.Errtype = "Нет левой лексемы";
            lexeme.Errvalue = (1, str[position]);
            lexeme.Errline = line;
            lexeme.Errposition = position + 1;
            SpisErr.push_back(lexeme);
        }
        else if (position + 1 >= str.size() ||
            !isRim(string(1, str[position + 1])) && !isLetter(str[position + 1]) && (str[position + 1] != '(')) {
            Err lexeme;
            lexeme.Errtype = "Нет правой лексемы";
            lexeme.Errvalue = (1, str[position]);
            lexeme.Errline = line;
            lexeme.Errposition = position + 1;
            SpisErr.push_back(lexeme);
        }

    }
    position++;
}

void Rim(vector<Lexeme>& SpisLex, vector<Err>& SpisErr, vector<int>& Flag, const string str, int line, int& position) {
    string number(string(1, str[position]));
    while (position + 1 < str.size() && isRim(string(1, str[position + 1]))) {
        number += string(1, str[position + 1]);
        position++;
    }
    if (position + 1 < str.size() && (isLetter(str[position + 1]) || isSpec(str[position + 1]) || str[position + 1] == '(' || isDigit(str[position + 1]))) {
        string identifier = number;
        while (position + 1 < str.size() && (isLetter(str[position + 1]) || isDigit(str[position + 1]))) {
            identifier += str[position + 1];
            position++;
        }
        Err lexeme;
        lexeme.Errtype = "Roman numeral";
        lexeme.Errvalue = identifier;
        lexeme.Errline = line;
        lexeme.Errposition = position - identifier.size() + 2;
        SpisErr.push_back(lexeme);
    }
    else if (isRim(number)) {
        Lexeme lexeme;
        lexeme.type = "Roman numeral";
        lexeme.value = number;
        lexeme.line = line;
        lexeme.position = position - number.size() + 2;
        SpisLex.push_back(lexeme);
    }
    else cout << "тут что-то другое";
    position++;
}

void Letter(vector<Lexeme>& SpisLex, vector<Err>& SpisErr, vector<int>& Flag, const string str, int line, int& position) {
    if (Flag[5] > 0) Flag[6] = 1;
    string identifier(1, str[position]);
    while (position + 1 < str.size() && (isLetter(str[position + 1]) || isDigit(str[position + 1]))) {
        identifier += str[position + 1];
        position++;
    }
    if (isRim(string(1, identifier[0]))) {
        Err lexeme;
        lexeme.Errtype = "Roman numeral";
        lexeme.Errvalue = identifier;
        lexeme.Errline = line;
        lexeme.Errposition = position - identifier.size() + 2;
        SpisErr.push_back(lexeme);
    }
    else {
        Lexeme lexeme;
        lexeme.type = "Identifier";
        lexeme.value = identifier;
        lexeme.line = line;
        lexeme.position = position - identifier.size() + 2;
        SpisLex.push_back(lexeme);
    }
    position++;
}

void Bracket(vector<Lexeme>& SpisLex, vector<Err>& SpisErr, vector<int>& Flag, const string str, int line, int& position) {
    char currentChar = str[position];
    Lexeme lexeme;
    if (currentChar == '(') {
        lexeme.type = "Parenthesis";
        Flag[5]++;
        Flag[6] = 0;
    }
    if (currentChar == ')') {
        lexeme.type = "Parenthesis";
        Flag[5]--;
        if (Flag[5] < 0) {
            Err lexeme;
            lexeme.Errtype = "Unexpected Parenthesis";
            lexeme.Errvalue = (1, str[position]);
            lexeme.Errline = line;
            lexeme.Errposition = position + 1;
            SpisErr.push_back(lexeme);
        }
        if (Flag[5] > 0 && Flag[6] != 1) {
            Err lexeme;
            lexeme.Errtype = "Отсутствие Identifierа в скобках";
            lexeme.Errvalue = (1, str[position]);
            lexeme.Errline = line;
            lexeme.Errposition = position + 1;
            SpisErr.push_back(lexeme);
        }
    }
    lexeme.line = line;
    lexeme.position = position + 1;
    lexeme.value = currentChar;
    SpisLex.push_back(lexeme);
    position++;

}

void Comment(vector<Err>& SpisErr, vector<int>& Flag, const string code, const string str, int& line, int& position, int& intg) {
    string str_4 = string(1, str[position]), str_2 = "";
    int sd = position;
    bool as;

    if (str_4 == "{") Flag[7] = 1; else  Flag[7] = 0;
    //position++;

    for (int a = 0; a < str.size(); a++) {
        if (str[a] == '{') as = true;
        else if (str[a] == '}') as = false;
    }

    if (!as) {
        for (int j = position + 1; j <= str.size(); j++) {
            if (str[j] == '}' && Flag[7] == 0) {
                Err lexeme;
                lexeme.Errtype = "Комментарий";
                lexeme.Errvalue = "}";
                lexeme.Errline = line;
                lexeme.Errposition = position + 1;
                SpisErr.push_back(lexeme);
                break;
            }
            else if (str[j] == '}' && Flag[7] == 1) { Flag[7] = 0; Flag[8] = 1; position += j - sd; break; }
            else if (Flag[7] == 1 && str[j] != '\n') str_2 += str[j];
            else if (j - 1 >= 0 && str[j - 1] == '}' && Flag[7] == 0) {
                Err lexeme;
                lexeme.Errtype = "Комментарий";
                lexeme.Errvalue = "}";
                lexeme.Errline = line;
                lexeme.Errposition = position + 1;
                SpisErr.push_back(lexeme);
                break;
            }
        }
        if (Flag[7] == 1) {
            Err lexeme;
            lexeme.Errtype = "Комментарий";
            lexeme.Errvalue = "{";
            lexeme.Errline = line;
            lexeme.Errposition = position + 2;
            SpisErr.push_back(lexeme);
        } //конец комментариев
    }
    else {
        int ty = intg - str.size();
        sd = ty;
        for (int j = ty + 1; j < code.size(); j++) {
            if (code[j] == '}' && Flag[7] == 0) {
                Err lexeme;
                lexeme.Errtype = "Комментарий";
                lexeme.Errvalue = "}";
                lexeme.Errline = line;
                lexeme.Errposition = position + 1;
                SpisErr.push_back(lexeme);
                break;
            }
            else if (code[j] == '}' && Flag[7] == 1) {
                Flag[7] = 0;
                //position++; 
                intg += str_2.size() - 1;
                break;
            }
            else if (Flag[7] == 1 && code[j] == '\n') {
                line++;
                position = 0;
                str_2 += code[j];
            }
            else if (Flag[7] == 1 && code[j] != '\n') { str_2 += code[j]; position++; }
        }
        if (Flag[7] == 1) {
            Err lexeme;
            lexeme.Errtype = "Комментарий";
            lexeme.Errvalue = "{";
            lexeme.Errline = line;
            lexeme.Errposition = position + 2;
            SpisErr.push_back(lexeme);
        } //конец комментариев
    }
    position++;
    //cout << str_2 << endl;
}

#endif
