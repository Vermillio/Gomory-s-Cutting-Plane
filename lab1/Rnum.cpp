#include "Rnum.h"
#include <iostream>
#include <algorithm>
using namespace std;
using namespace LinProg;

void Rnum::simplify()
{
	int tmp = GCD(abs(num), abs(denom));
	if (tmp)
	{
		num /= tmp;
		denom /= tmp;
	}
	if (num < 0 && denom < 0)
	{
		num = -num;
		denom = -denom;
	}
	else if (num == 0)
		denom = 1;
}

Rnum Rnum::getFracPart()
{
	int n = num;
	int entier = getEntier();
	return Rnum(n, denom) - entier;
}

Rnum LinProg::Rnum::getAbs()
{
	return Rnum(abs(num), abs(denom));
}

int LinProg::Rnum::getEntier()
{
	return floor((double)num / (double)denom);
}

int LinProg::Rnum::getNum()
{
	return num;
}

int LinProg::Rnum::getDenom()
{
	return denom;
}

Rnum & LinProg::Rnum::operator*=(Rnum & y)
{
	return ((*this) * y);
}


ostream & LinProg::operator<<(ostream & c, Rnum x)
{
	c << x.num << '/' << x.denom;
	return c;
}

vector<Rnum> LinProg::operator/= (vector<Rnum> X, Rnum divider)
{
	vector<Rnum> Z(X);
	for (int i = 0; i < X.size(); ++i)
		Z[i] /= divider;
	return Z;
}

Rnum LinProg::operator/= (Rnum x, Rnum y)
{
	return x / y;
}

Rnum LinProg::operator/ (Rnum x, Rnum y)
{
	if (x.num == 0 || y.num == 0)
		return Rnum(0);
	x.num *= y.denom;
	x.denom *= y.num;
	return x;
}

Rnum LinProg::operator-(Rnum x, Rnum y)
{
	x.num *= y.denom;
	x.denom *= y.denom;
	x.num -= y.num * x.denom / y.denom;
	return x;
}

Rnum LinProg::operator*(Rnum x, Rnum y)
{
	x.num *= y.num;
	x.denom *= y.denom;
	return x;
}

bool LinProg::operator< (Rnum x, int y)
{
	if (x.num < x.denom * y)
		return true;
	return false;
}

bool LinProg::operator< (Rnum x, Rnum y)
{
	if (x.denom < 0)
	{
		x.denom = -x.denom;
		x.num = -x.num;
	}
	if (y.denom < 0)
	{
		y.denom = -y.denom;
		y.num = -y.num;
	}
	if (x.num * y.denom < y.num * x.denom)
		return true;
	return false;
}

bool LinProg::operator!=(Rnum x, Rnum y)
{
	return !(x == y);
}

bool LinProg::operator==(Rnum x, Rnum y)
{
	return x.num*y.denom == y.num*x.denom;
}

bool LinProg::operator> (Rnum x, Rnum y)
{
	if (x.denom < 0)
	{
		x.denom = -x.denom;
		x.num = -x.num;
	}
	if (y.denom < 0)
	{
		y.denom = -y.denom;
		x.num = -x.num;
	}
	if (x.num*y.denom > y.num*x.denom)
		return true;
	return false;
}

bool LinProg::operator>=(Rnum x, Rnum y)
{
	return x > y || x == y;
}

vector<Rnum> LinProg::operator/(vector<Rnum> X, Rnum divider)
{
	for (int i = 0; i < X.size(); ++i)
		X[i] = X[i] / divider;
	return X;
}

vector<Rnum> LinProg::operator*=(vector<Rnum> X, Rnum multiplier)
{
	return X * multiplier;
}

vector<Rnum> LinProg::operator* (vector<Rnum> X, Rnum multiplier)
{
	vector<Rnum> Z(X);
	for (int i = 0; i < X.size(); ++i)
		Z[i] = Z[i] * multiplier;
	return Z;
}

vector<Rnum> LinProg::operator- (vector<Rnum> X, vector<Rnum> Y)
{

	int maxsz = max(X.size(), Y.size());

	vector<Rnum> Z(maxsz);
	for (int i = 0; i < maxsz; ++i)
	{
		Z[i] = X[i] - Y[i];
	}

	return Z;
}

int LinProg::GCD(int a, int b)
{
	if (!a)
		return b;
	if (!b)
		return a;
	int x = max(a, b), y = min(a, b);
	int tmp = x;
	do
	{
		x > y ? x = tmp : y = tmp;
		tmp = x > y ? x % y : y % x;
	} while (tmp != 0);
	return x > y ? y : x;
}