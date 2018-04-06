#pragma once
#include "ActivationFunction.hpp"

namespace opml
{
	class Linear : public ActivationFunction
	{
	public:
		inline double activation(double x) const noexcept override
		{
			return x;
		}

		inline double activation_prime(double x) const noexcept override
		{
			return 1;
		}
	};
}  // namespace opml