#pragma once
#include "../../../Utils/Utils.hpp"
#include <vector>

namespace opml
{
	struct CrossoverFunction
	{
		virtual inline std::vector<double> crossover(const std::vector<double> &parentA, const std::vector<double> &parentB) const = 0;
	};
} // namespace opml
