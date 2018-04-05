#pragma once
#include "Functions/Crossover/CrossoverFunction.hpp"
#include "../Functions/Activation/ActivationFunction.hpp"
#include <memory>
#include "../Tools/Random.hpp"
#include "../Tools/ArrayTools.hpp"
#include <cassert>

namespace opml
{
	static rng_mt19937_std rng;
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
			output[i].resize(this->layer_sizes[i]);
			if (i > 0)
			{
				this->weights[i].resize(this->layer_sizes[i], std::vector<double>(this->layer_sizes[i - 1], 0.0));
			}
		}
		tools::randomizeVec(&this->weights, this->lowerWeightsRange, this->upperWeightsRange);
	}

		std::vector<double> calculate(const std::vector<double> &input)
		{
			assert(input.size() == this->input_size);
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

		void mutate(size_t chance)
		{
			for (vector2D &vec2 : weights)
			{
				for (std::vector<double> &vec : vec2)
				{
					for (double &d : vec)
					{
						if (rng.randomInteger<size_t>(0, chance) == 0)
						{
							d = rng.randomReal<double>(this->lowerWeightsRange, this->upperWeightsRange);
						}
					}
				}
			}
		}

		EvoNet crossover(const EvoNet &parent)
		{
			EvoNet ret{ *this };
			vector3D newWeights{ this->weights };
			for (size_t i = 0; i < this->weights.size(); ++i)
			{
				for (size_t j = 0; j < this->weights[i].size(); ++j)
				{
					if (this->weights[i][j].size() > 0)
					{
						newWeights[i][j] = crossoverFunction->crossover(this->weights[i][j], parent.weights[i][j], rng);
					}
				}
			}
			ret.weights = newWeights;
			return ret;
		}

		EvoNet &setActivationFunction(std::shared_ptr<ActivationFunction> activationFunction)
		{
			this->activationFunction = activationFunction;
			return *this;
		}

		EvoNet &setCrossoverFunction(std::shared_ptr<CrossoverFunction> crossoverFunction)
		{
			this->crossoverFunction = crossoverFunction;
			return *this;
		}

		EvoNet& weightsRange(double lower, double upper)
		{
			this->lowerWeightsRange = lower;
			this->upperWeightsRange = upper;
			tools::randomizeVec(&this->weights, this->lowerWeightsRange, this->upperWeightsRange);
			return *this;
		}

	private:
		std::vector<size_t> layer_sizes;
		size_t input_size, output_size, network_size;

		vector2D output;
		vector3D weights;

		double lowerWeightsRange = -0.5f, upperWeightsRange = 0.5f;
		std::shared_ptr<CrossoverFunction> crossoverFunction;
		std::shared_ptr<ActivationFunction> activationFunction;
	};
} // namespace opml
