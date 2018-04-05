#pragma once
#include "../../Functions/Activation/ActivationFunction.hpp"
#include "../../Tools/Utils.hpp"
#include "Layer.hpp"
#include <memory>

namespace opml
{
	class ConvLayer : public Layer
	{
	public:
		explicit ConvLayer(size_t channel_amount, size_t filter_size, size_t filter_stride, size_t padding)
			: channelAmount(channel_amount)
			, filterSize(filter_size)
			, filterStride(filter_stride)
			, padding(padding)
		{
		}

		void calculate() override
		{
			OPML_PRAGMA_OMP(parallel for)
			for (int i = 0; i < this->OUTPUT_DEPTH; ++i)
			{
				for (int j = 0; j < this->OUTPUT_WIDTH; ++j)
				{
					for (int n = 0; n < this->OUTPUT_HEIGHT; ++n) 
					{
						this->output_values[i][j][n] = activationFunction->activation(this->calcSample(i, j, n));
						this->output_derivative_values[i][j][n] = activationFunction->activation_prime(this->calcSample(i, j, n));
					}
				}
			}
		}
		double calcSample(int actIndex, int x, int y) 
		{
			double sum = bias[actIndex];
			for (int i = 0; i < this->filterSize; ++i)
			{
				for (int n = 0; n < this->filterSize; ++n)
				{
					for (int j = 0; j < this->INPUT_DEPTH; ++j)
					{
						int x_i = -padding + (x * this->filterStride) + i;
						int y_i = -padding + (y * this->filterStride) + n;
						if (x_i >= 0 && y_i >= 0 && x_i < this->INPUT_WIDTH && y_i < this->INPUT_HEIGHT) 
						{
							sum += filter[actIndex][j][i][n] * this->prev_layer->get_output_values()[j][x_i][y_i];
						}
					}
				}
			}
			return sum;
		}

		void backpropError() override
		{

		}
		void updateWeights(double eta) override
		{

		}

		ConvLayer& setActivationFunction(const std::shared_ptr<ActivationFunction>& activationFunc)
		{
			this->activationFunction = activationFunc;
			return *this;
		}

		ConvLayer& weightsRange(double lower, double upper)
		{
			this->lowerWeightsRange = lower;
			this->upperWeightsRange = upper;
			return *this;
		}

		ConvLayer& biasRange(double lower, double upper)
		{
			this->lowerBiasRange = lower;
			this->upperBiasRange = upper;
			return *this;
		}

		ConvLayer& setFiler(size_t index, vector3D filter, double bias)
		{
			this->bias[index] = bias;
			this->filter[index] = filter;
			return *this;
		}

	protected:
		void calculateOutputDimensions() override 
		{
			this->OUTPUT_DEPTH = this->channelAmount;
			this->OUTPUT_WIDTH = (this->INPUT_WIDTH + this->padding * 2.0 - this->filterSize) / this->filterStride + 1.0;
			this->OUTPUT_HEIGHT = (this->INPUT_HEIGHT + this->padding * 2.0 - this->filterSize) / this->filterStride + 1.0;

			double g = (static_cast<double>(this->INPUT_WIDTH) + static_cast<double>(this->padding) * 2.0 - static_cast<double>(this->filterSize)) / static_cast<double>(this->filterStride) + 1.0;
			double g1 = (static_cast<double>(this->INPUT_HEIGHT) + static_cast<double>(this->padding) * 2.0 - static_cast<double>(this->filterSize)) / static_cast<double>(this->filterStride) + 1.0;

			if (g != static_cast<size_t>(g) || g1 != static_cast<size_t>(g1)) { std::cerr << "Format does not work! Use a different padding-value!" << std::endl; assert(false); }
		}
		void on_build() override
		{
			filter.resize(this->channelAmount, vector3D(this->INPUT_DEPTH, vector2D(this->filterSize, std::vector<double>(this->filterSize, 0.0))));
			bias.resize(this->channelAmount, 0.0);

			tools::randomizeVec(&filter, this->lowerWeightsRange, this->upperWeightsRange);
			tools::randomizeVec(&bias, this->lowerBiasRange, this->upperBiasRange);
		}

	private:
		size_t channelAmount;
		size_t filterSize;
		size_t filterStride;
		size_t padding;

		double lowerWeightsRange = 0.0, upperWeightsRange = 1.0;
		double lowerBiasRange = 0.0, upperBiasRange = 1.0;

		vector4D filter{};
		std::vector<double> bias{};

		std::shared_ptr<ActivationFunction> activationFunction;
	};
}  // namespace opml
