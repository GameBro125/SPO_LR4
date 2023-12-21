#pragma once
#ifndef _qwerty_
#define _qwerty_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>

#include "syntax.h"

using namespace std;

void FunRimArab(vector <string>& arr, vector <double>& arr2) {
	for (int i = 0; i < arr.size(); i++)
		if (arr[i] == "I") arr2[i] = stod("1");
		else if (arr[i] == "II") arr2[i] = stod("2");
		else if (arr[i] == "III") arr2[i] = stod("3");
		else if (arr[i] == "IV") arr2[i] = stod("4");
		else if (arr[i] == "V") arr2[i] = stod("5");
		else if (arr[i] == "VI") arr2[i] = stod("6");
		else if (arr[i] == "VII") arr2[i] = stod("7");
		else if (arr[i] == "VIII") arr2[i] = stod("8");
		else if (arr[i] == "IX") arr2[i] = stod("9");
		else if (arr[i] == "X") arr2[i] = stod("10");
}

void lr4(vector<Lexeme> lexemes)
{
	int num = 0;
	int k = 0;
	int j = 0, z = 0, fl = 0;
	vector <string> SpisLex4;
	string t[50][50]; // что это
	for (auto lexeme : lexemes) SpisLex4.push_back(lexeme.value);

	cout << "\nFragment:" << endl;
	for (int i = 0; i < SpisLex4.size(); i++)
	{
		cout << SpisLex4[i];
		if (SpisLex4[i] == ";") cout << endl;
	}

	int c = 0;
	double b = 0;
	vector <string> Idef(50, " "), pa2 = SpisLex4;
	vector <double> RimArab;
	vector <double> q(50);

	cout << "\nConvolution:" << endl;

	int zvezda2 = SpisLex4.size();

	for (int i = 0; i < zvezda2; i++)
	{
		for (int j = 0; j < zvezda2; j++)
		{
			if (j + 2 < SpisLex4.size() && SpisLex4[j] == "(" && SpisLex4[j + 2] == ")")
			{
				t[c][0] = SpisLex4[j];
				t[c][1] = SpisLex4[j + 1];
				t[c][2] = "0"; //ssilka
				for (int y = 0; y < c; y++)
				{
					if (t[c][1] == Idef[y])
					{
						b = stod(t[y][3]);
					}
				}
				q[j] = b;
				c++;
				SpisLex4[j] = to_string(c);
				SpisLex4.erase(SpisLex4.begin() + j + 2);
				SpisLex4.erase(SpisLex4.begin() + j + 1);

				cout << setw(3) << left << c << setw(3) << "(" << "(" << t[c - 1][1] << "," << t[c - 1][2] << ")" << endl;
				break;
			}
			if (j + 1 < SpisLex4.size() && (SpisLex4[j] == "+" || SpisLex4[j] == "*" || SpisLex4[j] == "/" || SpisLex4[j] == "-" && SpisLex4[j + 1] != "("))
			{
				if (j - 1 < SpisLex4.size() && (SpisLex4[j + 2] == "*" || SpisLex4[j + 2] == "/" || (SpisLex4[j + 2] == ")" && SpisLex4[j - 1] == "(")))
				{

				}
				else
				{
					string sohr = SpisLex4[j];
					t[c][0] = SpisLex4[j];
					t[c][1] = SpisLex4[j - 1];
					t[c][2] = SpisLex4[j + 1];

					FunRimArab(SpisLex4, q);

					for (int y = 0; y < c; y++)
					{
						if (t[c][1] == Idef[y])
						{
							q[j - 1] = stod(t[y][3]);
						}
						if (t[c][2] == Idef[y])
						{
							q[j + 1] = stod(t[y][3]);
						}
					}

					if (SpisLex4[j] == "+")
					{
						b = q[j - 1] + q[j + 1];
						q[j - 1] = b;
					}
					if (SpisLex4[j] == "*")
					{
						b = q[j - 1] * q[j + 1];
						q[j - 1] = b;
					}
					if (SpisLex4[j] == "/")
					{
						b = q[j - 1] / q[j + 1];
						q[j - 1] = b;
					}
					if (SpisLex4[j] == "-")
					{
						b = q[j - 1] - q[j + 1];
						q[j - 1] = b;
					}

					t[c][3] = to_string(b);


					c++;
					SpisLex4[j] = to_string(c);
					SpisLex4.erase(SpisLex4.begin() + j + 1);
					SpisLex4.erase(SpisLex4.begin() + j - 1);

					cout << setw(3) << left << c << setw(3) << sohr << "(" << t[c - 1][1] << "," << t[c - 1][2] << ")" << endl;
					break;
				}
			}
			if (j + 2 < SpisLex4.size() && (SpisLex4[j] == "-" && (SpisLex4[j + 2] == ")" || SpisLex4[j + 2] == ";")))
			{
				t[c][0] = SpisLex4[j];
				t[c][1] = SpisLex4[j + 1];
				t[c][2] = "0"; //ssilka



				b = -1 * q[j + 1];
				for (int y = 0; y < c; y++)
				{
					if (t[c][1] == Idef[y])
					{
						b = -stod(t[y][3]);
					}
				}
				q[j] = b;
				t[c][3] = to_string(b);
				c++;
				SpisLex4[j] = to_string(c);
				SpisLex4.erase(SpisLex4.begin() + j + 1);

				cout << setw(3) << left << c << setw(3) << "-" << "(" << t[c - 1][1] << "," << t[c - 1][2] << ")" << endl;
				break;
			}
			if (j + 3 < SpisLex4.size() && (SpisLex4[j] == "-" && SpisLex4[j + 3] == ")"))
			{
				t[c][0] = SpisLex4[j];
				t[c][1] = SpisLex4[j + 2];
				t[c][2] = "0";
				FunRimArab(SpisLex4, q);
				b = -1 * q[j + 2];

				for (int y = 0; y < c; y++)
				{
					if (t[c][1] == Idef[y])
					{
						b = -stod(t[y][3]);
					}
				}
				q[j] = b;
				t[c][3] = to_string(b);
				c++;
				SpisLex4[j + 1] = to_string(c);
				SpisLex4.erase(SpisLex4.begin() + j + 3);
				SpisLex4.erase(SpisLex4.begin() + j + 2);
				SpisLex4.erase(SpisLex4.begin() + j);

				cout << setw(3) << left << c << setw(3) << "-" << "(" << t[c - 1][1] << "," << t[c - 1][2] << ")" << endl;
				break;
			}
			if (j + 2 < SpisLex4.size() && (SpisLex4[j] == ":=" && SpisLex4[j + 2] == ";"))
			{
				t[c][0] = SpisLex4[j];
				t[c][1] = SpisLex4[j - 1];
				t[c][2] = SpisLex4[j + 1];

				if (b > 0 || b < 0)
				{
					Idef[c] = SpisLex4[j - 1];
					RimArab.push_back(b);
				}
				else
				{
					FunRimArab(SpisLex4, q);
					t[c][3] = to_string(q[j + 1]);
					Idef[c] = SpisLex4[j - 1];
					RimArab.push_back(stod(t[c][3]));
				}
				c++;
				q[j] = b;
				SpisLex4[j - 1] = to_string(c);
				SpisLex4.erase(SpisLex4.begin() + j + 2);
				SpisLex4.erase(SpisLex4.begin() + j + 1);
				SpisLex4.erase(SpisLex4.begin() + j);

				cout << setw(3) << left << c << setw(3) << ":=" << "(" << t[c - 1][1] << "," << t[c - 1][2] << ")" << endl;
				break;
			}
		}
	}
	int ll = 0;
	cout << "\nTable Т:\n";
	for (int w = 0; w < 50; w++)
	{

		if (Idef[w] != " ")
		{
			cout << "(";

			cout << Idef[w];

			cout << ", ";
			cout << fixed << setprecision(2) << RimArab[ll++];

			cout << ")";
			cout << endl;
		}

	}

	int end = pa2.size();
	cout << "\nDiversion redudants: " << endl;
	c = 0;
	b = 1;
	for (int i = 0; i < end; i++)
	{
		for (int j = 0; j < end; j++)
		{

			if (j + 1 < pa2.size() && (pa2[j] == "+" || pa2[j] == "*" || pa2[j] == "/" || pa2[j] == "-") && pa2[j + 1] != "(") {
				if (j + 2 < pa2.size() && j - 1 >= 0 && (pa2[j + 2] == "*" || pa2[j + 2] == "/" || (pa2[j + 2] == ")" && pa2[j - 1] == "(")))
				{

				}
				else
				{
					string sohr = pa2[j];
					t[c][0] = pa2[j];
					t[c][1] = pa2[j - 1];
					t[c][2] = pa2[j + 1]; //ssilka

					bool fl1 = false;
					for (int w = 0; w < c - 1; w++)
					{
						if ((t[c][0] == t[w][0]) && (t[c][1] == t[w][1]) && (t[c][2] == t[w][2]))
						{
							t[c][0] = " ";
							t[c][1] = " ";
							t[c][2] = " ";
							c--;

							if (fl1 == true) pa2[j - 1] = to_string(w + 1);
							else pa2[j] = to_string(w + 1);

							if (fl1 == false) {
								pa2.erase(pa2.begin() + j + 1);
								pa2.erase(pa2.begin() + j - 1);
							}
							b = 0;
							break;
						}
						else
						{
							if (fl1 == true) pa2[j - 1] = to_string(c + 1);
							else pa2[j] = to_string(c + 1);
							//pa2[j + 1] = ".";
							//pa2[j - 1] = ".";
							if (fl1 == false) {
								pa2.erase(pa2.begin() + j + 1);
								pa2.erase(pa2.begin() + j - 1);
								fl1 = true;
							}
						}
					}

					c++;

					if (b == 1)
						cout << setw(3) << left << c << setw(3) << sohr << "(" << t[c - 1][1] << "," << t[c - 1][2] << ")" << endl;
					b = 1;
					break;
				}
			}
			if (j + 2 < pa2.size() && (pa2[j] == "-" && (pa2[j + 2] == ")" || pa2[j + 2] == ";")))
			{
				t[c][0] = pa2[j];
				t[c][1] = pa2[j + 1];
				t[c][2] = "0"; //ssilka

				for (int w = 0; w < c - 1; w++)
				{
					if ((t[c][0] == t[w][0]) && (t[c][1] == t[w][1]) && (t[c][2] == t[w][2]))
					{
						t[c][0] = " ";
						t[c][1] = " ";
						t[c][2] = " ";
						c--;
						pa2[j] = to_string(w + 1);
						pa2.erase(pa2.begin() + j + 1);
						pa2.erase(pa2.begin() + j - 1);
						b = 0;
						break;
					}
					else
					{
						pa2[j] = to_string(c + 1);
						pa2.erase(pa2.begin() + j + 2);
						pa2.erase(pa2.begin() + j + 1);
					}
				}

				c++;
				if (b == 1)
					cout << setw(3) << left << c << setw(3) << "-" << "(" << t[c - 1][1] << "," << t[c - 1][2] << ")" << endl;
				b = 1;
				break;
			}
			if (j + 3 < pa2.size() && (pa2[j] == "-" && pa2[j + 3] == ")"))
			{
				t[c][0] = pa2[j];
				t[c][1] = pa2[j + 2];
				t[c][2] = "0";
				for (int w = 0; w < c - 1; w++)
				{
					if ((t[c][0] == t[w][0]) && (t[c][1] == t[w][1]) && (t[c][2] == t[w][2]))
					{
						t[c][0] = " ";
						t[c][1] = " ";
						t[c][2] = " ";
						c--;
						pa2[j] = to_string(w + 1);
						pa2.erase(pa2.begin() + j + 1);
						pa2.erase(pa2.begin() + j - 1);
						b = 0;
						break;
					}
					else
					{
						pa2[j + 1] = to_string(c + 1);
						pa2.erase(pa2.begin() + j + 3);
						pa2.erase(pa2.begin() + j + 2);
						pa2.erase(pa2.begin() + j);
					}
				}
				c++;

				if (b == 1)
					cout << setw(3) << left << c << setw(3) << "-" << "(" << t[c - 1][1] << "," << t[c - 1][2] << ")" << endl;
				b = 1;
				break;
			}
			if (j + 2 < pa2.size() && pa2[j] == "(" && pa2[j + 2] == ")")
			{
				t[c][0] = pa2[j];
				t[c][1] = pa2[j + 1];
				t[c][2] = "0";

				bool fl1 = false;
				for (int w = 0; w < c - 1; w++)
				{
					if ((t[c][0] == t[w][0]) && (t[c][1] == t[w][1]) && (t[c][2] == t[w][2]))
					{
						t[c][0] = " ";
						t[c][1] = " ";
						t[c][2] = " ";
						c--;
						pa2[j] = to_string(w + 1);
						pa2.erase(pa2.begin() + j + 1);
						pa2.erase(pa2.begin() + j - 1);
						b = 0;
						break;
					}
					else
					{
						pa2[j] = to_string(c + 1);
						/*pa2[j + 1] = ".";
						pa2[j + 2] = ".";*/
						//pa2.erase(pa2.begin() + j + 2);
						//pa2.erase(pa2.begin() + j + 1);
						if (fl1 == false) {
							pa2.erase(pa2.begin() + j + 2);
							pa2.erase(pa2.begin() + j + 1);
							fl1 = true;
						}
					}
				}
				c++;

				if (b == 1)
					cout << setw(3) << left << c << setw(3) << "(" << "(" << t[c - 1][1] << "," << t[c - 1][2] << ")" << endl;
				b = 1;
				break;
			}
			if (j + 2 < pa2.size() && pa2[j] == ":=" && pa2[j + 2] == ";")
			{
				t[c][0] = pa2[j];
				t[c][1] = pa2[j - 1];
				t[c][2] = pa2[j + 1]; //ssilka

				for (int w = 0; w < c - 1; w++)
				{
					if ((t[c][0] == t[w][0]) && (t[c][1] == t[w][1]) && (t[c][2] == t[w][2]))
					{
						t[c][0] = " ";
						t[c][1] = " ";
						t[c][2] = " ";
						c--;
						b = 0;
						break;
					}

				}

				c++;
				pa2[j - 1] = to_string(c);
				//pa2[j] = ".";
				//pa2[j + 1] = ".";
				//pa2[j + 2] = ".";
				pa2.erase(pa2.begin() + j + 2);
				pa2.erase(pa2.begin() + j + 1);
				pa2.erase(pa2.begin() + j);
				if (t[c - 1][1] == t[c][1]) break;
				if (b == 1) cout << setw(3) << left << c << setw(3) << ":=" << "(" << t[c - 1][1] << "," << t[c - 1][2] << ")" << endl;
				b = 1;
				break;
			}
		}
	}
	cout << endl;
	system("pause");
}
#endif
