#pragma once
#include "Functions/Crossover/CrossoverFunction.hpp"
#include "../Functions/Activation/ActivationFunction.hpp"
#include <memory>
#include "../Tools/Random.hpp"
#include "../Tools/ArrayTools.hpp"

namespace opml
{
	class EvoNet
	{
	public:
		
	EvoNet(const std::vector<size_t> &inputSizes)
		: layer_sizes(inputSizes)
	{

		this->input_size = this->layer_sizes[0];
		this->network_size = this->layer_sizes.size();
		this->output_size = this->layer_sizes[this->network_size - 1];

		this->output.resize(this->network_size);
		this->weights.resize(this->network_size);

		for (size_t i = 0; i < this->network_size; ++i)
		{
			output[i].reserve(this->layer_sizes[i]);
			if (i > 0)
			{
				this->weights[i].resize(this->layer_sizes[i], std::vector<double>(this->layer_sizes[i - 1], 0.0));
			}
		}
		tools::randomizeVec(&this->weights, this->lowerWeightsRange, this->upperWeightsRange);
	}

		std::vector<double> calculate(const std::vector<double> &input)
		{
			this->output[0] = input;
			for (size_t layer = 1; layer < this->network_size; ++layer)
			{
				for (size_t neuron = 0; neuron < this->layer_sizes[layer]; ++neuron)
				{
					double sum{ 0.0f };
					for (size_t prevNeuron = 0; prevNeuron < this->layer_sizes[layer - 1]; ++prevNeuron)
					{
						sum += this->output[layer - 1][prevNeuron] * this->weights[layer][neuron][prevNeuron];
					}
					output[layer][neuron] = activationFunction->activation(sum);
				}
			}
			return output[network_size - 1];
		}

		void mutate(double mutationRate)
		{
			for (vector2D &vec2 : weights)
			{
				for (std::vector<double> &vec : vec2)
				{
					for (double &d : vec)
					{
						if (rng.randomReal<double>(0, 1) < mutationRate)
						{
							d = rng.randomReal<double>(this->lowerWeightsRange, this->upperWeightsRange);
						}
					}
				}
			}
		}

		void crossover(const EvoNet &parent)
		{
			for (size_t i = 0; i < this->weights.size(); ++i)
			{
				for (size_t j = 0; j < this->weights[i].size(); ++j)
				{
					crossoverFunction->crossover(this->weights[i][j], parent.weights[i][j], rng);
				}
			}
		}

	private:
		std::vector<size_t> layer_sizes;
		size_t input_size, output_size, network_size;

		vector2D output;
		vector3D weights;

		double lowerWeightsRange = -0.5f, upperWeightsRange = 0.5f;
		std::shared_ptr<CrossoverFunction> crossoverFunction;
		std::shared_ptr<ActivationFunction> activationFunction;

		static rng_mt19937_std rng;
	};
} // namespace opml
