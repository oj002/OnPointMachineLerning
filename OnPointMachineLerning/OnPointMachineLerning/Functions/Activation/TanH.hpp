#pragma once
#include "ActivationFunction.hpp"
#include <cmath>

namespace opml
{
	struct TanH : public ActivationFunction
	{
		inline double activation(double x) const noexcept override
		{
			try
			{
				return std::tanh(x);
			} catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}

		inline double activation_prime(double x) const noexcept override
		{
			return 1.0 - std::pow(activation(x), 2);
		}
	};
}  // namespace opml