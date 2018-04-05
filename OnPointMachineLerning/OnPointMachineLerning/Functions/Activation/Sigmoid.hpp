#pragma once
#include "ActivationFunction.hpp"
#include <cmath>

namespace opml
{
	struct Sigmoid : public ActivationFunction
	{
		inline double activation(double x) override
		{
			return 1.0 / (1.0 + std::exp(-x));
		}

		inline double activation_prime(double x) override
		{
			double temp = activation(x);
			return  temp * (1 - temp);
		}
	};
}  // namespace opml