#pragma once
#include "../../Layers/OutputLayer.hpp"

namespace opml
{
	struct ErrorFunction
	{
		virtual inline double overall_error(const OutputLayer& outputLayer, const vector3D& target) = 0;
		virtual inline void apply(OutputLayer* outputLayer, const vector3D& target) = 0;
	};
} // namespace opml