#pragma once
#include "../Utils/Utils.hpp"
#include "DNA.hpp"

namespace opml
{
	template<typename T>
	struct Population
	{
		Population<T>() = default;

		void nextGeneration(double mutationChance, double mutationAmount)
		{
			const size_t popSize{ this->population.size() };
			std::vector<std::shared_ptr<T>> newPopulation{ this->population };
			double maxfit{ 0 };
			size_t index{ 0 };
			for (int i = 0; i < popSize; ++i)
			{
				if (this->population[i]->fitness > maxfit)
				{
					maxfit = this->population[i]->fitness;
					index = i;
				}
			}
			std::shared_ptr<T> best(newPopulation[index]);

			for (int i = 0; i < popSize; ++i)
			{
				std::shared_ptr<T> parentA{ acceptReject(maxfit) };
				std::shared_ptr<T> parentB{ acceptReject(maxfit) };
				std::shared_ptr<T> childNet{ parentA->crossover(parentB.get()) };
				if (rng.next<double>(0, 100) < mutationChance)
				{
					childNet->mutate(mutationAmount);
				}

				newPopulation[i] = childNet;
			}

			newPopulation[index] = best;


			this->population = newPopulation;
		}


		std::shared_ptr<T> acceptReject(double maxfit)
		{
			size_t besave{ 0 };
			while (true)
			{
				const size_t index{ opml::rng.next<size_t>(0, this->population.size() - 1) };

				const double r{ opml::rng.next<double>(0, maxfit) };
				if (r < this->population[index]->fitness)
				{
					return this->population[index];
				}
				++besave;
				if (besave > 10000)
				{
					// fuck it 
					return this->population[index];
				}
			}
		}

		std::vector<std::shared_ptr<T>> population;
	};
} // namespace opml