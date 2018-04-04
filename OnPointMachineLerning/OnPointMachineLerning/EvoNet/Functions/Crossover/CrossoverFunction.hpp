#pragma once
#include <vector>
#include "../../../Tools/Random.hpp"

namespace opml
{
	struct CrossoverFunction
	{
		virtual inline std::vector<double> crossover(const std::vector<double> &parentA, const std::vector<double> &parentB, rng_mt19937_std &rng) = 0;
	};
} // namespace opml
