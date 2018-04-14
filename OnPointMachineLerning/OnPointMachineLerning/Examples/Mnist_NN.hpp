#pragma once
#include "../Functions/Activation/Sigmoid.hpp"
#include "../NeuralNetwork/NetworkBuilder.hpp"

#include <fstream>
#include <opencv2\opencv.hpp>
#include <sstream>
#include <string>

namespace opml::Examples
{
	class Mnist_NN
	{
	public:
		template<typename ...Args>
		explicit Mnist_NN(Args... layerSizes)
			: networkBuilder(1, 1, 28 * 28)
			, set(1, 1, 28 * 28, 1, 1, 10)
			, testSet(1, 1, 28 * 28, 1, 1, 10)
			, lastDenseLayer(10)
		{
			(layers.emplace_back(layerSizes), ...);
			for (DenseLayer &layer : layers)
			{
				layer.weightsRange(-0.5, 0.5).biasRange(-0.5, 0.5).setActivationFunction(sigmoid);
				networkBuilder.addLayer(&layer);
			}
			
			lastDenseLayer.weightsRange(-0.5, 0.5).biasRange(-0.5, 0.5).setActivationFunction(sigmoid);
			networkBuilder.addLayer(&lastDenseLayer);

			network = networkBuilder.buildNetwork();

			load_mnist_TrainingData("res/Examples/mnist/mnist_train.csv", &set, 60000);
			load_mnist_TrainingData("res/Examples/mnist/mnist_test.csv", &testSet, 10000);
			set.shuffle();
			testSet.shuffle();
		}

		void train(size_t num_epochs = 10, double eta = 0.1, double wish_error = 0.001)
		{
			set.shuffle();
			network.train(set, set.size(), num_epochs, eta, wish_error);
		}

		void show(size_t num)
		{
			for (size_t i = 0; i < num; ++i)
			{
				const opml::vector3D &input = testSet.getInput(i);
				const opml::vector3D &res = network.calculate(input);
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
				const opml::vector3D &tar = testSet.getOutput(i);
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
				cv::Mat mat = cv::Mat::ones(28, 28, CV_32FC3);
				for (int x = 0; x < 28; ++x)
				{
					for (int y = 0; y < 28; ++y)
					{
						mat.at<cv::Vec3f>(x, y) = cv::Vec3f(static_cast<float>(input[0][0][x * 28 + y]));
					}
				}
				cv::resize(mat, mat, { 280, 280 });
				if (resIndex == tarIndex)
				{
					cv::putText(mat, std::to_string(resIndex), { 120, 140 }, CV_FONT_HERSHEY_SIMPLEX , 2.5, { 0, 255, 0 });
				}
				else
				{
					cv::putText(mat, std::to_string(resIndex), { 120, 140 }, CV_FONT_HERSHEY_SIMPLEX, 2.5, { 0, 0, 255 });
				}
				
				cv::putText(mat, "press any key to continue...", { 35, 250 }, CV_FONT_HERSHEY_SIMPLEX, 0.5, { 255, 255, 255 });
				cv::imshow("Mnist-Sample-Image", mat);
				cv::waitKey();
			}
			cv::destroyWindow("Mnist-Sample-Image");
		}

		void test()
		{
			testSet.shuffle();
			size_t count = 0;
			for (size_t i = 0; i < testSet.size(); ++i)
			{
				const opml::vector3D &res = network.calculate(testSet.getInput(i));
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
				const opml::vector3D &tar = testSet.getOutput(i);
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

					std::string line;
					std::getline(fin, line);
					std::stringstream lineStream(line);
					std::string cell;

					std::getline(lineStream, cell, ',');
					target[0][0][std::stoi(cell)] = 1.0;

					while (std::getline(lineStream, cell, ','))
					{
						input[0][0].push_back(std::stoi(cell) / 255.0);
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
} // namespace Examples // namespace opml