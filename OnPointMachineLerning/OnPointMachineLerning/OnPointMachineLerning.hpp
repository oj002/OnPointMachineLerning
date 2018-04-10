#pragma once

#include "Examples\FlappyBirds_Evo.hpp"
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

#include "Utils\Utils.hpp"

#include "EvolutionaryAlgorithms\EvoNet.hpp"
#include "EvolutionaryAlgorithms\Functions\Crossover\CrossoverFunction.hpp"
#include "EvolutionaryAlgorithms\Functions\Crossover\Midpoint.hpp"

