// KAW_HA8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

long long gcd(long long a, long long b) {
	return b == 0 ? a : gcd(b, a % b);
}

void print_vector(vector<unsigned char> input)
{
	for (auto i : input)
	{
		std::cout << static_cast<int>(i) << " ";
	}
}

bool test_solution(vector<unsigned char> solution, std::vector<std::pair<long long, std::vector<unsigned char>>> smooth_candidates)
{
	std::vector<unsigned char> result(smooth_candidates[0].second.size(), 0);
	for (int j = 0; j < solution.size(); ++j)
	{
		auto s = solution[j];
		if (s == 0)
		{
			continue;
		}
		int i = 0;
		for (auto digit : smooth_candidates[j].second)
		{
			result[i] += digit;
			result[i] %= 2;
			i += 1;
		}
	}

	for (auto r : result)
	{
		if (r != 0)
		{
			return false;
		}
	}
	return true;
	
}

vector<unsigned char> next_permutation(vector<unsigned char> input)
{

	bool carry = true;
	vector<unsigned char> result;
	for (int i = 0; i < input.size(); ++i)
	{
		int current_digit = input[i];
		
		if (carry)
		{
			if (current_digit == 0)
			{
				result.push_back(1);
				carry = false;
			}
			else
			{
				result.push_back(0);
				carry = true;
			}
		}
		else
		{
			result.push_back(current_digit);
		}
	}
	return result;
}

std::vector<long long> factorize(long long a)
{
	std::vector<long long> result;
	long long upper_bound = static_cast<long long>(a / 2);
	for (int i = 2; i <= upper_bound; ++i)
	{
		while((a % i) == 0)
		{
			a /= i;
			result.push_back(i);
		}
		if (a == 1)
		{
			return result;
		}
	}
	return std::vector<long long> { a };
}

bool isPrime(long long a)
{
	if (a < 2)
	{
		return false;
	}

	long long upper_bound = static_cast<long long>(sqrt(a));
	for (int i = 2; i <= upper_bound; ++i)
	{
		if ((a % i) == 0)
		{
			return false;
		}
	}
	return true;
}

// Source: http://martin-thoma.com/images/2013/09/Calculate-Legendre.png
long long calcLegendre(long long a, long long p)
{
	// assert(p >= 3);

	if (a >= p || a < 0)
	{
		return calcLegendre(a % p, p);
	}
	else if (a == 0 || a == 1)
	{
		return a;
	}
	else if (a == 2)
	{
		return (std::abs(p % 8ll) == 1) ? 1 : -1;
	}
	else if (a == (p - 1))
	{
		return ((p % 4) == 1) ? 1 : -1;
	}
	else if (!isPrime(a))
	{
		auto factors = factorize(a);
		long long sum = 1;
		for (auto f : factors)
		{
			sum *= calcLegendre(f, p);
		}
		return sum;
	}
	else
	{
		bool test_1 = (((p - 1) / 2) % 2) == 0;
		bool test_2 = (((a - 1) / 2) % 2) == 0;
		if (test_1 || test_2)
		{
			return calcLegendre(p, a);
		}
		else
		{
			return -calcLegendre(p, a);
		}
	}

	return 1;
}

std::vector<long long> calc_factor_base(long long n, long long max_prime)
{
	std::vector<long long> factor_base;

	for (int i = 2; i < max_prime; ++i)
	{
		if (isPrime(i) && calcLegendre(n, i) == 1)
		{
			factor_base.push_back(i);
		}
	}

	return factor_base;
}

int _tmain(int argc, _TCHAR* argv[])
{
	long long n = 99007;
	int max_prime = 20;
	int range = 15;

	std::cout << "The used parameters are: " << std::endl;
	std::cout << "N = " << n << ", p_i < " << max_prime << ", interval +-" << range << std::endl << std::endl;

	std::vector<long long> factor_base = calc_factor_base(n, max_prime);

	std::map<long long, std::pair<long long, long long>> roots;
	for (auto p : factor_base)
	{
		auto n_p = n % p;
		for (int x = 0; x < p; ++x)
		{
			if (((x * x) % p) == n_p)
			{
				auto pair = std::make_pair(x, p - x);
				roots.insert(std::make_pair(p, pair));
			}
		}
	}

	long long lower_bound = std::floor(std::sqrt(n)) - range;
	long long upper_bound = lower_bound + range + range;
	std::map<long long, long long> candidates;
	for (long long i = lower_bound; i <= upper_bound; ++i)
	{
		auto next = (i * i - n);
		candidates.insert(std::make_pair(i, next));
	}

	// check for b smooth
	std::vector<std::pair<long long, std::vector<unsigned char>>> smooth_candidates;
	for (auto c : candidates)
	{
		std::vector<unsigned char> binaries;

		if (c.second < 0)
		{
			c.second *= -1;
			binaries.push_back(1);
		}
		else
		{
			binaries.push_back(0);
		}

		for (auto p : factor_base)
		{
			unsigned char next_binary = 0;
			while ((c.second % p) == 0)
			{
				next_binary += 1;
				c.second /= p;
			}
			binaries.push_back(next_binary % 2);
		}
		if (c.second == 1)
		{
			auto pair = std::make_pair(c.first, binaries);
			smooth_candidates.push_back(pair);
		}
	}


	std::vector<unsigned char> solution;
	for (int i = 0; i < smooth_candidates.size(); ++i)
	{
		solution.push_back(0);
	}

	int max_perm = static_cast<int>(std::pow(2, smooth_candidates[0].second.size()));
	for (int i = 0; i < max_perm; ++i)
	{
		solution = next_permutation(solution);
		if (test_solution(solution, smooth_candidates))
		{
			break;
		}
	}

	long long x = 1;
	long long y = 1;
	for (int i = 0; i < solution.size(); ++i)
	{
		if (solution[i] == 1)
		{
			x *= smooth_candidates[i].first;
			x %= n;

			y *= (smooth_candidates[i].first * smooth_candidates[i].first) - n;
		}
	}
	y = static_cast<long long>(std::sqrt(y)) % n;

	long long fac_1 = gcd(x - y, n);
	long long fac_2 = gcd(x + y, n);


	// Print results
	std::cout << "The base and roots are:" << std::endl;
	for (auto r : roots)
	{
		std::cout << r.first << ":" << r.second.first << " " << r.second.second << std::endl;
	}
	std::cout << std::endl;

	std::cout << "The power smooth values and vectors are: " << std::endl;
	for (auto s : smooth_candidates)
	{
		std::cout << s.first << ":" ;
		for (auto digit : s.second)
		{
			std::cout << (int)digit;
		}
		std::cout << std::endl;
	}

	std::cout << "A correct solution vector is: ";
	print_vector(solution);
	std::cout << std::endl << std::endl;
	 
	std::cout << "This yiels the factorization: " << std::endl;
	std::cout << fac_1 << "*" << fac_2 << "=" << fac_1 * fac_2 << std::endl;


	return 0;
}