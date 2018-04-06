#pragma once
#include "ActivationFunction.hpp"
#include <cmath>

namespace opml
{
	struct LeakyReLU : public ActivationFunction
	{
		inline double activation(double x) const noexcept override
		{
			if (x > 0) { return x; }
			return 0.05 * x;
		}

		inline double activation_prime(double x) const noexcept override
		{
			if (x > 0) { return 1; }
			return 0.05;
		}
	};
} // namespace opml