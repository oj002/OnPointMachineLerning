#include "Layer.hpp"

namespace opml
{
	Layer::Layer()
		 
	= default;

	Layer::Layer(size_t OUTPUT_DEPTH, size_t OUTPUT_WIDTH, size_t OUTPUT_HEIGHT)
		: INPUT_DEPTH(0)
		, INPUT_WIDTH(0)
		, INPUT_HEIGHT(0)
		, OUTPUT_DEPTH(OUTPUT_DEPTH)
		, OUTPUT_WIDTH(OUTPUT_WIDTH)
		, OUTPUT_HEIGHT(OUTPUT_HEIGHT)
		, prev_layer(nullptr)
		, next_layer(nullptr)
	{
	}

	void Layer::connectToPreviouseLayer(Layer * prevLayer)
	{
		this->prev_layer = prevLayer;
		this->prev_layer->next_layer = this;

		this->INPUT_DEPTH = this->prev_layer->OUTPUT_DEPTH;
		this->INPUT_WIDTH = this->prev_layer->OUTPUT_WIDTH;
		this->INPUT_HEIGHT = this->prev_layer->OUTPUT_HEIGHT;

		if (this->OUTPUT_DEPTH == 0 || this->OUTPUT_WIDTH == 0 || this->OUTPUT_HEIGHT == 0)
		{
			this->calculateOutputDimensions();
		}

		if (this->OUTPUT_DEPTH == 0 || this->OUTPUT_WIDTH == 0 || this->OUTPUT_HEIGHT == 0)
		{
			std::cout << "Bad Dimensions";
			assert(false);
		}

		this->initializeArrays();
		this->on_build();
	}

	void Layer::feedForwardRecursive()
	{
		this->calculate();

		if (this->next_layer != nullptr)
		{
			this->next_layer->feedForwardRecursive();
		}
	}

	void Layer::feedBackwardRecursive()
	{
		this->backpropError();
		if (this->prev_layer != nullptr)
		{
			this->prev_layer->feedBackwardRecursive();
		}
	}

	void Layer::updateWeightsRecursive(double eta)
	{
		this->updateWeights(eta);
		if (this->next_layer != nullptr) 
		{
			this->next_layer->updateWeightsRecursive(eta);
		}
	}

	void Layer::printArray(const vector3D & array)
	{
		if (array.size() == 1 && array[0].size() == 1)
		{
			std::cout << '[';
			for (std::_Vector_const_iterator<std::_Vector_val<std::_Simple_types<double> > >::value_type iter : array[0][0])
			{
				std::cout << iter << " ";
			}
			std::cout << "]\n";
		}
		else
		{
			std::cout << "[\n";
			for (size_t n = 0; n < array[0][0].size() - 1; ++n)
			{
				std::cout << "[";
				for (size_t i = 0; i < array.size(); ++i)
				{
					std::cout << '[';
					for (size_t k = 0; k < array[0].size(); ++k)
					{
						std::cout << array[i][k][n] << ' ';
					}
					std::cout << "], ";
				}
				std::cout << "],\n";
			}
			std::cout << "]\n";
		}
	}

	void Layer::initializeArrays()
	{
		this->output_values.resize(this->OUTPUT_DEPTH, vector2D(this->OUTPUT_WIDTH, std::vector<double>(this->OUTPUT_HEIGHT, 0.0)));
		this->output_derivative_values = this->output_values;
		this->output_error_values = this->output_values;
	}
} // namespace opml