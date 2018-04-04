#pragma once
#include "Network.hpp"

namespace opml
{
	class NetworkBuilder
	{
	public:
		NetworkBuilder(size_t input_depth, size_t input_width, size_t input_height)
			: inputLayer(input_depth, input_width, input_height)
		{
			inputLayer.set_output_error_values(vector3D(input_depth, vector2D(input_width, std::vector<double>(input_height, 0.0))));
			inputLayer.set_output_derivative_values(inputLayer.get_output_error_values());
			inputLayer.set_output_values(inputLayer.get_output_error_values());
		}

		void addLayer(Layer* layer)
		{
			layers.emplace_back(layer);
		}

		Network buildNetwork()
		{
			Layer* b = &inputLayer;
			for (auto & layer : layers)
			{
				layer->connectToPreviouseLayer(b);
				b = layer;
			}
			
			outputLayer.connectToPreviouseLayer(b);

			return Network(&inputLayer, &outputLayer);
		}
	private:
		InputLayer inputLayer;
		OutputLayer outputLayer{};
		std::vector<Layer*> layers{};
	};
} // namespace opml