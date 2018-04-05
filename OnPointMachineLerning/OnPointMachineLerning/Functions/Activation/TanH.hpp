#pragma once
#include "ActivationFunction.hpp"
#include <cmath>

namespace opml
{
	struct TanH : public ActivationFunction
	{
		inline double activation(double x) override
		{
			return tanh(x);
		}

		inline double activation_prime(double x) override
		{
			return 1.0 - std::pow(tanh(x), 2);
		}
	};
}  // namespace opml