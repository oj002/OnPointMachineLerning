#pragma once

namespace opml
{
	struct ActivationFunction
	{
		virtual inline double activation(double x) const noexcept = 0;
		virtual inline double activation_prime(double x) const noexcept  = 0;
	};
} // namespace opml