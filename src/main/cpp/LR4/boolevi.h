    #pragma once
#ifndef _BOOLEVI_
#define _BOOLEVI_

#include <string>
using namespace std;

struct Lexeme {
    string type; // ��� �������
    string value; // ���� �������
    int line; //  ������
    int position; // �������
};
struct Err {
    string Errtype;
    int Errline;
    int Errposition;
    string Errvalue;
};

// ������� ��� ��������, �������� �� ������ ������� �������
bool isBracket(char ch) {
    string delimiters = "()";
    return delimiters.find(ch) != string::npos;
}

// ������� ��� ��������, �������� �� ������ ������������
bool isDelimiter(char ch) {
    string delimiters = ";";
    return delimiters.find(ch) != string::npos;
}

// ������� ��� ��������, �������� �� ������ ������ ������������
bool isMark(char ch) {
    string operators = ":=";
    return operators.find(ch) != string::npos;
}

// ������� ��� ��������, �������� �� ������ ����������
bool isOperator(char ch) {
    string operators = "+-*/";
    return operators.find(ch) != string::npos;
}

bool isRim(const string& str) { // �������
    vector<string> operators = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "X",
                                "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII", "XIX", "XX",
                                "XXI", "XXII", "XXIII", "XXIV", "XXV", "XXVI", "XXVII", "XXVIII", "XXIX", "XXX",
                                "XXXI", "XXXII", "XXXIII", "XXXIV", "XXXV", "XXXVI", "XXXVII", "XXXVIII", "XXXIX", "XL",
                                "XLI", "XLII", "XLIII", "XLIV", "XLV", "XLVI", "XLVII", "XLVIII", "XLIX", "L" };
    return find(operators.begin(), operators.end(), str) != operators.end();
}

// ������� ��� ��������, �������� �� ������ Identifier�� (����������)
bool isLetter(char ch) {
    return isalpha(ch);
}

// ������� ��� ��������, �������� �� ������ ������
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
