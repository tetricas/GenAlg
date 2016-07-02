#include "stdafx.h"
#include "GenAlgorithm.h"


GenAlgorithm::GenAlgorithm(int A, int B, int C, int D, int n, int count, func function) :a(A), b(B), c(C), d(D), N(n), Count(count), fun(function)
{
	chr = new std::pair< std::vector<bool>, std::vector<bool>>[N];
	newchr = new std::pair< std::vector<bool>, std::vector<bool>>[N];
	individuals = new int[N];
	fitness = new double[N];
}


double GenAlgorithm::fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

std::vector<bool> GenAlgorithm::to_binary(size_t n, size_t count)
{
	std::vector<bool> result;
	do
	{
		result.push_back(n % 2);
		n /= 2;
	} while (n > 0);
	while (result.size() < count)
		result.push_back(0);
	std::reverse(result.begin(), result.end());
	return result;
}

int GenAlgorithm::to_dec(std::vector<bool> vec)
{
	int dec = (int)vec[vec.size() - 1];
	for (int i = vec.size() - 2, j = 1; i >= 0; --i, ++j)
		dec += (int)vec[i] * pow(2, j);
	return dec;
}

void GenAlgorithm::MakeStep()
{
	Lx = roundf(log(fabs(b - a)*pow(10, 1) + 1) / log(2)), Ly = roundf(log(fabs(d - c)*pow(10, 1) + 1) / log(2));
	hx = ((b - a) / (pow(2, Lx) - 1));
	hy = ((d - c) / (pow(2, Ly) - 1));
}

void GenAlgorithm::Cover()
{
	double is = (double)(b - a) / 4, js = (double)(d - c) / 3;
	double r = a + is, t = c + js;
	int count = 0;
	for (int i(0); i < 3; ++i, r += is, t -= 2 * js)
		for (size_t i = 0; i < 2; i++, t += js)
		{
			fitness[count] = fun(r, t);
			Y += fitness[count];

			chr[count] = std::make_pair(to_binary(fabs(r - a) / hx, (int)Lx), to_binary(fabs(t - c) / hy, (int)Ly));
			count++;
		}
	F = Y / N;
}

void GenAlgorithm::Roulette()
{
	double Fmin = fitness[0];
	for (int i(0); i < 6; ++i)
		if (fitness[i] < Fmin)
			Fmin = fitness[i];
	if (Fmin < 0)
		for (int i(0); i < 6; ++i)
			fitness[i] += 2 * fabs(Fmin);
	for (int i(0); i < 6; ++i)
	{
		double R = fRand(0, Y);
		if (R >= 0 && R < fitness[0])
			individuals[i] = 0;
		else if (R >= fitness[0] && R < fitness[0] + fitness[1])
			individuals[i] = 1;
		else if (R >= fitness[0] + fitness[1] && R < fitness[0] + fitness[1] + fitness[3])
			individuals[i] = 2;
		else if (R >= fitness[0] + fitness[1] + fitness[3] && R < fitness[0] + fitness[1] + fitness[3] + fitness[4])
			individuals[i] = 3;
		else if (R >= fitness[0] + fitness[1] + fitness[3] + fitness[4] && R < fitness[0] + fitness[1] + fitness[3] + fitness[4] + fitness[5])
			individuals[i] = 4;
		else if (R >= fitness[0] + fitness[1] + fitness[3] + fitness[4] + fitness[5] && R < Y)
			individuals[i] = 5;
	}
}

void GenAlgorithm::Crossover()
{
	for (int i(0); i < 6; i += 2)
	{
		newchr[i].second.push_back(chr[individuals[i + 1]].second[0]);
		newchr[i + 1].second.push_back(chr[individuals[i]].second[0]);

		newchr[i].second.push_back(chr[individuals[i]].second[1]);
		newchr[i + 1].second.push_back(chr[individuals[i + 1]].second[1]);

		newchr[i].second.push_back(chr[individuals[i]].second[2]);
		newchr[i + 1].second.push_back(chr[individuals[i + 1]].second[2]);

		//------------------------------

		newchr[i].first.push_back(chr[individuals[i + 1]].first[0]);
		newchr[i + 1].first.push_back(chr[individuals[i]].first[0]);

		newchr[i].first.push_back(chr[individuals[i + 1]].first[1]);
		newchr[i + 1].first.push_back(chr[individuals[i]].first[1]);

		newchr[i].first.push_back(chr[individuals[i + 1]].first[2]);
		newchr[i + 1].first.push_back(chr[individuals[i]].first[2]);

		newchr[i].first.push_back(chr[individuals[i]].first[3]);
		newchr[i + 1].first.push_back(chr[individuals[i + 1]].first[3]);

		newchr[i].first.push_back(chr[individuals[i]].first[4]);
		newchr[i + 1].first.push_back(chr[individuals[i + 1]].first[4]);
	}
}

void GenAlgorithm::Mutation()
{
	for (size_t i(0); i < 6; ++i)
	{
		int r = rand() % 100;
		if (!r)
		{
			int rj = rand() % 8;
			if (rj < 5)
				newchr[i].first[rj] = !newchr[i].first[rj];
			else
			{
				rj -= 5;
				newchr[i].second[rj] = !newchr[i].second[rj];
			}
		}
	}
}

void GenAlgorithm::Fitness()
{
	Y = 0;
	double Fmin, x[6], y[6];
	for (size_t i = 0; i < 6; i++)
	{
		int xz = to_dec(newchr[i].first), yz = to_dec(newchr[i].second);
		x[i] = a + xz*hx, y[i] = c + yz*hy;
		fitness[i] = fun(x[i], y[i]);
		Y += fitness[i];
	}
	Fmin = fitness[0];
	int index = 0;
	for (int i(0); i < 6; ++i)
		if (fitness[i] < Fmin)
		{
			Fmin = fitness[i];
			index = i;
		}
	std::cout << " Fmin=" << Fmin << " x=" << x[index] << " y=" << y[index] << std::endl;
	F = Y / N;
}

void GenAlgorithm::Loop()
{
	MakeStep();
	Cover();
	for (int i(0); i < Count; ++i)
	{
		Roulette();
		Crossover();
		Mutation();
		Fitness();

		for (size_t j = 0; j < N; j++)
		{
			std::swap(newchr[j], chr[j]);
			newchr[j].first.clear();
			newchr[j].second.clear();
		}
	}
}
