#include "Plane.h"
#include <fstream>
#include <iostream>
#include <cassert>
#include <string>

using namespace std;

void LinProg::Plane::init()
{
	Columns = Rows + InputVarNum + 1;
	Variables.resize(InputVarNum);
	SimplexTable.resize(Rows);
	for (int i = 0; i < Rows; ++i)
		SimplexTable[i].resize(Columns);
	BasisVars.resize(Rows);
	StartTargetFunc = 0;
	IntVarNum = InputVarNum;
}

void LinProg::Plane::addTargetFunc(string s)

{
	bool numOccured = false, operTypeIsPresent = false, signOccured = false;

	string thVar;

	int i = 0, j = 0;

	int freeMember = 0;

	int factor = -1;

	while (i < s.length())
	{
		if (s[i] == ' ' || s[i] == '*')
		{
		}
		else if (signOccured == true)
		{
			if (i + 2 < s.length() && (s[i] == 'm' || s[i] == 'M'))
			{
				if ((s[i + 1] == 'a' || s[i + 1] == 'A') && (s[i + 2] == 'x' || s[i + 2] == 'X'))
					operation = maximiz;
				else if ((s[i + 1] == 'i' || s[i + 1] == 'I') && (s[i + 2] == 'n' || s[i + 2] == 'N'))
					operation = minimiz;
				else {
					//Invalid Syntax
					cout << "INVALID SYNTAX" << endl;
					return;
				}
				operTypeIsPresent = true;
				break;
			}
			else
			{
				cout << "INVALID SYNTAX" << endl;
				return;
			}
		}
		else if (isdigit(s[i]))
		{
			while (j < s.length() && isdigit(s[j]))
				++j;
			factor *= atoi(string(s.begin() + i, s.begin() + j).c_str());
			numOccured = true;
		}
		else if (isalpha(s[i]))
		{
			while (j < s.length() && (isalpha(s[j]) || isdigit(s[j])))
				++j;
			thVar = string(s.begin() + i, s.begin() + j);
			bool VarExists = false;
			for (int k = 0; k < Variables.size(); ++k)
				if (Variables[k].name == thVar || Variables[k].name == "")
				{
					Variables[k].name = thVar;
					VarExists = true;
					SimplexTable[0][k] = Rnum(factor);
					break;
				}

			if (!VarExists)
			{
				cout << "Too many variables" << endl;
				return;
			}
			factor = -1;
			numOccured = false;
		}
		else
		{
			if (numOccured)
			{
				freeMember -= factor;
				numOccured = false;
			}

			if (s[i] == '-' && i + 1 < s.length() && s[i + 1] == '>')
			{
				++i;
				signOccured = true;
			}
			else if (s[i] == '-' || s[i] == '+')
			{
				if (s[i] == '-')
					factor = -factor;
			}
			else {
				cout << "Invalid syntax." << endl;
				return;
			} // error
		}
		if (j > i)
			i = j;
		else j = ++i;
	}

	if (!operTypeIsPresent)
	{
		cout << "InvalidSyntax" << endl;
		return;
	}


	if (numOccured)
	{
		freeMember -= factor;
		numOccured = false;
	}


	SimplexTable[0][Columns - Rows - 1] = Rnum(1);
	SimplexTable[0][Columns - 1] = freeMember;
	BasisVars[0] = InputVarNum;
}

void LinProg::Plane::addCondition(string s, int index)
{
	bool signOccured = false, digitOccured = false;
	string thVar;
	int i = 0, j = 0;

	double freeMember = 0;

	int factor = 1;

	while (i < s.length())
	{
		if (s[i] == ' ' || s[i] == '*')
		{}
		else if (isdigit(s[i]))
		{
			while (j < s.length() && isdigit(s[j]))
				++j;
			if (signOccured)
				factor *= -1;
			factor *= atoi(string(s.begin() + i, s.begin() + j).c_str());
			digitOccured = true;
		}
		else if (isalpha(s[i]))
		{
			while (j < s.length() && (isalpha(s[j]) || isdigit(s[j])))
				++j;

			thVar = string(s.begin() + i, s.begin() + j);
			bool VarExists = false;

			for (int k = 0; k < Variables.size(); ++k)
				if ((Variables[k].name == thVar) || (Variables[k].name == ""))
				{
					//InputVarNum++;
					Variables[k].name = thVar;
					VarExists = true;
					SimplexTable[index][k] = Rnum(factor);
					break;
				}

			if (!VarExists)
			{
				cout << "Too many variables" << endl;
				return;
			}
			factor = 1;
			digitOccured = false;
		}
		else
		{
			if (signOccured == true)
			{
				cout << "Expression can have only one sign (>, <, >=, <=, =)" << endl;
				return;
			}

			if (digitOccured)
			{
				if (signOccured)
					freeMember -= factor;
				else
					freeMember += factor;
				digitOccured = false;
			}
			if (s[i] == '-' || s[i] == '+')
			{
				if (s[i] == '-')
					factor = -factor;
			}
			else
				if (i + 1 < s.length() && ((s[i] == '>') || (s[i] == '<')) && (s[i + 1] == '='))
				{
					SimplexTable[index][InputVarNum + index] = Rnum(1);
					BasisVars[index] = InputVarNum + index;
					if (s[i] == '>')
						SimplexTable[index] = SimplexTable[index] * Rnum(-1);
					signOccured = true;
					j = ++i;
				}
				else if (s[i] == '=')
				{
					SimplexTable[index][InputVarNum + index] = Rnum(0);
					BasisVars[index] = index;
					signOccured = true;
				}
				else {
					cout << "Unknown symbol." << endl;
					return;
				}
		}
		if (j > i)
			i = j;
		else j = ++i;
	}


	if (digitOccured)
	{
		if (signOccured)
			freeMember -= factor;
		else
			freeMember += factor;
		digitOccured = false;
	}

	SimplexTable[index][Columns - 1] = freeMember;
}

void LinProg::Plane::addIntVars(string s)
{
	if (s == "")
	{
		for (int i = 0; i < Variables.size(); ++i)
			Variables[i].MustBeInteger = true;
		return;
	}
	IntVarNum = 0;
	int i = 0;
	while (i < s.length())
	{
		while (i < s.length() && !isalpha(s[i]))
			++i;
		int j = i;
		while (j < s.length() && s[j] != ' ')
			++j;
		string varName = string(s.begin() + i, s.begin() + j);

		for (int i = 0; i < Variables.size(); ++i)
			if (Variables[i].name == varName)
			{
				Variables[i].MustBeInteger = true;
				++IntVarNum;
			}
		++i;
	}
}

void LinProg::Plane::consoleInput()
{
	cout << "Enter num of variables: ";
	cin >> InputVarNum;
	cout << "Enter num of equations (including target function): ";
	cin >> Rows;
	assert(InputVarNum >= 1 && Rows >= 1);
	init();

	string s;
	cout << "Enter target function : " << endl;
	do {
		cin.ignore(cin.rdbuf()->in_avail());
		getline(cin, s);
	} while (s == "");
	addTargetFunc(s);

	for (int i = 1; i < Rows; ++i)
	{
		do {
			cin.ignore(cin.rdbuf()->in_avail());
			getline(cin, s);
		} while (s == "");

		addCondition(s, i);
	}
	cout << "Variables that must be integer (all by default. enter empty string to finish input) : " << endl;
	cin.ignore(cin.rdbuf()->in_avail());
	getline(cin, s);
	addIntVars(s);
}

void LinProg::Plane::readFile(string filename)
{
	ifstream File(filename);
	assert(File.is_open());
	File >> InputVarNum;
	File >> Rows;
	assert(InputVarNum > 1 && Rows > 1);
	init();

	string s;
	do {
		getline(File, s);
	} while (s == "");
	addTargetFunc(s);

	for (int i = 1; i < Rows; ++i)
	{
		assert(!File.eof());
		do {
			getline(File, s);
		} while (s == "");
		addCondition(s, i);
	}
	s = "";
	while (!File.eof() && s == "")
		getline(File, s);
	addIntVars(s);
	cout << endl;
}

LinProg::Plane::Plane(string input, bool isfilename)
{
}

string LinProg::getCD()
{
	wchar_t *w = nullptr;
	wstring ws = _wgetcwd(w, _MAX_PATH);
	return string(ws.begin(), ws.end());
}