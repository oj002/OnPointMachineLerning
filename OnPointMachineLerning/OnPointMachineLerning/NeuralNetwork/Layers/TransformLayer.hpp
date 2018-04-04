#pragma once
#include "Layer.hpp"

namespace opml
{
	class TransformLayer : public Layer
	{
	public:
		void calculate() override
		{
			std::vector<double>& output_values00(this->output_values[0][0]);
			std::vector<double>& output_derivative_values00(this->output_derivative_values[0][0]);

			#pragma omp parallel for
			for (int i = 0; i < this->INPUT_DEPTH; ++i) 
			{
				for (size_t n = 0; n< this->INPUT_WIDTH; ++n)
				{
					for (size_t j = 0; j < this->INPUT_HEIGHT; ++j)
					{
						int index = map(i, n, j);
						output_values00[index] = this->prev_layer->get_output_values()[i][n][j];
						output_derivative_values00[index] = this->prev_layer->get_output_derivative_values()[i][n][j];
					}
				}
			}
		}
		void backpropError() override
		{
			vector3D temp = this->prev_layer->get_output_error_values();
;
			const std::vector<double>& output_error_values00(this->output_error_values[0][0]);
			const std::vector<double> prev_output_derivative_values00(this->prev_layer->get_output_derivative_values()[0][0]);

			#pragma omp parallel for
			for (int i = 0; i < this->INPUT_DEPTH; ++i) 
			{
				for (size_t n = 0; n< this->INPUT_WIDTH; ++n)
				{
					for (size_t j = 0; j < this->INPUT_HEIGHT; ++j)
					{
						int index = map(i, n, j);
						temp[i][n][j] = output_error_values00[index];
					}
				}
			}

			this->prev_layer->set_output_error_values(temp);
		}
		void updateWeights(double eta) override
		{
		}

	protected:
		void calculateOutputDimensions() override 
		{
			this->OUTPUT_DEPTH = 1;
			this->OUTPUT_WIDTH = 1;
			this->OUTPUT_HEIGHT = this->INPUT_HEIGHT * this->INPUT_DEPTH * this->INPUT_HEIGHT;
		}
		void on_build() override { }

	private:
		size_t map(size_t d, size_t w, size_t h)
		{
			return (d * (this->INPUT_WIDTH * this->INPUT_HEIGHT) + w * this->INPUT_HEIGHT + h);
		}

	private:
		vector2D weights{};
		std::vector<double> bias{};
	};
}  // namespace opml
