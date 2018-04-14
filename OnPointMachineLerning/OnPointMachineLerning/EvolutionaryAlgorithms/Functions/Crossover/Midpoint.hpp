#pragma once
#include "CrossoverFunction.hpp"

namespace opml
{
	struct Midpoint : public CrossoverFunction
	{
		inline std::vector<double> crossover(const std::vector<double> &parentA, const std::vector<double> &parentB) const override
		{
			try
			{
				if (parentA.size() == 1)
				{
					return (opml::rng.next<size_t>(0, 1) != 0u ? parentA : parentB);
				}
				size_t mid{ opml::rng.next<size_t>(0, parentA.size() - 1) };

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

				return child;
			} OPML_INTERNAL_CATCH
		}
	};
} // namespace opml
