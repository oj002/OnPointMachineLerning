#pragma once
#include "Functions/Crossover/CrossoverFunction.hpp"
#include <memory>
#include "../Tools/Random.hpp"

namespace opml
{
	class EvoNet
	{
	public:
		EvoNet()
		{

		}

		void calculate()
		{
			/*const std::vector<double> prev_output_values00(this->prev_layer->get_output_values()[0][0]);
			std::vector<double>& output_values00(this->output_values[0][0]);
			std::vector<double>& output_derivative_values00(this->output_derivative_values[0][0]);

			#pragma omp parallel for
			for (int i = 0; i < this->OUTPUT_HEIGHT; ++i)
			{
				double sum{ 0 };

				for (size_t prevNeuron = 0; prevNeuron < this->INPUT_HEIGHT; ++prevNeuron)
				{
					sum += prev_output_values00[prevNeuron] * this->weights[prevNeuron];
				}

				output_values00[i] = activationFunction->activation(sum);
				output_derivative_values00[i] = activationFunction->activation_prime(sum);
			}*/
		}

		void mutate(double mutationRate)
		{
			for (std::vector<double>::iterator it = weights.begin(); it != weights.end(); ++it)
			{
				if (rng.randomReal<double>(0, 1) < mutationRate)
				{
					*it = rng.randomReal<double>(this->lowerWeightsRange, this->upperWeightsRange);
				}
			}
		}

		void crossover()
		{
		}

	private:
		double lowerWeightsRange = 0.0, upperWeightsRange = 1.0;
		std::vector<double> weights{};
		std::shared_ptr<CrossoverFunction> crossoverFunction;

		static rng_mt19937_std rng;
	};
}  // namespace opml