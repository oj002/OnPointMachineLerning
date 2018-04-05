#pragma once
#include "CrossoverFunction.hpp"

namespace opml
{
	struct Midpoint : public CrossoverFunction
	{
		virtual inline std::vector<double> crossover(const std::vector<double> &parentA, const std::vector<double> &parentB, rng_mt19937_std &rng)
		{
			size_t mid{ rng.randomInteger<size_t>(0, parentA.size()) };

			std::vector<double> child{ parentA };
			child.erase(child.begin() + mid);
			child.insert(child.end(), parentB.begin() + mid, parentB.end());
			return child;
		}
	};
} // namespace opml
