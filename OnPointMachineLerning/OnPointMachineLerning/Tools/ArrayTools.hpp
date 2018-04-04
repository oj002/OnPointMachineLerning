#pragma once
#include "Random.hpp"
#include <algorithm>
#include <vector>

#ifdef _DEBUG
#define DO_IN_DEBUG(debugCode, releaseCode) debugCode;
#else
#define DO_IN_DEBUG(debugCode, releaseCode) releaseCode;
#endif // _DEBUG

namespace opml
{
	using vector2D = std::vector<std::vector<double>>;
	using vector3D = std::vector<std::vector<std::vector<double>>>;
	using vector4D = std::vector<std::vector<std::vector<std::vector<double>>>>;

	namespace tools
	{
		static inline const std::vector<double>& convertFlattenedArray(const vector3D& array)
		{
			return array[0][0];
		}

		static inline const vector3D createComplexFlatArray(const std::vector<double>& input)
		{
			return vector3D(1, vector2D(1, input));
		}
		
		static rng_mt19937_std rng;

		static void randomizeVec(std::vector<double>* vec, double lowerBound, double upperBound)
		{
			for (std::_Vector_iterator<std::_Vector_val<std::_Simple_types<double> > >::value_type & iter : *vec)
			{
				iter = rng.randomReal<double>(lowerBound, upperBound);
			}
		}

		static void randomizeVec(vector2D* vec, double lowerBound, double upperBound)
		{
			for (auto & iter : *vec)
			{
				randomizeVec(&iter, lowerBound, upperBound);
			}
		}

		static void randomizeVec(vector3D* vec, double lowerBound, double upperBound)
		{
			for (auto & iter : *vec)
			{
				randomizeVec(&iter, lowerBound, upperBound);
			}
		}

		static void randomizeVec(vector4D* vec, double lowerBound, double upperBound)
		{
			for (auto & iter : *vec)
			{
				randomizeVec(&iter, lowerBound, upperBound);
			}
		}

		static std::vector<size_t> randomValues(size_t lowerBound, size_t upperBound, size_t amount)
		{

			lowerBound--;

			if (amount > (upperBound - lowerBound)) 
			{
				return std::vector<size_t>();
			}

			std::vector<size_t> values(amount, 0);
			for (size_t i = 0; i < amount; i++) 
			{
				auto n = rng.randomInteger<size_t>(lowerBound, upperBound);
				while (std::find(values.begin(), values.end(),n) != values.end())
				{
					n = rng.randomInteger<size_t>(lowerBound, upperBound);
				}
				values[i] = n;
			}
			return values;
		}

		static inline size_t indexOfHighestValue(std::vector<double> values)
		{
			size_t index = 0;
			for (size_t i = 1; i < values.size(); i++)
			{
				if (values[i] > values[index]) 
				{
					index = i;
				}
			}
			return index;
		}

		static inline double randomValue(double lower, double upper) 
		{
			return rng.randomReal<double>(lower, upper);
		}
	
	} // namespace tools
} // namespace opml