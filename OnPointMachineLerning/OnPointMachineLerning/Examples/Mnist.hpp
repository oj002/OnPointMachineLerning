#pragma once
#include "../NeuralNetwork/NetworkBuilder.hpp"
#include "../NeuralNetwork/Functions/Functions.hpp"

#include <fstream>
#include <sstream>

namespace opml::Examples
{
	class Mnist_NN
	{
	public:
		template<typename ...Args>
		Mnist_NN(Args... layerSizes)
			: networkBuilder(1, 1, 28 * 28)
			, set(1, 1, 28 * 28, 1, 1, 10)
			, testSet(1, 1, 28 * 28, 1, 1, 10)
			, lastDenseLayer(10)
		{
			(layers.emplace_back(layerSizes), ...);
			for (DenseLayer &layer : layers)
			{
				layer.weightsRange(-1, 1).biasRange(-1, 1).setActivationFunction(sigmoid);
				networkBuilder.addLayer(&layer);
			}
			
			lastDenseLayer.weightsRange(-1, 1).biasRange(-1, 1).setActivationFunction(sigmoid);
			networkBuilder.addLayer(&lastDenseLayer);

			network = networkBuilder.buildNetwork();

			load_mnist_TrainingData("res/Examples/mnist/mnist_train.csv", &set, 60000);
			load_mnist_TrainingData("res/Examples/mnist/mnist_test.csv", &testSet, 10000);
		}

		void train(size_t num_epochs = 10, double eta = 0.1, double wish_error = 0.001)
		{
			network.train(set, set.size(), num_epochs, eta, wish_error);
		}

		void test()
		{
			size_t count = 0;

			for (size_t i = 0; i < testSet.size(); ++i)
			{
				auto res = network.calculate(testSet.getInput(i));
				double resMax = 0.0;
				size_t resIndex = 0;
				for (size_t j = 0; j < 10; ++j)
				{
					if (res[0][0][j] > resMax)
					{
						resMax = res[0][0][j];
						resIndex = j;
					}
				}
				auto tar = testSet.getOutput(i);
				double tarMax = 0.0;
				size_t tarIndex = 0;
				for (size_t j = 0; j < 10; ++j)
				{
					if (tar[0][0][j] > tarMax)
					{
						tarMax = tar[0][0][j];
						tarIndex = j;
					}
				}
				if (resIndex == tarIndex)
				{
					++count;
				}
			}
			std::cout << "acc: (" << count << "/10000): " << count / 10000.0 << "\n\n";
		}

	private:
		void load_mnist_TrainingData(const char* filename, opml::TrainSet* dataSet, size_t trainingSize)
		{
			std::ifstream fin(filename);

			if (fin.is_open())
			{
				for (size_t i = 0; i < trainingSize; ++i)
				{
					opml::vector3D input(1, opml::vector2D(1, std::vector<double>()));
					input.reserve(28 * 28);
					opml::vector3D target(1, opml::vector2D(1, std::vector<double>(10, 0.0)));

					char buff = ' ';
					int iIndex = 0;
					fin >> iIndex;

					target[0][0][iIndex] = 1.0;
					double val = 0;


					for (size_t h = 1; h < 29; ++h)
					{
						for (size_t w = 0; w < 28; ++w)
						{
							fin >> buff;
							fin >> val;

							input[0][0].emplace_back(val / 255.0);
						}
					}
					dataSet->addData(input, target);

				}
			}
			fin.close();
		}
	
	private:
		opml::NetworkBuilder networkBuilder;
		opml::TrainSet set;
		TrainSet testSet;
		opml::Network network;
		std::vector<DenseLayer> layers;
		opml::DenseLayer lastDenseLayer;
		std::shared_ptr<Sigmoid> sigmoid = std::make_shared<Sigmoid>();
	};
}