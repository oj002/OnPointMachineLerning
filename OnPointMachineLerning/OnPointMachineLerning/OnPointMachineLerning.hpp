#pragma once

#include "Examples\FlapyBirds_Evo.hpp"
#include "Examples\Mnist_NN.hpp"

#include "data\TrainSet.hpp"

#include "NeuralNetwork\Functions\Error\ErrorFunction.hpp"
#include "NeuralNetwork\Functions\Error\MSE.hpp"
#include "NeuralNetwork\Layers\DenseLayer.hpp"
#include "NeuralNetwork\Layers\InputLayer.hpp"
#include "NeuralNetwork\Layers\Layer.hpp"
#include "NeuralNetwork\Layers\OutputLayer.hpp"
#include "NeuralNetwork\Layers\PoolingLayer.hpp"
#include "NeuralNetwork\Layers\TransformLayer.hpp"
#include "NeuralNetwork\Network.hpp"
#include "NeuralNetwork\NetworkBuilder.hpp"

#include "Tools\ArrayTools.hpp"
#include "Tools\Clock.hpp"
#include "Tools\Random.hpp"

#include "EvoNet\EvoNet.hpp"
#include "EvoNet\Functions\Crossover\CrossoverFunction.hpp"
#include "EvoNet\Functions\Crossover\Midpoint.hpp"

