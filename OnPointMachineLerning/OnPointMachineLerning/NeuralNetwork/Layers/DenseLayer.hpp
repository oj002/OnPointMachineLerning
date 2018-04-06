#pragma once
#include "../../Functions/Activation/ActivationFunction.hpp"
#include "../../Utils/Utils.hpp"
#include "Layer.hpp"
#include <memory>

namespace opml
{
	class DenseLayer : public Layer
	{
	public:
		explicit DenseLayer(size_t OUTPUT_HEIGHT)
			: Layer(1, 1, OUTPUT_HEIGHT)
		{}

		void calculate() override
		{
			const std::vector<double> prev_output_values00(this->prev_layer->get_output_values()[0][0]);
			std::vector<double>& output_values00(this->output_values[0][0]);
			std::vector<double>& output_derivative_values00(this->output_derivative_values[0][0]);

			OPML_PRAGMA_OMP(parallel for)
			for (int i = 0; i < this->OUTPUT_HEIGHT; ++i)
			{
				double sum = bias[i];
				std::vector<double>& weighti(this->weights[i]);

				for (size_t prevNeuron = 0; prevNeuron < this->INPUT_HEIGHT; ++prevNeuron)
				{
					sum += prev_output_values00[prevNeuron] * weighti[prevNeuron];
				}

				output_values00[i] = activationFunction->activation(sum);
				output_derivative_values00[i] = activationFunction->activation_prime(sum);
			}
		}
		void backpropError() override
		{
			vector3D temp = this->prev_layer->get_output_error_values();

			std::vector<double>& temp00(temp[0][0]);
			const std::vector<double>& output_error_values00(this->output_error_values[0][0]);
			const std::vector<double> prev_output_derivative_values00(this->prev_layer->get_output_derivative_values()[0][0]);

			OPML_PRAGMA_OMP(parallel for)
			for (int i = 0; i < this->INPUT_HEIGHT; ++i)
			{
				double sum = 0.0;

				for (size_t n = 0; n < this->OUTPUT_HEIGHT; ++n)
				{
					sum += this->weights[n][i] * output_error_values00[n];
				}

				temp00[i] = prev_output_derivative_values00[i] * sum;
			}

			this->prev_layer->set_output_error_values(temp);
		}
		void updateWeights(double eta) override
		{
			const std::vector<double>& output_error_values00(this->output_error_values[0][0]);
			const std::vector<double> prev_output_values00(this->prev_layer->get_output_values()[0][0]);

			OPML_PRAGMA_OMP(parallel for)
			for (int i = 0; i < this->OUTPUT_HEIGHT; ++i)
			{
				double delta = -eta * output_error_values00[i];
				this->bias[i] += delta;
				std::vector<double>& weighti(this->weights[i]);

				for (size_t p = 0; p < this->INPUT_HEIGHT; ++p)
				{
					weighti[p] += delta * prev_output_values00[p];
				}
			}
		}

		DenseLayer& setActivationFunction(const std::shared_ptr<ActivationFunction>& activationFunc)
		{
			this->activationFunction = activationFunc;
			return *this;
		}

		DenseLayer& weightsRange(double lower, double upper)
		{
			this->lowerWeightsRange = lower;
			this->upperWeightsRange = upper;
			return *this;
		}

		DenseLayer& biasRange(double lower, double upper)
		{
			this->lowerBiasRange = lower;
			this->upperBiasRange = upper;
			return *this;
		}

	protected:
		void calculateOutputDimensions() override {}
		void on_build() override
		{
			weights.resize(this->OUTPUT_HEIGHT, std::vector<double>(this->INPUT_HEIGHT, 0.0));
			bias.resize(this->OUTPUT_HEIGHT, 0.0);

			randomizeVec(&weights, this->lowerWeightsRange, this->upperWeightsRange);
			randomizeVec(&bias, this->lowerBiasRange, this->upperBiasRange);
		}

	private:
		double lowerWeightsRange = 0.0, upperWeightsRange = 1.0;
		double lowerBiasRange = 0.0, upperBiasRange = 1.0;

		vector2D weights{};
		std::vector<double> bias{};

		std::shared_ptr<ActivationFunction> activationFunction;
	};
}  // namespace opml
