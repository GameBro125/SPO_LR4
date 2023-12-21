#pragma once
#ifndef _syntax_
#define _syntax_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>

#include "boolevi.h"
#include "lecsic.h"

void shift_reduce(vector<Lexeme>& lexemes) {
	vector<string> SpisSyntax;
	vector<string> LecsicOtput;
	vector <int> SpisRules;
	for (auto lexeme : lexemes) LecsicOtput.push_back(lexeme.value);
	cout << "\n Lexical analyzer output text: \n";
	for (int v = 0; v < LecsicOtput.size(); v++) cout << " " << LecsicOtput[v];
	cout << endl;
	for (auto& lexeme : lexemes) if (lexeme.type == "Identifier" || lexeme.type == "Roman numeral") lexeme.value = "a";
	for (auto lexeme : lexemes) SpisSyntax.push_back(lexeme.value);
	cout << "\n Input text for the parser: \n";
	for (int z = 0; z < SpisSyntax.size(); z++) cout << " " << SpisSyntax[z];
	cout << endl;

	for (int j = 0; j < 20; j++) {
		for (int z = 1; z < SpisSyntax.size() - 1; z++) {
			if (SpisSyntax[z - 1] == "a" && SpisSyntax[z] == ":=" && SpisSyntax[z + 1] == "E" && SpisSyntax[z + 2] == ";") {
				SpisSyntax[z] = "S";
				SpisRules.push_back(1);
				cout << "\n Rule 1 \n" << " " << SpisSyntax[z];
			}
			else if (SpisSyntax[z] == "E" && SpisSyntax[z + 1] == "+" && SpisSyntax[z + 2] == "E" && (SpisSyntax[z + 3] != "/" && SpisSyntax[z + 3] != "*")) {
				SpisSyntax[z] = "E";
				SpisSyntax.erase(next(SpisSyntax.begin(), z + 1), next(SpisSyntax.begin(), z + 3));
				SpisRules.push_back(2);
				cout << "\n Rule 2 \n";
				for (int i = 0; i < SpisSyntax.size(); i++) cout << " " << SpisSyntax[i];
			}
			else if (SpisSyntax[z] == "E" && SpisSyntax[z + 1] == "*" && SpisSyntax[z + 2] == "E") {
				SpisSyntax[z] = "E";
				SpisSyntax.erase(next(SpisSyntax.begin(), z + 1), next(SpisSyntax.begin(), z + 3));
				SpisRules.push_back(4);
				cout << "\n Rule 4 \n";
				for (int i = 0; i < SpisSyntax.size(); i++) cout << " " << SpisSyntax[i];
			}
			else if (SpisSyntax[z] == "E" && SpisSyntax[z + 1] == "/" && SpisSyntax[z + 2] == "E") {
				SpisSyntax[z] = "E";
				SpisSyntax.erase(next(SpisSyntax.begin(), z + 1), next(SpisSyntax.begin(), z + 3));
				SpisRules.push_back(5);
				cout << "\n Rule 5 \n ";
				for (int i = 0; i < SpisSyntax.size(); i++) cout << " " << SpisSyntax[i];
			}
			else if (SpisSyntax[z] == "(" && SpisSyntax[z + 1] == "E" && SpisSyntax[z + 2] == ")") {
				SpisSyntax[z] = "E";
				SpisSyntax.erase(next(SpisSyntax.begin(), z + 1), next(SpisSyntax.begin(), z + 3));
				SpisRules.push_back(7);
				cout << "\n Rule 7 \n ";
				for (int i = 0; i < SpisSyntax.size(); i++) cout << " " << SpisSyntax[i];
			}
			else if (SpisSyntax[z] == "-" && SpisSyntax[z + 1] == "E") {
				SpisSyntax[z] = "E";
				SpisSyntax.erase(SpisSyntax.begin() + z + 1);
				SpisRules.push_back(8);
				cout << "\n Rule 8 \n ";
				for (int i = 0; i < SpisSyntax.size(); i++) cout << " " << SpisSyntax[i];
			}
			else if (SpisSyntax[z] == "a") {
				SpisSyntax[z] = "E";
				SpisRules.push_back(9);
				cout << "\n Rule 9 \n";
				for (int i = 0; i < SpisSyntax.size(); i++) cout << " " << SpisSyntax[i];
				break;
			}
		}
	}
	cout << " \n Sequence of rules usage: \n ";
	// вывод последовательности правил
	for (int z = 0; z < SpisRules.size(); z++) {
		cout << SpisRules[z];
		if (z < SpisRules.size() - 1 && SpisRules[z + 1] != 0) cout << " - ";
	}
	cout << endl;
	Sleep(2000);
}

#endif
