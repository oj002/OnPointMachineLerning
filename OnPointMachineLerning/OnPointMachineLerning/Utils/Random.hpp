#pragma once
#include "Exeptions.hpp"
#include <random>

namespace opml
{
	class rng_mt19937_std
	{
	private:
		std::random_device randomDevice;
		std::mt19937 mt;

	public:

		rng_mt19937_std()
			try : mt(randomDevice())
		{
		} OPML_INTERNAL_CATCH

		explicit rng_mt19937_std(unsigned int seed)
			try : mt(seed)
		{
		} OPML_INTERNAL_CATCH

		template<typename T>
		T randomInteger()
		{
			try 
			{
				std::uniform_int_distribution<T> dist(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
				return dist(mt);
			} OPML_INTERNAL_CATCH
		}

		template<typename T>
		T randomInteger(T min, T max)
		{
			try 
			{
				std::uniform_int_distribution<T> dist(min, max);
				return dist(mt);
			} OPML_INTERNAL_CATCH
		}

		template<typename T>
		T randomReal()
		{
			try
			{
				std::uniform_real_distribution<T> dist(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
				return dist(mt);
			} OPML_INTERNAL_CATCH
		}

		template<typename T>
		T randomReal(T min, T max)
		{
			try
			{
				std::uniform_real_distribution<T> dist(min, max);
				return dist(mt);
			} OPML_INTERNAL_CATCH
		}

		template<typename T>
		T normal(T mean, T deviation)
		{
			try
			{
				std::normal_distribution<T> dist(mean, deviation);
				return dist(mt);
			} OPML_INTERNAL_CATCH
		}

		std::mt19937& getMt19937() noexcept
		{
			return mt;
		}
	};
} // namespace opml