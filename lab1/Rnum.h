#pragma once
#include <vector>
using namespace std;

namespace LinProg
{
	class Rnum
	{
		int num;
		int denom;
	public:
		Rnum() : num(0), denom(1) {};
		Rnum(int x) : num(x), denom(1) {};
		Rnum(int p, int q) : num(p), denom(q) {};

		void simplify();
		Rnum getFracPart();
		Rnum getAbs();
		int getEntier();
		int getNum();
		int getDenom();


		friend ostream& operator<< (ostream&c, Rnum x);
		friend vector<Rnum> operator/= (vector<Rnum> X, Rnum divider);
		friend Rnum operator/= (Rnum x, Rnum y);
		friend Rnum operator/ (Rnum x, Rnum y);
		Rnum& operator*= (Rnum& y);
		friend Rnum operator- (Rnum x, Rnum y);
		friend Rnum operator* (Rnum x, Rnum y);
		friend bool operator< (Rnum x, int y);
		friend bool operator< (Rnum x, Rnum y);
		friend bool operator!= (Rnum x, Rnum y);
		friend bool operator== (Rnum x, Rnum y);
		friend bool operator> (Rnum x, Rnum y);
		friend bool operator>= (Rnum x, Rnum y);
		friend vector<Rnum> operator/ (vector<Rnum> X, Rnum divider);
		friend vector<Rnum> operator* (vector<Rnum> X, Rnum multiplier);
		friend vector<Rnum> operator*=(vector<Rnum> X, Rnum multiplier);
		friend vector<Rnum> operator- (vector<Rnum> X, vector<Rnum> Y);
	};

	int GCD(int a, int b);
}