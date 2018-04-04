#pragma once
#include "Layer.hpp"

namespace opml
{
	class InputLayer : public Layer
	{
	public:
		InputLayer(size_t OUTPUT_DEPTH, size_t OUTPUT_WIDTH, size_t OUTPUT_HEIGHT)
			: Layer(OUTPUT_DEPTH, OUTPUT_WIDTH, OUTPUT_HEIGHT)
		{
			this->INPUT_DEPTH = OUTPUT_DEPTH;
			this->INPUT_WIDTH = OUTPUT_WIDTH;
			this->INPUT_HEIGHT = OUTPUT_HEIGHT;
		}

		void setInput(const vector3D&  in)
		{
			if (this->matchingDimensions(in))
			{
				this->output_values = in;
			}
		}

		void calculate() override{}
		void backpropError() override{}
		void updateWeights(double   /*eta*/) override{}

	protected:
		void calculateOutputDimensions() override{}
		void on_build() override{}

	};
}  // namespace opml