#pragma once

#include "Plane.h"
#include "Simplex.h"
//#include <string>
//#include <vector>

using namespace std;



namespace LinProg
{
	class Gomory
	{
		Plane *P;
		Simplex S;
		void addRestriction(bool isFirstAlgo);
		void addNewBaseVar();
		Rnum secondAlgoCountCoeff(int i);
		void findPivot();
		void findMaxFracPart();
		bool isProperSolution();

	public:

		Gomory(Plane *_arg) : P(_arg) {};
		Gomory() : P(new Plane) {};
		~Gomory();
		void readFile(string filename);
		void consoleInput();
		void run();
		void run1();
		void run2();
		void demo();
	};

}