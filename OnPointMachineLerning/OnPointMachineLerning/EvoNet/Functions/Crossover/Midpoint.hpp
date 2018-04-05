#pragma once
#include "CrossoverFunction.hpp"

namespace opml
{
	struct Midpoint : public CrossoverFunction
	{
		virtual inline std::vector<double> crossover(const std::vector<double> &parentA, const std::vector<double> &parentB, rng_mt19937_std &rng)
		{
			assert(parentA.size() == parentB.size());
			if (parentA.size() == 1)
			{
				return (rng.randomInteger<size_t>(0, 1) ? parentA : parentB);
			}
			size_t mid{ rng.randomInteger<size_t>(0, parentA.size() - 1) };

			std::vector<double> child{ parentA };
			if (mid == 0)
			{
				return parentB;
			}
			if (mid == parentA.size() - 1)
			{
				return parentA;
			}

			child.erase(child.begin() + mid, child.end());
			child.insert(child.end(), parentB.begin() + mid, parentB.end());

			assert(child.size() == parentA.size() && child.size() == parentB.size());
			return child;
		}
	};
} // namespace opml
