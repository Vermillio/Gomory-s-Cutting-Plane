#include <algorithm>
#include <iostream>
#include <string>
#include "Simplex.h"

using namespace std;
using namespace LinProg;


void Simplex::run()
{
	P->StepsCount = 0;
	if (P->operation == minimiz)
	{
		P->SimplexTable[0] = P->SimplexTable[0] * (-1);
		P->operation = maximiz; //???
	}
	makeBasis();
	for (;;)
	{
		++P->StepsCount;
		displayResult();
		if (isOptimalSolution())
		{
			cout << "SIMPLEX ---END---  OPTIMAL SOLUTION FOUND" << endl;
			break;
		}
		stepSimplex();
	}
}


void LinProg::Simplex::stepSimplex()
{
	findResolvElem();
	applyChangesToSystem();
}


//Solution is optimal if target function contains negative elements.
bool Simplex::isOptimalSolution()
{
	for (int i = 0; i < P->Columns; ++i)
		if (P->SimplexTable[0][i] < 0)
			return false;
	return true;
}

void Simplex::findResolvElem()
{
	P->pivotColumn = 0;
	P->pivotRow = 0;
	findPivotColumn();
	findPivotRow();
}

void LinProg::Simplex::findPivotColumn()
{
	auto theta = [](Plane *P, int i, int j) 
	{ 
		return		P->pivotRow == P->StartTargetFunc		? 
					P->SimplexTable[i][j] 					: 
					P->SimplexTable[P->StartTargetFunc][j] / P->SimplexTable[i][j]	; 
	};


	for (int i = 1; i < P->Columns - 1; ++i)
	{
		auto A = theta(P, P->pivotRow, i);
		auto B = theta(P, P->pivotRow, P->pivotColumn);
		if (A != 0 && (A < B || B == 0))
		{
			P->pivotColumn = i;
			P->resolvElem = P->SimplexTable[P->pivotRow][i];
		}
	}

}

void LinProg::Simplex::findPivotRow()
{
	P->resolvElem = P->SimplexTable[1][P->pivotColumn];
	P->pivotRow = 1;
	Rnum tmpAbs = (P->SimplexTable[P->pivotRow][P->Columns - 1] / P->resolvElem).getAbs();
	for (int j = 2; j < P->Rows; ++j)
		if ((P->SimplexTable[j][P->Columns - 1] / P->SimplexTable[j][P->pivotColumn]).getAbs() < tmpAbs)
		{
			P->resolvElem = P->SimplexTable[j][P->pivotColumn];
			P->pivotRow = j;
			tmpAbs = (P->SimplexTable[P->pivotRow][P->Columns - 1] / P->resolvElem).getAbs();
		}
	P->BasisVars[P->pivotRow] = P->pivotColumn;
}

void Simplex::applyChangesToSystem()
{
	P->SimplexTable[P->pivotRow] = P->SimplexTable[P->pivotRow] / P->resolvElem;

	for (int i = 0; i < P->Rows; ++i)
		if (i != P->pivotRow)
		{
			vector<Rnum> tmp = P->SimplexTable[P->pivotRow] * P->SimplexTable[i][P->pivotColumn];
				P->SimplexTable[i] = P->SimplexTable[i] - tmp;
		}

	simplifyTable();
	updateSolution();
}

void Simplex::updateSolution()
{
	for (int i = 0; i < P->Rows; ++i)
	{
		if (P->BasisVars[i] < P->InputVarNum)
			P->Variables[P->BasisVars[i]].value = P->SimplexTable[i][P->Columns - 1];
		else if (P->BasisVars[i] == P->InputVarNum)
			P->targetValue = P->SimplexTable[i][P->Columns - 1];
	}
}

void LinProg::Simplex::makeBasis()
{
	for (int i = 0; i < P->Rows; ++i)
	{
		if (P->BasisVars[i] < P->InputVarNum)
		{
			for (int j = 0; j < P->Rows; ++j)
			{
				if (j != i && P->SimplexTable[j][P->BasisVars[i]] != Rnum(0))
				{
					P->SimplexTable[j] = P->SimplexTable[j] - P->SimplexTable[i] / P->SimplexTable[i][P->BasisVars[i]] * P->SimplexTable[j][P->BasisVars[i]];
				}
			}
		}
	}
}

void LinProg::Simplex::simplifyTable()
{
	for (int i = 0; i < P->Rows; ++i)
		for (int j = 0; j < P->Columns; ++j)
			P->SimplexTable[i][j].simplify();
}

void LinProg::Simplex::displayResult()
{
	cout << " STEP #" << P->StepsCount << endl;
	for (int i = 0; i < P->Rows; ++i)
	{
		cout << "| ";
		for (int j = 0; j < P->Columns; ++j)
			cout << P->SimplexTable[i][j] << " ";
		cout << "|" << endl;
	}
	cout << " SOLUTION" << endl;
	cout << "--------------------------" << endl;
	for (auto v : P->Variables)
		cout << " " << v.name << " = " << v.value << ";" << endl;
	cout << " Target Value = " << P->targetValue << endl;
	cout << "--------------------------" << endl;
	cout << endl;
}

void LinProg::Simplex::insertColumn()
{
	for (int i = 0; i < P->Rows; ++i)
		P->SimplexTable[i].insert(P->SimplexTable[i].begin() + P->Columns - 1, { Rnum(0) });
}

void LinProg::Simplex::insertRow(int pos)
{
	P->SimplexTable.insert(P->SimplexTable.begin() + pos, vector<Rnum>(P->Columns));
	if (pos<P->StartTargetFunc)
		++P->StartTargetFunc;
	if (pos<P->pivotRow)
		++P->pivotRow;
	++P->Rows;
}
