#pragma once
#include "ActivationFunction.hpp"

namespace opml
{
	class Linear : public ActivationFunction
	{
	public:
		inline double activation(double x) override
		{
			return x;
		}

		inline double activation_prime(double x) override
		{
			return 1;
		}
	};
}  // namespace opml