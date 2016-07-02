#pragma once
class GenAlgorithm
{
	int a, b, c = 1, d = 2, N = 6, Count = 100, *individuals;
	double Lx, Ly, hx, hy, *fitness, Y = 0, F;
	std::pair< std::vector<bool>, std::vector<bool>> *chr, *newchr;
	typedef double(*func)(double, double);
	func fun;

	double fRand(double, double);
	std::vector<bool> to_binary(size_t, size_t);
	int to_dec(std::vector<bool>);
	void MakeStep();
	void Cover();
	void Roulette();
	void Crossover();
	void Mutation();
	void Fitness();

public:
	GenAlgorithm(int, int, int, int, int, int, func);
	void Loop();
};

