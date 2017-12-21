#pragma once
#include "Plane.h"
using namespace std;



namespace LinProg
{
	class Simplex
	{
		Plane *P;

	public:
		friend Gomory;

		Simplex() {};

		Simplex(Plane *_arg) : P(_arg) {};

		void run();

		void updateSolution();

		void displayResult();

	private:

		void makeBasis();

		void simplifyTable();

		void insertColumn();

		void insertRow(int pos);

		void stepSimplex();

		bool isOptimalSolution();

		void findResolvElem();

		void findPivotColumn();

		void findPivotRow();

		void applyChangesToSystem();

	};
}