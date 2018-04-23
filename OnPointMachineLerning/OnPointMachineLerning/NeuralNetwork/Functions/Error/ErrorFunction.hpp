#pragma once
#include "../../Layers/OutputLayer.hpp"

namespace opml
{
	struct ErrorFunction
	{
		virtual inline double overall_error(const OutputLayer& outputLayer, const vector3D& target) const noexcept = 0;
		virtual inline void apply(gsl::not_null<OutputLayer*> outputLayer, const vector3D& target) const noexcept = 0;
	};
} // namespace opml