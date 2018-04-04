#pragma once

namespace opml
{
	struct ActivationFunction
	{
		virtual inline double activation(double x) = 0;
		virtual inline double activation_prime(double x) = 0;
	};
} // namespace opml