#pragma once
#include "ErrorFunction.hpp"

namespace opml
{
	struct MSE : public ErrorFunction
	{
		inline double overall_error(const OutputLayer& outputLayer, const vector3D&  target) const noexcept override
		{
			try
			{
				double v{ 0.0 };

				const vector3D& output_values(outputLayer.get_output_values());

				for (size_t i = 0; i < output_values.size(); ++i)
				{
					for (size_t n = 0; n < output_values[0].size(); ++n)// TODO(Bernstein): if an error happens change to [i]
					{
						for (size_t j = 0; j < output_values[0][0].size(); ++j)// TODO(Bernstein): if an error happens change to [i][n]
						{
							v += (output_values[i][n][j] - target[i][n][j]) * (output_values[i][n][j] - target[i][n][j]);
						}
					}
				}

				size_t count = output_values.size() * output_values[0].size() * output_values[0][0].size();
				return v / (2.0 * static_cast<double>(count));
			} catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}

		inline void apply(OutputLayer* outputLayer, const vector3D& target) const noexcept override
		{
			try
			{
				const vector3D& output = outputLayer->get_output_values();
				const vector3D& output_derivative = outputLayer->get_output_derivative_values();
				vector3D error_signals(outputLayer->get_output_error_values());

				for (size_t i = 0; i < output.size(); ++i)
				{
					for (size_t n = 0; n < output[0].size(); ++n)// TODO(Bernstein): if an error happens change to [i]
					{
						for (size_t j = 0; j < output[0][0].size(); ++j)// TODO(Bernstein): if an error happens change to [i][n]
						{
							error_signals[i][n][j] = output_derivative[i][n][j] * (output[i][n][j] - target[i][n][j]);
						}
					}
				}

				outputLayer->set_output_error_values(error_signals);
			} catch (std::exception &e) { out_opml << e.what() << '\n'; }
		}
	};
}  // namespace opml