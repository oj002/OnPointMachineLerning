#pragma once
#include "ActivationFunction.hpp"

namespace opml
{
	class ReLU : public ActivationFunction
	{
	public:
		inline double activation(double x) const noexcept override
		{
			if (x > 0) { return x; }
			return 0;
		}

		inline double activation_prime(double x) const noexcept override
		{
			if (x > 0) { return 1; }
			return 0; // no learning
		}
	};
}  // namespace opml