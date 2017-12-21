#pragma once
#include "Rnum.h"

using namespace std;

namespace LinProg
{
	class Gomory;
	class Simplex;

	enum Type { maximiz, minimiz };

	
	struct var
	{
		string name;
		Rnum value;
		bool MustBeInteger;
		var() : name(""), value(0), MustBeInteger(false) {};
		var(string x) : name(x), value(0), MustBeInteger(false) {};
	};

	class Plane
	{
		int StepsCount;

		Type operation;

		int IntVarNum;

		int InputVarNum;

		int StartTargetFunc;

		int Columns;

		int Rows;

		int pivotRow;

		int pivotColumn;

		Rnum resolvElem;

		vector<int> BasisVars;

		vector<vector<Rnum>> SimplexTable;

		friend Simplex;
		friend Gomory;


		void init();

		void addTargetFunc(string s);

		void addCondition(string s, int index);

		void addIntVars(string s);

	public:

		//INPUT
		void consoleInput();

		void readFile(string filename);

		//SOLUTION DATA
		vector<var> Variables;

		Rnum targetValue;

		//CONSTRUCTORS
		Plane() {};

		Plane(string input, bool isfilename);

	};

	string getCD();
}