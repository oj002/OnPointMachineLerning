#pragma once
#include <vector>
#include "../../../Utils/Utils.hpp"

namespace opml
{
	struct CrossoverFunction
	{
		virtual inline std::vector<double> crossover(const std::vector<double> &parentA, const std::vector<double> &parentB) const = 0;
	};
} // namespace opml
