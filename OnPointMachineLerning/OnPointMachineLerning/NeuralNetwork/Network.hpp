#pragma once
#include "Functions\Error\ErrorFunction.hpp"
#include "Layers\DenseLayer.hpp"
#include "Layers\InputLayer.hpp"
#include "Layers\OutputLayer.hpp"
#include "..\data\TrainSet.hpp"
#include <iomanip>
#include <utility>

namespace opml
{
	class Network
	{
	public:
		Network() = default;

		Network(InputLayer* inputLayer, OutputLayer*  outputLayer)
			: inputLayer(inputLayer)
			, outputLayer(outputLayer)
			, overall_error(1.0)
		{
		}

		vector3D calculate(const vector3D& in)
		{
			try
			{
				if (!this->inputLayer->matchingDimensions(in)) { return vector3D(); }
				this->inputLayer->setInput(in);
				this->inputLayer->feedForwardRecursive();
				return getOutput();
			} 
			catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}

		// 24.4876
		void train(const vector3D&  input, const vector3D&  target, double eta, double wishError)
		{
			try
			{
				if (!this->inputLayer->matchingDimensions(input) || !this->outputLayer->matchingDimensions(target))
				{
					out_opml << OPML_MESSAGE("input and target data have wrong dimensions, not training with this data.") << '\n';
					return;
				}
				this->calculate(input);
				double error = this->outputLayer->overall_error(target);
				this->overall_error += error;
				if (error > wishError * 0.5)
				{
					this->backpropagateError(target);
					this->updateWeights(eta);
				}
			}
			catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}
		
		void train(TrainSet trainSet, size_t batch_size, size_t maxIterations, double eta, double wishError)
		{
			try
			{
				for (size_t it = 0; it < maxIterations; ++it)
				{
					++iterations;
					TrainSet currTrainSet = trainSet.extractBatch(batch_size);

					this->overall_error = 0.0;
					for (size_t k = 0; k < trainSet.size(); ++k)
					{
						train(currTrainSet.getInput(k), currTrainSet.getOutput(k), eta, wishError);
					}

					std::cout << "Iteration: " << iterations << "   MSE: " << (this->overall_error / static_cast<double>(currTrainSet.size())) << "   Learning rate: " << eta << '\n';

					if ((this->overall_error / static_cast<double>(currTrainSet.size())) <= wishError)
					{
						std::cout << "wishError fulfilled\n";
						break;
					}
					trainSet.shuffle();
				}
			}
			catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}

		void printArchitecture()
		{
			try
			{
				Layer* cur = inputLayer;

				std::cout << typeid(*cur).name();
				std::cout << ">>>>> ns: [" << cur->OUTPUT_DEPTH << " " << cur->OUTPUT_WIDTH << " " << cur->OUTPUT_HEIGHT << "] <<<<<<<";

				while (cur->next_layer != nullptr)
				{
					cur = cur->next_layer;
					std::cout << '\n' << typeid(*cur).name() << "\n>>>>> ns: [" << cur->OUTPUT_DEPTH << " " << cur->OUTPUT_WIDTH << " " << cur->OUTPUT_HEIGHT << "] <<<<<<<";
				}
			}
			catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}

		void analyseNetwork() 
		{
			try
			{
				Layer* cur = inputLayer;

				std::cout << typeid(*cur).name() << '\n';
				std::cout << ">>>>> ns: [" << cur->OUTPUT_DEPTH << " " << cur->OUTPUT_WIDTH << " " << cur->OUTPUT_HEIGHT << "] <<<<<<<\n";

				while (cur->next_layer != nullptr)
				{
					cur = cur->next_layer;
					std::cout << "################################################################################################\n\n";
					std::cout << typeid(*cur).name() << '\n';
					std::cout << ">>>>> ns: [" << cur->OUTPUT_DEPTH << " " << cur->OUTPUT_WIDTH << " " << cur->OUTPUT_HEIGHT << "] <<<<<<<\n";
					Layer::printArray(cur->output_values);
					Layer::printArray(cur->output_derivative_values);
					Layer::printArray(cur->output_error_values);
				}
			}
			catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}

		inline Network setErrorFunction(const std::shared_ptr<ErrorFunction>& errorFunction) const
		{
			try
			{
				this->outputLayer->setErrorFunction(errorFunction);
				return *this;
			}
			catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}

		inline vector3D getOutput() const noexcept { return this->outputLayer->output_values; }

	private:
		void backpropagateError(const vector3D&  targetOutput)
		{
			if (!this->outputLayer->matchingDimensions(targetOutput)) { return; }
			this->outputLayer->calculateOutputErrorSignals(targetOutput);
			this->outputLayer->feedBackwardRecursive();
		}
		void updateWeights(double eta)
		{
			this->inputLayer->updateWeightsRecursive(eta);
		}

	private:
		InputLayer* inputLayer;
		OutputLayer* outputLayer{};
		size_t iterations{ 0 };

		double overall_error;
	};
} // namespace opml