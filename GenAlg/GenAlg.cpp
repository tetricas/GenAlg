// GenAlg.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

double Function(double x, double y)
{
	return x*x + y*y;
}

int main()
{
	GenAlgorithm genAl(0, 4, 1, 2, 6, 10, &Function);
	genAl.Loop();

	system("PAUSE");
	return 0;
}