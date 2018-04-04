#pragma once
#include "CrossoverFunction.hpp"

namespace opml
{
	struct Midpoint : public CrossoverFunction
	{
		virtual inline std::vector<double> crossover(const std::vector<double> &parentA, const std::vector<double> &parentB, rng_mt19937_std &rng)
		{
			size_t mid{ rng.randomInteger<size_t>(0, std::min(parentA.size(), parentB.size())) };

			std::vector<double> child{ parentA };
			child.emplace(child.begin() + mid, parentB.begin() + mid, parentB.end());
			return child;
		}
	};
} // namespace opml
