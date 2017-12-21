#include "Gomory.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cassert>

using namespace std;



LinProg::Gomory::~Gomory()
{
	if (P)
		delete P;
}

void LinProg::Gomory::readFile(string filename)
{
	P->readFile(filename);
}

void LinProg::Gomory::consoleInput()
{
	P->consoleInput();
}

void LinProg::Gomory::run()
{
	if (P->IntVarNum == P->InputVarNum)
	{
		cout << "GOMORY 1 IS RUNNING : " << endl;
		run1();
	}
	else
	{
		cout << "GOMORY 2 IS RUNNING : " << endl;
		run2();
	}
}

void LinProg::Gomory::run1()
{
	S = Simplex(P);
	S.run();
	P->StepsCount = 0;

	while (!isProperSolution())
	{
		++P->StepsCount;
		findMaxFracPart();
		addRestriction(1);
		addNewBaseVar();
		S.displayResult();
		findPivot();
		S.applyChangesToSystem();
	}

	cout << "HOMORI1 ---END--- OPTIMAL INTEGER SOLUTION FOUND" << endl;
	S.displayResult();
}

void LinProg::Gomory::run2()
{

	S = Simplex(P);
	S.run();

	P->StepsCount = 0;

	while (!isProperSolution())
	{
		++P->StepsCount;
		findMaxFracPart();
		addRestriction(0);
		addNewBaseVar();
		S.displayResult();
		findPivot();
		S.applyChangesToSystem();
	}

	cout << "HOMORI2 ---END--- OPTIMAL INTEGER SOLUTION FOUND" << endl;
	S.displayResult();
}


void LinProg::Gomory::addRestriction(bool isFirstAlgo)
{
	S.insertRow(0);
	if (isFirstAlgo == 1)
	{
		for (int i = 0; i < P->Columns; ++i)
			P->SimplexTable[0][i] = P->SimplexTable[P->pivotRow][i].getFracPart() * Rnum(-1);
	}
	else {
		for (int i = 0; i < P->Columns; ++i)
			P->SimplexTable[0][i] = secondAlgoCountCoeff(i);
	}
	S.P->StartTargetFunc++;
}

void LinProg::Gomory::addNewBaseVar()
{
	S.insertColumn();
	++P->Columns;
	P->SimplexTable[0][P->Columns - 2] = Rnum(1);
	P->BasisVars.insert(P->BasisVars.begin(), P->Columns - 1);
}



LinProg::Rnum LinProg::Gomory::secondAlgoCountCoeff(int i)
{
	Rnum tmp = P->SimplexTable[P->pivotRow][i];
	if (i == P->Columns - 1)
		return P->SimplexTable[P->pivotRow][P->Columns - 1];
	if (i < P->Variables.size() && !P->Variables[i].MustBeInteger)
	{
		if (tmp >= 0)
			return tmp;
		Rnum tmpFP = tmp.getFracPart();
		return tmpFP / (1 - tmpFP) * (-1) * tmp;
	}
	else if (P->SimplexTable[P->pivotRow][P->Columns - 1] > tmp)
	{
		Rnum tmpFP = tmp.getFracPart();
		return tmpFP / (1 - tmpFP) * (1 - tmpFP);
	}
	else return tmp.getFracPart();
}

void LinProg::Gomory::findPivot()
{
	S.P->pivotRow = 0;
	int tmp = S.P->StartTargetFunc;
	S.P->StartTargetFunc = S.P->StepsCount - 1;
	S.findPivotColumn();
	S.P->StartTargetFunc = tmp;
}

//BY MAX FRACTIONAL PART
void LinProg::Gomory::findMaxFracPart()
{
	P->pivotColumn = P->Columns - 1;

	Rnum M, Cur;

	int i = 0;
	P->pivotRow = 1;

	while (P->BasisVars[i] >= P->InputVarNum)
		++i;
	M = P->SimplexTable[i][P->Columns-1].getFracPart();

	while (i < P->Rows)
	{
//		while (i < P->Rows && P->BasisVars[i] >= P->InputVarNum)
//			++i;
//		if (i >= P->Rows)
//			break;
		Cur = P->SimplexTable[i][P->Columns - 1].getFracPart();
		if (Cur > M)
		{
			M = Cur;
			P->pivotRow = i;
		}
		++i;
	}
	P->resolvElem = M;
}

bool LinProg::Gomory::isProperSolution()
{
	for (auto v : P->Variables)
		if (v.MustBeInteger && v.value.getFracPart() != 0)
			return false;
	return true;
}


void LinProg::Gomory::demo()
{
	bool choise;

	cout << "CONSOLE INPUT - 1, READ FILE - 0 : ";
	cin >> choise;
	if (choise)
		consoleInput();
	else 
	{
		string filename;
		cout << "ENTER FILENAME ( must be stored here : " << getCD() << " )" << endl;
		cin.ignore(cin.rdbuf()->in_avail());
		getline(cin, filename);
		P->readFile(filename);
	}

	run();
	
}





