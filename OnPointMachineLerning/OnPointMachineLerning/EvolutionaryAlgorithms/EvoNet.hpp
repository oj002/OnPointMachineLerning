#pragma once
#include "Functions/Crossover/CrossoverFunction.hpp"
#include "../Functions/Activation/ActivationFunction.hpp"
#include "../Utils/Utils.hpp"
#include "DNA.hpp"
#include <memory>
#include <cassert>

namespace opml
{
	class EvoNet : public DNA<EvoNet>
	{
	public:
		EvoNet(const std::vector<size_t> &inputSizes) noexcept
			try : layer_sizes(inputSizes)
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
			randomizeVec(&this->weights, this->lowerWeightsRange, this->upperWeightsRange);
		}
		catch (std::exception &e) { out_opml << e.what() << '\n'; }

		std::vector<double> calculate(const std::vector<double> &input) noexcept
		{
			try
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
			catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}

		virtual void mutate(double persentage) noexcept override
		{
			try
			{
				for (vector2D &vec2 : weights)
				{
					for (std::vector<double> &vec : vec2)
					{
						for (double &d : vec)
						{
							if (opml::rng.next<double>(0, 100) < persentage)
							{
								d = opml::rng.next<double>(this->lowerWeightsRange, this->upperWeightsRange);
							}
						}
					}
				}
			}
			catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}

		virtual std::shared_ptr<EvoNet> crossover(const std::shared_ptr<EvoNet> parent) const noexcept override
		{
			try
			{
				std::shared_ptr<EvoNet> ret{ std::make_shared<EvoNet>(*this) };
				vector3D newWeights{ this->weights };
				for (size_t i = 0; i < this->weights.size(); ++i)
				{
					for (size_t j = 0; j < this->weights[i].size(); ++j)
					{
						if (this->weights[i][j].size() > 0)
						{
							newWeights[i][j] = crossoverFunction->crossover(this->weights[i][j], parent->weights[i][j]);
						}
					}
				}
				ret->weights = newWeights;
				return ret;
			}
			catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}

		void setActivationFunction(std::shared_ptr<ActivationFunction> activationFunction) noexcept
		{
			this->activationFunction = activationFunction;
		}

		void setCrossoverFunction(std::shared_ptr<CrossoverFunction> crossoverFunction) noexcept
		{
			this->crossoverFunction = crossoverFunction;
		}

		void weightsRange(double lower, double upper) noexcept
		{
			this->lowerWeightsRange = lower;
			this->upperWeightsRange = upper;
			randomizeVec(&this->weights, this->lowerWeightsRange, this->upperWeightsRange);
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
