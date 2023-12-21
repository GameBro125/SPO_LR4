    #pragma once
#ifndef _BOOLEVI_
#define _BOOLEVI_

#include <string>
using namespace std;

struct Lexeme {
    string type; // тип лексемы
    string value; // сама лексема
    int line; //  строка
    int position; // позиция
};
struct Err {
    string Errtype;
    int Errline;
    int Errposition;
    string Errvalue;
};

// Функция для проверки, является ли символ круглой скобкой
bool isBracket(char ch) {
    string delimiters = "()";
    return delimiters.find(ch) != string::npos;
}

// Функция для проверки, является ли символ разделителем
bool isDelimiter(char ch) {
    string delimiters = ";";
    return delimiters.find(ch) != string::npos;
}

// Функция для проверки, является ли символ знаком присваивания
bool isMark(char ch) {
    string operators = ":=";
    return operators.find(ch) != string::npos;
}

// Функция для проверки, является ли строка оператором
bool isOperator(char ch) {
    string operators = "+-*/";
    return operators.find(ch) != string::npos;
}

bool isRim(const string& str) { // Затычка
    vector<string> operators = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X",
                                "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII", "XIX", "XX",
                                "XXI", "XXII", "XXIII", "XXIV", "XXV", "XXVI", "XXVII", "XXVIII", "XXIX", "XXX",
                                "XXXI", "XXXII", "XXXIII", "XXXIV", "XXXV", "XXXVI", "XXXVII", "XXXVIII", "XXXIX", "XL",
                                "XLI", "XLII", "XLIII", "XLIV", "XLV", "XLVI", "XLVII", "XLVIII", "XLIX", "L" };
    return find(operators.begin(), operators.end(), str) != operators.end();
}

// Функция для проверки, является ли символ Identifierом (переменной)
bool isLetter(char ch) {
    return isalpha(ch);
}

// Функция для проверки, является ли символ цифрой
bool isDigit(char ch) {
    string operators = "1234567890";
    return operators.find(ch) != string::npos;
}

bool isSpec(char ch) {
    string specialSymbols = "@#$&_[],<>?~";
    return specialSymbols.find(ch) != string::npos;
}

bool isComment(char ch) {
    string specialSymbols = "{}";
    return specialSymbols.find(ch) != string::npos;
}

#endif 
