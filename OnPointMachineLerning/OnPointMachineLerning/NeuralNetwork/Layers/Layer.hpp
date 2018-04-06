#pragma once
#include "../../Utils/Utils.hpp"
#include <cassert>
#include <iostream>

namespace opml
{
	class Layer
	{
	public:
		friend class Network;
		Layer() = default;

		Layer(size_t OUTPUT_DEPTH, size_t OUTPUT_WIDTH, size_t OUTPUT_HEIGHT);

		void connectToPreviouseLayer(Layer* prevLayer);

		virtual void calculate() = 0;
		void feedForwardRecursive();

		virtual void backpropError() = 0;
		void feedBackwardRecursive();

		virtual void updateWeights(double eta) = 0;
		void updateWeightsRecursive(double eta);

		static inline bool matchingDimensions(const Layer& layer, vector3D  values) { return layer.OUTPUT_DEPTH == values.size() && layer.OUTPUT_WIDTH == values[0].size() && layer.OUTPUT_HEIGHT == values[0][0].size(); }
		bool matchingDimensions(const vector3D&  values) { return matchingDimensions(*this, values); }

		static void printArray(const vector3D& array);


		inline void set_output_values(const vector3D&  vec) { if (this->matchingDimensions(vec)) { this->output_values = vec; } }
		inline void set_output_derivative_values(const vector3D&  vec) { if (this->matchingDimensions(vec)) { this->output_derivative_values = vec; } }
		inline void set_output_error_values(const vector3D&  vec) { if (this->matchingDimensions(vec)) { this->output_error_values = vec; } }

		inline const vector3D& get_output_values() const { return this->output_values; }
		inline const vector3D& get_output_derivative_values() const { return this->output_derivative_values; }
		inline const vector3D& get_output_error_values() const { return this->output_error_values; }

	protected:
		virtual void calculateOutputDimensions() = 0;
		virtual void on_build() = 0;

	private:
		void initializeArrays();

		//maybe change to public
	protected:
		size_t INPUT_DEPTH{0}, INPUT_WIDTH{0}, INPUT_HEIGHT{0};
		size_t OUTPUT_DEPTH{0}, OUTPUT_WIDTH{0}, OUTPUT_HEIGHT{0};

		vector3D output_values;
		vector3D output_derivative_values;
		vector3D output_error_values;

		Layer* prev_layer{nullptr};
		Layer* next_layer{nullptr};

	};
}  // namespace opml