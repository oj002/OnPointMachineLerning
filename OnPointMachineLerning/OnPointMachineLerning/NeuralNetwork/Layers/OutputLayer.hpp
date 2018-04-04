#pragma once
#include "Layer.hpp"
#include <memory>

namespace opml
{
	struct ErrorFunction;
	class OutputLayer : public Layer
	{
	public:
		OutputLayer()
			 
		= default;


		void calculate() override 
		{
			this->output_values = this->prev_layer->get_output_values();
			this->output_derivative_values = this->prev_layer->get_output_derivative_values();
		}

		void backpropError() override 
		{
			this->prev_layer->set_output_error_values(this->output_error_values);
		}
		void updateWeights(double  /*eta*/) override {}

		void calculateOutputErrorSignals(const vector3D& target);
		double overall_error(const vector3D& target);

		OutputLayer setErrorFunction(const std::shared_ptr<ErrorFunction>& errorFunc);

	protected:
		void calculateOutputDimensions() override 
		{
			this->OUTPUT_DEPTH = this->INPUT_DEPTH;
			this->OUTPUT_WIDTH = this->INPUT_WIDTH;
			this->OUTPUT_HEIGHT = this->INPUT_HEIGHT;
		}
		void on_build() override;
		

	private:
		std::shared_ptr<ErrorFunction> errorFunction;
	};
}  // namespace opml