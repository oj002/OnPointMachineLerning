#pragma once
#include "ActivationFunction.hpp"
#include <cmath>

namespace opml
{
	struct Sigmoid : public ActivationFunction
	{
		inline double activation(double x) const noexcept override
		{
			try
			{
				return 1.0 / (1.0 + std::exp(-x));
			}
			catch (std::exception &e) { out_opml << e.what() << '\n'; return x; }
		}

		inline double activation_prime(double x) const noexcept override
		{
			double temp = activation(x);
			return temp * (1 - temp);
		}
	};
}  // namespace opml